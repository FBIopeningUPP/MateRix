#include <Arduino.h>
#include "engine.h"

void RunPCMode();
void RunSnake(int input);
void RunTetris(int input);
void RunScreensaver();

enum State {MENU, SNAKE, PC_MODE, TETRIS, SCREENSAVER};
State state = MENU;
int menuSelection = 0;

void setup() {
  Serial.begin(115200);
  Engine_Init();
}

void loop() {
    int input = ReadInputs();

    if (state != PC_MODE) {
        while (Serial.available()) {
            if (Serial.read() == OxFF) {
                delay(5);
                if (Serial.available() >= 4) {
                    uint8_t cmd = Serial.read();
                    uint8_t r = Serial.read();
                    uint8_t g = Serial.read();
                    uint8_t b = Serial.read();

                    if (cmd == 1) {
                        screensaverColor = CRGB(r, g, b);
                    }
                }
            }
        }
    }
    static unsigned long lastTelemetry = 0;
    if (millis() - lastTelemetry > 1000) {
        lastTelemetry = millis();
        if (state == MENU) Serial.println("State: MAIN MENU");
        else if (state == SNAKE) Serial.println("State: SNAKE");
        else if (state == PC_MODE) Serial.println("State: PC_MODE");
        else if (state == TETRIS) Serial.println("State: TETRIS");
        else if (state == SCREENSAVER) Serial.println("State: SCREENSAVER");
    }

    static unsigned long lastInteraction = 0;

    if (input != -1) {
        lastInteraction = millis();
        if (state == SCREENSAVER) {
            state = MENU;
            ClearScreen();
        }
    }

    if (millis() - lastInteraction > 10000 && state != PC_MODE && state != SCREENSAVER) {
        state = SCREENSAVER;
    }

    if (input == PIN_BTN_B) state = MENU;

    ClearScreen();

    if (state == MENU) {
        if (input == PIN_JOY_RIGHT) menuSelection = (menuSelection + 1) % 3;
        if (input == PIN_JOY_LEFT) menuSelection = (menuSelection - 1 < 0) ? 2 : menuSelection - 1;

        if (input == PIN_BTN_A) {
            if (menuSelection == 0) state = SNAKE;
            else if (menuSelection == 1) state = PC_MODE;
            else if (menuSelection == 2) state = TETRIS;
        }

        DrawPixel(4, 4, (menuSelection == 0) ? CRGB::Red : CRGB::DarkRed);
        DrawPixel(4, 5, (menuSelection == 1) ? CRGB::Blue : CRGB::DarkBlue);
        DrawPixel(4, 6, (menuSelection == 2) ? CRGB::Green : CRGB::DarkGreen);
    } else if (state == SNAKE) {
        RunSnake(input);
    } else if (state == PC_MODE) {
        RunPCMode();
    } else if (state == TETRIS) {
        RunTetris(input); 
    } else if (state == SCREENSAVER) {
        RunScreensaver();
    }

    ShowScreen();
    delay(16);
}