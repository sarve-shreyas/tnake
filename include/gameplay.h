#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "objectspace.h"
typedef struct {
    char* username;
    char* time;
    int score;
} game;

void updateScore(int score, game* game);
int initGameplay(const char* username, game* game);
void processKeyPressedAction(struct objectspace* space);
void moveSnake(struct objectspace* space);
#endif  // GAMEPLAY_H