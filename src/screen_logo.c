#include "screen_logo.h"
#include "raylib.h"

static int LogoFrameCounter = 0;
static const int LOGO_DURATION = 60;

void Logo_Init(void) {
    LogoFrameCounter = 0;
}

void Logo_Update(void) {
    LogoFrameCounter ++;
}

void Logo_Draw(void) {
    DrawRectangle(70, 90, 200, 200, BLACK);
    DrawRectangle(70 + 16, 90 + 16, 200 - 32, 200 - 32, RAYWHITE);
    DrawText("raylib", 70 + 200 - MeasureText("raylib", 40) - 32, 
            90 + 200 - 40 - 24, 40, BLACK);
    DrawText("6.x", 290, 90 - 26, 280, BLACK);
    DrawText("GAMEJAM", 70, 90 + 210, 120, MAROON);
}

bool Logo_IsFinished(void){
    return LogoFrameCounter >= LOGO_DURATION;
}

void Logo_Reset(void) {
    LogoFrameCounter = 0;
}