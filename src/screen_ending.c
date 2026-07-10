#include "screen_ending.h"
#include "raylib.h"

void Ending_Init(void){}
void Ending_Update(void) {}
void Ending_Draw(void) {
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - MeasureText("GAME OVER", 40)/2,
             100, 40, DARKGRAY);
}

bool Ending_ShouldStartGame(void) {
    return IsKeyPressed(KEY_SPACE);
}