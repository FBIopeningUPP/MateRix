#include "engine.h"

static float dropY[10];
static float dropSpeed[10];
static bool initialized = false;

CRGB screensaverColor = CRGB::Green;

void InitScreensaver() {
    for (int x = 0; x < 10 ; x++) {
        dropY[x] = random(-10, 0);
        dropSpeed[x] = random(10, 40) / 100.0f;
    }
    initialized = true;
}

void RunScreensaver() {
    if (!initialized) InitScreensaver();
    FadeScreen(40);
    for (int x = 0; x < 10; x++) {
        dropY[x] += dropSpeed[x];

        if (dropY[x] > 12) {
            dropY[x] = random(-5, 0);
            dropSpeed[x] = random(10, 50) / 100.0f;
        }

        int intY = (int)dropY[x];
        if (intY >= 0 && intY < 10) {
            DrawPixel(x, intY, screensaverColor);
        }
    }
}