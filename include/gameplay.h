#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "objectspace.h"
typedef struct {
    char* username;
    char* time;
    int score;
    int gamestate;
} game;

enum Gameplay_State {
    GAMEPLAY_PLAYING,
    GAMEPLAY_PAUSED,
    GAMEPLAY_WINNER,
    GAMEPLAY_GAMEOVER,
    GAMEPLAY_NONE
};

int initGameplay();
int getGameplay(game* g);
void progressGameplay();
int saveGamescore();
game* loadGamescores(int *load_len);
#endif  // GAMEPLAY_H