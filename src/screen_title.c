#include "screen_title.h"
#include "raylib.h"

void Title_Init(void) {}

void Title_Update(void) {}

void Title_Draw(void) {
    DrawText("HEX-SHOOT",
        GetScreenWidth()/2 - MeasureText("HEX-SHOOT", 80)/2,
        GetScreenHeight()/2 - 80, 80, PURPLE);
    if ((GetTime() * 2) - (int)(GetTime() * 2) < 0.5f) {
        DrawText("PRESS SPACE",
                GetScreenWidth()/2 - MeasureText("PRESS SPACE", 40)/2,
                GetScreenHeight()/2 + 80, 40, BLUE);
    }
}

bool Title_ShouldStartGame(void) {
    return IsKeyPressed(KEY_SPACE);
}