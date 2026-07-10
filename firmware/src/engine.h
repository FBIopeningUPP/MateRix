#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
#include <FastLED.h>

#define PIN_JOY_UP 15
#define PIN_JOY_DOWN 16
#define PIN_JOY_LEFT 17
#define PIN_JOY_RIGHT 18
#define PIN_JOY_PRESS 21

#define PIN_BTN_A 8
#define PIN_BTN_B 9
#define PIN_BTN_X 10
#define PIN_BTN_Y 11
#define PIN_BTN_START 12

void Engine_Init();
int ReadInputs();
void DrawPixel(int x, int y, CRGB color);
void ClearScreen();
void ShowScreen();

#endif 