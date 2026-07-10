#include "engine.h"
#define PIN_TL 4
#define PIN_TR 5
#define PIN_BL 6
#define PIN_BR 7
#define NUM_LEDS 25

CRGB leds_tl[NUM_LEDS], leds_TR[NUM_LEDS], leds_BL[NUM_LEDS], leds_BR[NUM_LEDS];

void Engine_Init() {
    int pins[] = {PIN_JOY_UP, PIN_JOY_DOWN, PIN_JOY_LEFT, PIN_JOY_RIGHT, PIN_JOY_PRESS,
                  PIN_BTN_A, PIN_BTN_B, PIN_BTN_X, PIN_BTN_Y, PIN_BTN_START};
    for (int p : pins) pinMode(p, INPUT_PULLUP);

    FastLED.addLeds<WS2812, PIN_TL, GRB>(leds_tl, NUM_LEDS);
    FastLED.addLeds<WS2812, PIN_TR, GRB>(leds_TR, NUM_LEDS);
    FastLED.addLeds<WS2812, PIN_BL, GRB>(leds_BL, NUM_LEDS);
    FastLED.addLeds<WS2812, PIN_BR, GRB>(leds_BR, NUM_LEDS);
    FastLED.setBrightness(128);
}

int ReadInputs() {
    static unsigned long lastPress = 0;
    if (millis() - lastPress < 150) return -1;

    int pins[] = {PIN_JOY_UP, PIN_JOY_DOWN, PIN_JOY_LEFT, PIN_JOY_RIGHT, PIN_JOY_PRESS,
                  PIN_BTN_A, PIN_BTN_B, PIN_BTN_X, PIN_BTN_Y, PIN_BTN_START};
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
    int idx = (ly % 2 == 0) ? (ly * 5) + lx : (ly * 5) + (4 - lx);

    if(x < 5 && y < 5) leds_tl[idx] = color;
    else if(x >= 5 && y < 5) leds_TR[idx] = color;
    else if(x < 5 && y >= 5) leds_BL[idx] = color;
    else leds_BR[idx] = color;
}

void ClearScreen() {
    FastLED.clear();
}

void ShowScreen() {
    FastLED.show();
}

void FadeScreen(uint8_t amount) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds_tl[i].fadeToBlackBy(amount);
        leds_TR[i].fadeToBlackBy(amount);
        leds_BL[i].fadeToBlackBy(amount);
        leds_BR[i].fadeToBlackBy(amount);
    }
}