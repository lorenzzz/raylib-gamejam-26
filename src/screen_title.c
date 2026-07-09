#include "screen_title.h"
#include "raylib.h"

void Title_Init(void) {}

void Title_Update(void) {}

void Title_Draw(void) {
    if ((GetTime() * 2) - (int)(GetTime() * 2) < 0.5f) {
        DrawText("PRESS SPACE",
                GetScreenWidth()/2 - MeasureText("PRESS SPACE", 40)/2,
                GetScreenHeight()/2, 40, DARKGRAY);
    }
}

bool Title_ShouldStartGame(void) {
    return IsKeyPressed(KEY_SPACE);
}