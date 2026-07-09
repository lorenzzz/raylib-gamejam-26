#ifndef SCREEN_GAMEPLAY_H
#define SCREEN_GAMEPLAY_H

#include "raylib.h"
#include <stdbool.h>

#define GAME_AREA_X 160
#define GAME_AREA_WIDTH 480
#define GAME_AREA_RIGHT 560

typedef enum {
    MODE_RED = 0,
    MODE_BLUE,
    MODE_PURPLE
} FireMode;

typedef struct {
    Vector2 position;
    float size;
    float speed;
    FireMode mode;
    Color color;
} Player;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    bool active;
} Bullet;

void Gameplay_Init(void);
void Gameplay_Update(void);
void Gameplay_Draw(void);

#endif