#include "screen_gameplay.h"
#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

static Player player;
static int shootCooldown = 0;
static Bullet bullets[50];
static int purpleLock = 0;
static Block blocks[MAX_BLOCKS];
static EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];
static float wallSpeed = 2.0f;

void ShootBullet(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = (Vector2){
                player.position.x,
                player.position.y - player.size
            };
            bullets[i].size = (Vector2){10, 30};
            bullets[i].speed = 15.0f;
            bullets[i].color = player.shipColor;
            bullets[i].mode = player.fireMode;
            bullets[i].active = true;
            break;
        }
    }
}

void SpawnPatternLine(float y, const char* pattern, float offsetX) {
    float blockSize = 40;
    float spacing = 10;
    float startX = GAME_AREA_X + 20 + offsetX;

    int patternLength = strlen(pattern);
    for (int i = 0; i < patternLength; i++) {
        for (int j = 0; j < MAX_BLOCKS; j++){
            if (!blocks[j].active) {
                // Position
                blocks[j].position.x = startX + i * (blockSize + spacing);
                blocks[j].position.y = y;
                blocks[j].size = blockSize;
                blocks[j].active = true;
                blocks[j].health = 3;

                // Color
                switch(pattern[i]) {
                    case 'R':
                        blocks[j].color = RED;
                        blocks[j].requiredMode = MODE_RED;
                        break;
                    case 'B':
                        blocks[j]. color = BLUE;
                        blocks[j].requiredMode = MODE_BLUE;
                        break;
                    case 'P':
                        blocks[j].color = PURPLE;
                        blocks[j].requiredMode = MODE_PURPLE;
                }
                break; // next pattern char
            }
        }
    }
}
/*
bool CheckCollisionCircleRec(Vector2 circlePos, float circleRadius,
                             Rectangle rec) {
    // Find nearest point of the circle on the rectangle
    float closestX = fmaxf(rec.x, fminf(circlePos.x, rec.x + rec.width));
    float closestY = fmaxf(rec.y, fminf(circlePos.y, rec.y + rec.height));

    // Distance between this point and the circle center
    float dx = circlePos.x - closestX;
    float dy = circlePos.y - closestY;

    return (dx*dx + dy*dy) < (circleRadius * circleRadius);
}
*/
void Gameplay_Init(void) {
    player.position = (Vector2){360, 600};
    player.size = 20.0f;
    player.speed = 5.0f;
    player.fireMode = MODE_RED;
    player.shipColor = RED;
    player.hitboxRadius = 10.0f;
    player.showHitbox = true;

    SpawnPatternLine(-50, "RRRRR", 0);
    SpawnPatternLine(-150, "BBBBB", 0);
}

bool Gameplay_Update(void) {
    if(IsKeyDown(KEY_LEFT)) player.position.x -= player.speed;
    if(IsKeyDown(KEY_RIGHT)) player.position.x += player.speed;
    if(IsKeyDown(KEY_UP)) player.position.y -= player.speed;
    if(IsKeyDown(KEY_DOWN)) player.position.y += player.speed;

    if(player.position.x < GAME_AREA_X + player.size)
       player.position.x = GAME_AREA_X + player.size;
    if(player.position.x > GAME_AREA_RIGHT - player.size)
       player.position.x = GAME_AREA_RIGHT - player.size;
    if(player.position.y < player.size)
       player.position.y = player.size;
    if(player.position.y > GetScreenHeight() - player.size)
       player.position.y = GetScreenHeight() - player.size;

    bool xPressed = IsKeyDown(KEY_X);
    bool cPressed = IsKeyDown(KEY_C);

    if (xPressed && cPressed) {
        player.fireMode = MODE_PURPLE;
        player.shipColor = PURPLE;
        purpleLock = 10;
    } 
    else if (purpleLock > 0) {
        purpleLock--;
    }
    else if (xPressed) {
        player.fireMode = MODE_RED;
        player.shipColor = RED;
    }
    else if (cPressed) {
        player.fireMode = MODE_BLUE;
        player.shipColor = BLUE;
    }

    if(IsKeyDown(KEY_SPACE) && shootCooldown <= 0) {
        ShootBullet();
        shootCooldown = 8;
    }

    if (shootCooldown > 0) {
        shootCooldown --;
    }

    // Player's bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active){
            bullets[i].position.y -= bullets[i].speed;
            if (bullets[i].position.y < -50) {
                bullets[i].active = false;
            }
        }
    }

    // Enemy's bullets
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBullets[i].active){
            float dx = player.position.x - enemyBullets[i].position.x;
            float dy = player.position.y - enemyBullets[i].position.y;
            float distance = sqrt(dx*dx +dy*dy);

            if (distance < player.hitboxRadius + enemyBullets[i].radius) {
                return false;
            }
        }
    } 

    // Wall blocks
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].active) {
            blocks[i].position.y += wallSpeed;

            Rectangle blockRec = {
                blocks[i].position.x,
                blocks[i].position.y,
                blocks[i].size,
                blocks[i].size
            };

            if(CheckCollisionCircleRec(player.position, player.hitboxRadius, blockRec)) {
                return false;
            }

            if(blocks[i].position.y > 720) {
                blocks[i].active = false;
            }
        }
    }

    return true;
}

void Gameplay_Draw(void) {
    // Player Ship
    Vector2 v1 = {player.position.x, player.position.y - player.size}; // top
    Vector2 v2 = {player.position.x - player.size/2, player.position.y + player.size}; // left
    Vector2 v3 = {player.position.x + player.size/2, player.position.y + player.size}; // right

    DrawTriangle(v1, v2, v3, player.shipColor);

    // Player Hitbox
    if (player.showHitbox) {
        DrawCircle(player.position.x, player.position.y,
                   player.hitboxRadius,
                   GREEN);
                   //(Color){0, 255, 0, 128});
    }

    // Game zone limit
    DrawLine(GAME_AREA_X, 0, GAME_AREA_X, GetScreenHeight(), GRAY);
    DrawLine(GAME_AREA_RIGHT, 0, GAME_AREA_RIGHT, GetScreenHeight(), GRAY);

    // Player Bullets
    for (int i =0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            DrawRectangle (
                bullets[i].position.x - bullets[i].size.x/2,
                bullets[i].position.y,
                bullets[i].size.x,
                bullets[i].size.y,
                bullets[i].color
            );
        }
    }

    // Wall blocks
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].active) {
            DrawRectangle(
                blocks[i].position.x,
                blocks[i].position.y,
                blocks[i].size,
                blocks[i].size,
                blocks[i].color
            );
        }
    }
}
