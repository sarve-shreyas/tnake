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
    int ret = drawmenu(m);
    switch (ret) {
        case 0: {
            return gameplayscreen();
        }
        case 1: {
            return scoreboardscreen();
        }
        case 2: {
            pexit(0);
            break;
        }
        default:
            die("Invalid option selected");
    }
    return -1;
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
    char* title = "How to play ?";
    return screenPromptMessage(lem, megs, NULL, title);
}

int scorescreen() {
    game g;
    getGameplay(&g);
    char time[256];
    stringAppend(2, time, "Time - ", g.time);
    char username[256];
    stringAppend(2, username, "User - ", g.username);
    char scoe[256];
    stringAppend(2, scoe, "Score - ", "22");
    int len = 7;
    char* title = "Game Score";
    char* footer = "Press any other key to exit";
    char* megs[] = {time, username, scoe, "", "S - save score", "M - Main Menu", "Q - Quit"};
    int key = screenPromptMessage(len, megs, footer, title);
    switch (key) {
        case 's':
        case 'S':
            // save score
            break;
        case 'M':
        case 'm':
            return mainmenuscreen();
        default:
            pexit(0);
            break;
    }
    return -1;
}

int gameplayscreen() {
    howtoplayscreen();
    clearScreenAB();
    initGameplay();
    int keep_running = ALIVE;
    while (keep_running == ALIVE) {
        game gameplay;
        if (getGameplay(&gameplay) != 0) die("getGameplay");
        switch (gameplay.gamestate) {
            case GAMEPLAY_PLAYING:
                progressGameplay();
                break;
            case GAMEPLAY_PAUSED:
                progressGameplay();
                break;
            case GAMEPLAY_WINNER:
            case GAMEPLAY_GAMEOVER:
                keep_running = DEAD;
                break;
            default:
                die("invalidGamestate");
        }
        refreshGameScreen();
    }
    refreshGameScreen();
    return scorescreen();
}

int scoreboardscreen() {
    clearScreenAB();
    char* megs[] = {"Score - 20"};
    int len = 1;
    return screenPromptMessage(len, megs, NULL, NULL);
}
