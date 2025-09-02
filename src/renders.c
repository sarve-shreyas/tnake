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

#define SNAKE_INIT_LEN 20
#define GAMEBOARD_INIT_WIDTH 40
#define GAMEBOARD_INIT_HEIGHT 32

objectspaceconfigs configs = {.gameboard_height = GAMEBOARD_INIT_HEIGHT, .gameboard_width = GAMEBOARD_INIT_WIDTH, .snake_init_len = SNAKE_INIT_LEN};

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
            if (saveGamescore() == 0) {
                promptUser("Score saved success !!", 0);
            } else {
                promptUser("Some error in saving score", 0);
            }
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
    if (initObjectSpace(configs) != 0) die("initObjectspace");
    if (initGameplay() != 0) die("initGameplay");
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
    deleteObjectSpace();
    return scorescreen();
}

char** formatGame(game g, int* len) {
    char time[256];
    stringAppend(2, time, "Time - ", g.time);
    char username[256];
    stringAppend(2, username, "User - ", g.username);
    char scoe[256];
    stringAppend(2, scoe, "Score - ", "22");
    char gamestate[256];
    char* state = g.gamestate == GAMEPLAY_WINNER ? "WON" : "GAMEOVER";
    stringAppend(2, gamestate, "Gamestate", state);

    *len = 4;
    char** megs = malloc(*len * sizeof(char*));
    megs[0] = strdup(time);
    megs[1] = strdup(username);
    megs[2] = strdup(scoe);
    megs[3] = strdup(gamestate);
    return megs;
}
int scoreboardscreen() {
    clearScreenAB();
    int load_len = 5;
    game* games = loadGamescores(&load_len);
    int total_len = 0;
    char** megs = NULL;
    for (int i = 0; i < load_len; i++) {
        int form_len;
        char** formattedScore = formatGame(games[i], &form_len);
        megs = realloc(megs, (total_len + form_len + 2) * sizeof(char*));
        for (int j = 0; j < form_len; j++) {
            megs[total_len++] = formattedScore[j];
        }
        megs[total_len++] = "";
        megs[total_len++] = "---";
        free(formattedScore);
    }
    if (games) free(games);
    return screenPromptMessage(total_len, megs, "Press any key to go back", "Scoreboard");
}

int initRender() {
    changeCusrsorState(DEAD);
    clearScreenAB();
    return mainmenuscreen();
}