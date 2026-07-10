#include <Arduino.h>
#include "engine.h"

void RunPCMode();

enum State {MENU, SNAKE, PC_MODE};
State state = MENU;
int menuSelection = 0;

void setup() {
  Serial.begin(115200);
  Engine::Init();
}

void loop() {
    int input = readInput();

    if (input == PIN_BTN_B) state = MENU;

    ClearScreen();

    if (state == MENU) {
        if (input == PIN_JOY_RIGHT) menuSelection = 1;
        if (input == PIN_JOY_LEFT) menuSelection = 0;
        if (input == PIN_BTN_A) state = (menuSelection == 0) ? SNAKE : PC_MODE;

        DrawPixel(4, 4, (menuSelection == 0) ? CRGB::Red : CRGB::DarkRed);
        DrawPixel(5, 4, (menuSelection == 0) ? CRGB::Blue : CRGB::DarkBlue);
    } else if (state == SNAKE) {
        RunSnakeGame();
    } else if (state == PC_MODE) {
        RunPCMode();
    }

    ShowScreen();
    delay(16);
}