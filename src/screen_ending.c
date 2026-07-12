#include "screen_ending.h"
#include "raylib.h"
#include "screen_gameplay.h"
#include <stdio.h>

void Ending_Init(void){}
void Ending_Update(void) {}
void Ending_Draw(void) {
    DrawText("GAME OVER",
             GetScreenWidth() / 2 - MeasureText("GAME OVER", 80)/2,
             100, 80, BLACK);

    // Score
    char finalScore[32];
    sprintf(finalScore, "SCORE : %d", GetScore());
    DrawText(finalScore,
             120,
             300, 60, PURPLE);

    // Restart
    if ((GetTime() * 2) - (int)(GetTime() * 2) < 0.5f) {
        DrawText("PRESS SPACE TO RESTART",
                120,
                370, 30, BLUE);
    }
}

bool Ending_ShouldStartGame(void) {
    return IsKeyPressed(KEY_SPACE);
}