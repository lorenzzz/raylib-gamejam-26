#ifndef SCREEN_TITLE_H
#define SCREEN_TITLE_H
#include <stdbool.h>

void Title_Init(void);
void Title_Update(void);
void Title_Draw(void);
bool Title_ShouldStartGame(void);

#endif