#include "engine.h"

void RunPCMode() {
    static int byteCount = 0;
    static uint8_t rgb[3];
    static unsigned long lastByteTime = 0;

    if (millis() - lastByteTime > 50) byteCount = 0; 

    while (Serial.available()) {
        lastByteTime = millis();
        rgb[ByteCount % 3] = Serial.read();

        if((byteCount % 3) == 2) {
            int pixelIndex = byteCount / 3;
            DrawPixel(pixelIndex % 10, pixelIndex / 10, CRGB(rgb[0], rgb[1], rgb[2]));
        }

        byteCount++;
        if (byteCount >= 300) byteCount = 0;
    }
}