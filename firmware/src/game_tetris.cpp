#include "engine.h"

static CRGB board[10][10];

static bool gameActive = false;

static int pieceX = 4;
static int pieceY = 0;
static CRGB pieceColor = CRGB::Cyan;

static int currentPiece[4][4];

const int TETROMINOES[7][4][4] = {
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},

    {{1,0,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},

    {{0,0,1,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    
    {{0,1,1,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},

    {{0,1,1,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},

    {{0,1,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},

    {{1,1,0,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}
};

void SpawnNewPiece() {
    int shapeID = random(7);

    for (int y=0; y<4; y++) {
        for (int x=0; x<4; x++) {
            currentPiece[y][x] = TETROMINOES[shapeID][y][x];
        }
    }

    if (shapeID == 0) pieceColor = CRGB::Cyan;
    if (shapeID == 1) pieceColor = CRGB::Blue;
    if (shapeID == 2) pieceColor = CRGB::Orange;
    if (shapeID == 3) pieceColor = CRGB::Yellow;
    if (shapeID == 4) pieceColor = CRGB::Green;
    if (shapeID == 5) pieceColor = CRGB::Purple;
    if (shapeID == 6) pieceColor = CRGB::Red;

    pieceX = 3;
    pieceY = 0;
}

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
bool CheckCollision(int nextX, int nextY, int piece[4][4]) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (piece[y][x] == 1) {
                int boardX = nextX + x;
                int boardY = nextY + y;

                if (boardX < 0 || boardX > 9 || boardY > 9) return true; 
                if (boardY >=0) {
                    if (board[boardY][boardX] != CRGB::Black) return true;
                }
            }
        }
    }
    return false;
}

void RotatePiece() {
    int temp[4][4];

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp[x][3 - y] = currentPiece[y][x];
        }
    }

    if (!CheckCollision(pieceX, pieceY, temp)) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                currentPiece[y][x] = temp[y][x];
            }
        }
    }
}

void ClearLines() {
    for (int y = 9; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < 10; x++) {
            if (board[y][x] == CRGB::Black) {
                full = false;
                break;
            }
        }

        if (full) {
            for (int shiftY = y; shiftY > 0; shiftY--) {
                for (int x = 0; x < 10; x++) {
                    board[shiftY][x] = board[shiftY - 1][x];
                }
            }
            for (int x = 0; x < 10; x++) {
                board[0][x] = CRGB::Black;
            }
            y++; 
        }
    }
}

void RunTetris(int input) {
    static unsigned long lastGravity = 0;

    if (!gameActive) {
        if (input == PIN_BTN_A) ResetTetris();
        return;
    }

    if (input == PIN_JOY_LEFT) {
        if (!CheckCollision(pieceX - 1, pieceY, currentPiece)) pieceX--;
    }
    if (input == PIN_JOY_RIGHT) {
        if (!CheckCollision(pieceX + 1, pieceY, currentPiece)) pieceX++;
    }
    if (input == PIN_JOY_DOWN) {
        if (!CheckCollision(pieceX, pieceY + 1, currentPiece)) pieceY++;
    }
    if (input == PIN_BTN_A) {
        RotatePiece();
    }


    if (millis() - lastGravity > 500) {
        lastGravity = millis();

        if (!CheckCollision(pieceX, pieceY + 1, currentPiece)) {
            pieceY++;
        } else {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    if (currentPiece[y][x] == 1) {
                        if (pieceY + y >= 0) {
                            board[pieceY + y][pieceX + x] = pieceColor;
                        }
                    }
                }
            }
            ClearLines();
            SpawnNewPiece();
        }
    }

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (board[y][x] != CRGB::Black) DrawPixel(x, y, board[y][x]);
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece[y][x] == 1) DrawPixel(pieceX + x, pieceY + y, pieceColor);
        }
    }
}
