#include <Arduino.h>
#include <FastLED.h>

#define PIN_TL 4
#define PIN_TR 5
#define PIN_BL 6
#define PIN_BR 7
#define NUM_LEDS 25

CRGB leds_TL[NUM_LEDS], leds_TR[NUM_LEDS], leds_BL[NUM_LEDS], leds_BR[NUM_LEDS];

enum State {MENU, SNAKE, PC_MODE};
State state = MENU;
int menuSelection = 0;

int ReadInput() {
    static unsigned long lastPress = 0;
    if (millis() - lastPress < 150) return -1; ///150ms ka debounce bhains ki shi mein bta bhai

    //pins hai input out valli and xyzbdsadsa saari piuns
    int pins[] = {15, 16, 17, 18, 21, 8, 9, 10, 11, 12};
    for (int p : pins) {
        if (digitalRead(p) == LOW) {
            lastPress = millis();
            return p;
        }
    }
    return -1;
}

void DrawPixel(int x, int y, CRGB color) {
    if (x < 0 || x > 9 || y < 0 || y > 9) return;

    int lx = x % 5, ly = y % 5;
    int idx = (ly % 2 == 0) ? (ly * 5) + lx : (ly * 5) + (4 - lx); //math hai bekar subjhect mat karo baccho 

    if (x < 5 && y < 5) leds_TL[idx] = color;
    else if (x > 4 && y < 5) leds_TR[idx] = color;
    else if (x < 5 && y > 4) leds_BL[idx] = color;
    else leds_BR[idx] = color;
}

void setup() {
    Serial.begin(115200);

    int pins[] = {15, 16, 17, 18, 21, 8, 9, 10, 11, 12};
    for (int p : pins) pinMode(p, INPUT_PULLUP);

    FastLED.addLeds<WS2812B, PIN_TL, GRB>(leds_TL, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_TR, GRB>(leds_TR, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_BL, GRB>(leds_BL, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_BR, GRB>(leds_BR, NUM_LEDS);
    FastLED.setBrightness(50);
}

void RunPCMode() {
    static int byteCount = 0;
    static uint8_t rgb[3];
    static unsigned long lastByteTime = 0;

    if (millis() - lastByteTime > 50) byteCount = 0; 
    
    while (Serial.available()) {
        lastByteTime = millis();

        rgb[byteCount % 3] = Serial.read();

        if ((byteCount % 3) == 2) {
            int pixelIndex = byteCount / 3;
            DrawPixel(pixelIndex % 10, pixelIndex / 10, CRGB(rgb[0], rgb[1], rgb[2]));
        }

        byteCount++;
        if (byteCount >= 300) byteCount = 0; 
    }
}

void RunSnake(int input) {
    static int8_t sX[100], sY[100];
    static int sLen = 0;
    static int dX = 1, dY = 0;
    static int fX = 7, fY = 5;
    static unsigned long lastMove = 0;
    static bool dead = false;

    if (sLen == 0 || dead) {
        if (input == 8) {
            sLen = 3;
            for(int i=0; i<3; i++) { sX[i] = 4 - i; sY[i] = 5; }
            dX = 1; dY = 0;
            fX = random(10); fY = random(10);
            dead = false;
        }
        if (dead) DrawPixel(sX[0], sY[0], CRGB::Purple);
        return;
    }

    if (input == 15 && dY != 1) { dX = 0; dY = -1; } 
    if (input == 16 && dY != -1) { dX = 0; dY = 1; }
    if (input == 17 && dX != 1) { dX = -1; dY = 0; }
    if (input == 18 && dX != -1) { dX = 1; dY = 0; }

    if (millis() - lastMove > 200) {
        lastMove = millis();

        int headX = sX[0] + dX;
        int headY = sY[0] + dY;

        if (headX < 0 || headX > 9 || headY < 0 || headY > 9) dead = true;

        for (int i = 0; i < sLen; i++) {
            if (headX == sX[i] && headY == sY[i]) dead = true;
        }

        if (!dead) {
            for (int i = sLen - 1; i > 0; i--) {
                sX[i] = sX[i - 1];
                sY[i] = sY[i - 1];
            }
            sX[0] = headX;
            sY[0] = headY;

            if (headX == fX && headY == fY) {
                if (sLen < 100) sLen++;
                fX = random(10);
                fY = random(10);
            }
        }
    }

    DrawPixel(fX, fY, CRGB::Red);
    DrawPixel(sX[0], sY[0], CRGB::Yellow);
    for (int i = 1; i < sLen; i++) {
        DrawPixel(sX[i], sY[i], CRGB::Green);
    }
}

void loop() {
    int input = ReadInput();

    if (input == 9) state = MENU;
    
    FastLED.clear();

    if (state == MENU) {
        if (input == 18) menuSelection = 1;//right hai bhainki
        if (input == 17) menuSelection = 0;//left hai bhainki
        if (input == 8) state = (menuSelection == 0) ? SNAKE : PC_MODE;

        DrawPixel(4, 4, (menuSelection == 0) ? CRGB::Red : CRGB::DarkRed);
        DrawPixel(5, 4, (menuSelection == 1) ? CRGB::Blue : CRGB::DarkBlue);
    } else if (state == SNAKE) {
        RunSnake(input);
    } else if (state == PC_MODE) {
        RunPCMode();
    }

    FastLED.show();
    delay(32); // 60 freaking fps
}