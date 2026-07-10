#include "engine.h"

static CRGB board[10][10];

static bool gameActive = false;

static int pieceX = 4;
static int pieceY = 0;
static CRGB pieceColor = CRGB::Cyan;

static int currentPiece[4][4];

const int TETROMINOES[7][4][4] = {
    {{0,0,0,0}
     {1,1,1,1}
     {0,0,0,0}
     {0,0,0,0}},

    {{1,0,0,0}
     {1,1,1,0}
     {0,0,0,0}
     {0,0,0,0}},

    {{0,0,1,0}
     {1,1,1,0}
     {0,0,0,0}
     {0,0,0,0}},
    
    {{0,1,1,0}
     {0,1,1,0}
     {0,0,0,0}
     {0,0,0,0}},

    {{0,1,1,0}
     {1,1,0,0}
     {0,0,0,0}
     {0,0,0,0}},

    {{0,1,0,0}
     {1,1,1,0}
     {0,0,0,0}
     {0,0,0,0}},

    {{1,1,0,0}
     {0,1,1,0}
     {0,0,0,0}
     {0,0,0,0}}
};

void SpawnNewPiece

void ResetTetris() {
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            board[y][x] = CRGB::Black;
        }
    }
    pieceX = 4;
    pieceY = 0;
    gameActive = true;
}

void RunTetris(int input) {
    static unsigned long lastGravity = 0;

    if (!gameActive) {
        if (input == PIN_BTN_A) ResetTetris();
        return;
    }

    if (input == PIN_JOY_LEFT) pieceX--;
    if (input == PIN_JOY_RIGHT) pieceX++;

    if (millis() - lastGravity > 500) {
        lastGravity = millis();
        pieceY++;

        if (pieceY > 8) {
            for (int y=0; y<2; y++) {
                for (int x=0; x<2; x++) {
                    if (currentPiece[y][x] == 1) board[pieceY + y][pieceX + x] = pieceColor;
                }
            }

            pieceX = 4;
            pieceY = 0;
        }
    }

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (board[y][x] != CRGB::Black) DrawPixel(x, y, board[y][x]);
        }
    }

    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            if (currentPiece[y][x] == 1) DrawPixel(pieceX + x, pieceY + y, pieceColor);
        }
    }
}