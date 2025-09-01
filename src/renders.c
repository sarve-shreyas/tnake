#include "renders.h"

#include <string.h>

#include "abuffer.h"
#include "gameplay.h"
#include "gmenu.h"
#include "logger.h"
#include "message.h"
#include "objectspace.h"
#include "printter.h"
#include "screen.h"
#include "utils.h"

void clearScreenAB() {
    struct abuf ab = ABUF_INIT;
    clearScreen(&ab);
    abFlush(&ab);
}

int mainmenuscreen() {
    clearScreenAB();
    char* menuItems[] = {"New Game", "Scoreboard", "Quit"};
    char* menuTitle = "Main Menu";
    menudata m = {.len = 3, .options = menuItems, .title = menuTitle};
    return drawmenu(m);
}

int howtoplayscreen() {
    clearScreenAB();
    char* megs[] = {
        "Arrow Up/W - Direction to upward",
        "Arrow Down/S - Direction to downward",
        "Arrow Left/D - Direction to leftward",
        "Arrow Right/A - Direction to rightward",
        "",
        "Space - pause gameplay",
        "CTRL + C / CTRL + Q - Quit game"};
    int lem = 7;
    screenPromptMessage(lem, megs);
    return 0;
}

int scorescreen() {
    game g;
    getGameplay(&g);
    char* time[256];
    stringAppend(2, time, "Time - ", g.time);
    char* username[256];
    stringAppend(2, username, "User - ", g.username);
    char* scoe[256];
    stringAppend(2, scoe, "Score - ", "22");
    int len = 3;
    char* megs[] = {time, username, scoe};
    screenPromptMessage(len, megs);
}

int gameplayscreen() {
    clearScreenAB();
    initGameplay();
    while (objspace.sn->len) {
        moveSnake(&objspace);
        refreshGameScreen();
    }
    refreshGameScreen();
    scorescreen();
    return 0;
}

int scoreboardscreen() {
    clearScreenAB();
    char* megs[] = {"Score - 20"};
    int len = 1;
    screenPromptMessage(len, megs);
}
