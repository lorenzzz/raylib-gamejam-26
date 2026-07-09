#include "screen_gameplay.h"
#include "raylib.h"

static Player player;

void Gameplay_Init(void) {
    player.position = (Vector2){360, 600};
    player.size = 20.0f;
    player.speed = 5.0f;
}

void Gameplay_Update(void) {
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
}

void Gameplay_Draw(void) {
    Vector2 v1 = {player.position.x, player.position.y - player.size}; // top
    Vector2 v2 = {player.position.x - player.size/2, player.position.y + player.size}; // left
    Vector2 v3 = {player.position.x + player.size/2, player.position.y + player.size}; // right

    DrawTriangle(v1, v2, v3, RED);

    // Game zone limit
    DrawLine(GAME_AREA_X, 0, GAME_AREA_X, GetScreenHeight(), GRAY);
    DrawLine(GAME_AREA_RIGHT, 0, GAME_AREA_RIGHT, GetScreenHeight(), GRAY);
}