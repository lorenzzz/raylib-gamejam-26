#include "screen_gameplay.h"
#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

static int startBuffer = 0;
static Player player;
static int shootCooldown = 0;
static Bullet bullets[50];
static int purpleLock = 0;
static Block blocks[MAX_BLOCKS];
static EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];
static float wallSpeed = 2.0f;
static Particle particles[MAX_PARTICLES];

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

void SpawnEnemyBullet(float x, float y, Color color, FireMode mode) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if(!enemyBullets[i].active) {
            enemyBullets[i].position= (Vector2){x, y};
            enemyBullets[i].color = color;
            enemyBullets[i].mode = mode;
            enemyBullets[i].radius = 8;
            enemyBullets[i].active = true;

            // Calculate direction to the player
            float dx = player.position.x - x;
            float dy = player.position.y - y;
            float length = sqrt(dx*dx + dy*dy);

            if (length > 0) {
                enemyBullets[i].velocity.x = (dx / length) * 4.0f; // 4px speed
                enemyBullets[i].velocity.y = (dy / length) * 4.0f;
            }
            //printf("Spawn: mode=%d", mode);
            break;
        }
    }
}

void SpawnParticle(Vector2 pos, Vector2 vel, Color col, int life, float size) {
    for (int i = 0; i < MAX_PARTICLES; i++){
        if (!particles[i].active) {
            particles[i].position = pos;
            particles[i].velocity = vel;
            particles[i].color = col;
            particles[i].life = life;
            particles[i].size = size;
            particles[i].active = true;
            break;
        }
    }
}

void SpawnExplosion(float x, float y, Color color){
    int numParticles = 12;
    for(int i = 0; i < numParticles; i ++){
        float angle = (i * 360.0f / numParticles) * DEG2RAD;
        Vector2 vel = {
            cos(angle) * GetRandomValue(2, 5),
            cos(angle) * GetRandomValue(2, 5)
        };
        SpawnParticle((Vector2){x, y}, vel, color, 30, 12.0f);
    }
}

void SpawnTrail(float x, float y, Color color) {
    Vector2 vel = {
        GetRandomValue(-1, 1),
        GetRandomValue(1, 3)
    };
    SpawnParticle((Vector2){x, y}, vel, color, 15, 8.0f);
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

    startBuffer = 60;

    // player position
    player.position = (Vector2){360, 600};
    player.size = 20.0f;
    player.speed = 5.0f;
    player.fireMode = MODE_RED;
    player.shipColor = RED;
    player.hitboxRadius = 10.0f;
    player.showHitbox = true;

    // Deactivate player bullet
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
    shootCooldown = 0;

    // Deactivate wall blocks
    for (int i=0; i < MAX_BLOCKS; i ++){
        blocks[i].active = false;
        blocks[i].hasBeenHit = false;
        blocks[i].canShootBack = false;
        blocks[i].health = 3;
        blocks[i].shootTimer = 0;
    }

    // Deactivate enemy bullets
    for (int i = 0; i < MAX_ENEMY_BULLETS; i ++) {
        enemyBullets[i].active = false;
    }

    purpleLock = 0;

    for (int i=0; i < MAX_PARTICLES; i++){
        particles[i].active = false;
    }

    SpawnPatternLine(-50, "RRRRR", 0);
    SpawnPatternLine(-150, "BBBBB", 0);
}

