#include "screen_ending.h"
#include "raylib.h"
#include "screen_gameplay.h"
#include <stdio.h>

void Ending_Init(void){}
void Ending_Update(void) {}
void Ending_Draw(void) {
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - MeasureText("GAME OVER", 40)/2,
             100, 40, DARKGRAY);

    // Score
    char finalScore[32];
    sprintf(finalScore, "SCORE : %d", GetScore());
    DrawText(finalScore,
             GetScreenWidth()/2 - MeasureText(finalScore, 30)/2,
             300, 30, BLACK);

    // Restart
    if ((GetTime() * 2) - (int)(GetTime() * 2) < 0.5f) {
        DrawText("PRESS SPACE TO RESTART",
                GetScreenWidth()/2 - MeasureText("PRESS SPACE TO RESTART", 30)/2,
                400, 30, DARKGRAY);
    }
}

bool Ending_ShouldStartGame(void) {
    return IsKeyPressed(KEY_SPACE);
}