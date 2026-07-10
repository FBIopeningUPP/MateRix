#include <Arduino.h>
#include "engine.h"

void RunPCMode();
void RunSnake(int input);
void RunTetris(int input);

enum State {MENU, SNAKE, PC_MODE, TETRIS};
State state = MENU;
int menuSelection = 0;

void setup() {
  Serial.begin(115200);
  Engine_Init();
}

void loop() {
    int input = ReadInputs();

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
    }

    ShowScreen();
    delay(16);
}