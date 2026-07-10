#ifndef SCREEN_GAMEPLAY_H
#define SCREEN_GAMEPLAY_H

#include "raylib.h"
#include <stdbool.h>

#define GAME_AREA_X 160
#define GAME_AREA_WIDTH 480
#define GAME_AREA_RIGHT 560
#define MAX_BULLETS 50
#define MAX_ENEMY_BULLETS 100
#define MAX_BLOCKS 100
#define MAX_PARTICLES 200

typedef enum {
    MODE_RED = 0,
    MODE_BLUE,
    MODE_PURPLE
} FireMode;

typedef enum {
    ENEMY_SHOT_SINGLE = 0,
    ENEMY_SHOT_TRIPLE,
    ENEMY_SHOT_CIRCLE,
    ENEMY_SHOT_SPREAD
} EnemyShotPattern;

typedef struct {
    Vector2 position;
    float size;
    float speed;
    FireMode fireMode;
    Color shipColor;
    float hitboxRadius;
    bool showHitbox;
} Player;

typedef struct {
    Vector2 position;
    Vector2 size;
    float speed;
    Color color;
    FireMode mode;
    bool active;
} Bullet;

typedef struct {
    Vector2 position;
    float size;
    Color color;
    FireMode requiredMode;
    bool active;
    int health;
    EnemyShotPattern pattern;
    bool hasBeenHit;
    bool canShootBack;
    int shootCooldown;
    int shootTimer;
    FireMode bulletMode;
    int flashTimer;
} Block;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float speed;
    float radius;
    FireMode mode;
    bool active; 
} EnemyBullet;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    int life; // frames duration
    float size;
    bool active;
} Particle;

void Gameplay_Init(void);
bool Gameplay_Update(void);
void Gameplay_Draw(void);

void SpawnPatternLine(float, const char* pattern, float offsetX);
void SpawnWallWithOffset(int numLines, const char* basePattern,
                         float startY, float offestPerLine); //TODO
void SpawnEnemyBullet(float x, float y, Color, FireMode mode);
void SpawnParticle(Vector2 pos, Vector2 vel, Color col, int life, float size);
void SpawnExplosion(float x, float y, Color color);
void SpawnTrail(float x, float y, Color color);
bool CheckCollisionCricleRec(Vector2 circlePos, float circleRadius,
                              Rectangle rec);

#endif