bool Gameplay_Update(void) {
    if (startBuffer > 0) {
        startBuffer --;
        return true;
    }

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
            Rectangle bulletRec = {
                bullets[i].position.x - bullets[i].size.x/2,
                bullets[i].position.y,
                bullets[i].size.x,
                bullets[i].size.y
            };

            // Block collision
            for (int b = 0; b < MAX_BLOCKS; b++){
                if (blocks[b].active) {
                    Rectangle blockRec = {
                        blocks[b].position.x,
                        blocks[b].position.y,
                        blocks[b].size,
                        blocks[b].size
                    };

                    if (CheckCollisionRecs(bulletRec, blockRec)) {
                        if (blocks[b].hasBeenHit) {
                                if(bullets[i].mode !=  blocks[b].requiredMode) {
                                    blocks[b].health--;
                                    blocks[b].flashTimer = 5;
                                    if (blocks[b].health <= 0) {
                                        blocks[b].active = false;
                                    }
                                }
                        }
                        else {
                            if (bullets[i].mode == blocks[b].requiredMode) {
                                blocks[b].health --;
                                if (blocks[b].health <= 0) {
                                    SpawnExplosion(
                                        blocks[b].position.x + blocks[b].size/2,
                                        blocks[b].position.y + blocks[b].size/2,
                                        blocks[b].color
                                    );
                                    blocks[b].active = false;
                                }
                            }
                            else {
                                blocks[b].hasBeenHit = true;
                                blocks[b].canShootBack = true;
                                blocks[b].health = 10;
                                blocks[b].pattern = GetRandomValue(0, 3);
                                blocks[b].bulletMode = blocks[b].requiredMode;
                                blocks[b].shootCooldown = 60;
                                blocks[b].shootTimer = 0;
                                //blocks[b].bulletColor = GetRandomFireMode();

                                // Instant shot
                                SpawnEnemyBullet(
                                    blocks[b].position.x + blocks[b].size/2,
                                    blocks[b].position.y + blocks[b].size/2,
                                    blocks[b].color,
                                    blocks[b].bulletMode
                                );
                            }


                            blocks[b].health--;
                        }

                        bullets[i].active = false;
                    }
                }
            }

            if (bullets[i].position.y < -50) {
                bullets[i].active = false;
            }
        }
    }

    // Enemy's bullets
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemyBullets[i].active){
            enemyBullets[i].position.x += enemyBullets[i].velocity.x;
            enemyBullets[i].position.y += enemyBullets[i].velocity.y;


            // collision
            float dx = player.position.x - enemyBullets[i].position.x;
            float dy = player.position.y - enemyBullets[i].position.y;
            float distance = sqrt(dx*dx +dy*dy);

            if (distance < player.hitboxRadius + enemyBullets[i].radius) {
                /*
                printf("Collision: bullet=%d, player=%d, match=%d\n",
                        enemyBullets[i].mode, player.fireMode,
                        enemyBullets[i].mode == player.fireMode);
                */
                if (enemyBullets[i].mode == player.fireMode){
                    enemyBullets[i].active = false;
                } else {
                    return false;
                }
            }

            if (enemyBullets[i].position.x > 720 ||
                enemyBullets[i].position.y > 720 ||
                enemyBullets[i].position.x < 0) {
                enemyBullets[i].active = false;
            }



        }
    } 

    // Wall blocks
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].active) {
            blocks[i].position.y += wallSpeed;

            // Bullets timer
            if (blocks[i].active && blocks[i].canShootBack) {
                blocks[i].shootTimer ++;
                if (blocks[i].shootTimer >= 60) {
                    SpawnEnemyBullet(
                        blocks[i].position.x + blocks[i].size/2,
                        blocks[i].position.y + blocks[i].size / 2,
                        blocks[i].color,
                        blocks[i].bulletMode
                    );
                    blocks[i].shootTimer = 0;
                }
            }

            // Flash timer
            if (blocks[i].flashTimer > 0) {
                blocks[i].flashTimer --;
            }

            // Collisions
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

    // Particles
    for (int i=0; i < MAX_PARTICLES; i ++) {
        if (particles[i].active) {
            particles[i].position.x += particles[i].velocity.x;
            particles[i].position.y += particles[i].velocity.y;
            particles[i].life--;
            particles[i].size *= 0.95f;

            if (particles[i].life <= 0) {
                particles[i].active = false;
            }
        }
    }

    SpawnTrail(player.position.x, player.position.y, player.shipColor);

    return true;
}

void Gameplay_Draw(void) {

    // Go startup
    if (startBuffer > 30) {
        DrawText("GO!", 
                  GetScreenWidth()/2 - MeasureText("GO", 60)/2,
                  GetScreenHeight()/2, 60, DARKGRAY);
    }

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

    // Enemy Block bullets
    for (int i = 0; i < MAX_ENEMY_BULLETS; i ++) {
        if(enemyBullets[i].active) {
            Color c = enemyBullets[i].color;

            DrawCircle(enemyBullets[i].position.x, enemyBullets[i].position.y,
                       enemyBullets[i].radius + 6, (Color){c.r, c.g, c.b, 50});
            DrawCircle(enemyBullets[i].position.x, enemyBullets[i].position.y,
                       enemyBullets[i].radius + 3, (Color){c.r, c.g, c.b, 128});
            DrawCircle(enemyBullets[i].position.x, enemyBullets[i].position.y,
                       enemyBullets[i].radius, c);
        }
    }

    // Wall blocks
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].active) {
            Color drawColor = (blocks[i].flashTimer > 0) ? BLACK : blocks[i].color;
            DrawRectangle(
                blocks[i].position.x,
                blocks[i].position.y,
                blocks[i].size,
                blocks[i].size,
                drawColor
            );
            if (blocks[i].canShootBack) {
                DrawRectangleLines(blocks[i].position.x, blocks[i].position.y,
                                   blocks[i].size, blocks[i].size, BLACK);
                DrawRectangleLines(blocks[i].position.x - 1, blocks[i].position.y - 1,
                                   blocks[i].size+2, blocks[i].size+2, BLACK);
            }
        }
    }

    // Particules
    for (int i = 0; i< MAX_PARTICLES; i ++) {
        if (particles[i].active) {
            DrawCircle(
                particles[i].position.x,
                particles[i].position.y,
                particles[i].size,
                particles[i].color
            );
        }
    }
}
