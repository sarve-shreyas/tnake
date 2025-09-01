#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "objectspace.h"
typedef struct {
    char* username;
    char* time;
    int score;
    int gamestate;
} game;

void updateScore(int score);
int initGameplay();
int getGameplay(game* g);
void processKeyPressedAction(struct objectspace* space);
void moveSnake(struct objectspace* space);
#endif  // GAMEPLAY_H