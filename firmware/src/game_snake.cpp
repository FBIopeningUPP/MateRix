#include "engine.h"

void RunSnake(int input) {
    static int8_t sX[100], sY[100];
    static int sLen = 0;
    static int dX = 1, dY = 0;
    static int fX = 7, fY = 7;
    static unsigned long lastMove = 0;
    static bool dead = false;

    if (sLen == 0 || dead) {
        if (input == PIN_BTN_A) {
            sLen = 3;
            for(int i=0; i<3; i++) { sX[i] = 5 - i; sY[i] = 5; }
            dX = 1; dY = 0;
            fX = random(10); fY = random(10);
            dead = false;
        }
        if(dead) DrawPixel(sX[0], sY[0], CRGB::Purple);
        return;
    }

    if(input == PIN_JOY_UP && dY != 1) { dX = 0; dY = -1; }
    if(input == PIN_JOY_DOWN && dY != -1) { dX = 0; dY = 1; }
    if(input == PIN_JOY_LEFT && dX != 1) { dX = -1; dY = 0; }
    if(input == PIN_JOY_RIGHT && dX != -1) { dX = 1; dY = 0; }

    if(millis() - lastMove > 200) {
        lastMove = millis();

        int headX = sX[0] + dX;
        int headY = sY[0] + dY;

        if(headX < 0 || headX > 9 || headY < 0 || headY > 9) dead = true;

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

            if(headX == fX && headY == fY) {
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