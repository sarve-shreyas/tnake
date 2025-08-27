#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "abuffer.h"
#include "ansi.h"
#include "fruit.h"
#include "gameboard.h"
#include "gameplay.h"
#include "logger.h"
#include "objectspace.h"
#include "snake.h"
#include "space.h"
#include "terminal.h"
#include "translate.h"
#include "utils.h"

struct terminal termi;
struct objectspace objspace;

#define SNAKE_INIT_LEN 20
#define GAMEBOARD_INIT_WIDTH 40
#define GAMEBOARD_INIT_HEIGHT 32

void init(void) {
    enableRawMode();
    struct snake* sn = malloc(sizeof(struct snake));
    struct gameboard board;
    struct abuf ab = ABUF_INIT;
    if (getWindowSize(&termi.row, &termi.col) == -1) die("getWindowSize");
    if (configureSnake(SNAKE_INIT_LEN, sn) != 0) die("configgureSnake");
    if (configureGameBoard(GAMEBOARD_INIT_HEIGHT, GAMEBOARD_INIT_WIDTH, &board, termi) != 0) die("defineGameBoard");
    objspace = (struct objectspace){.board = board, .sn = sn};
    if (initFruit(&objspace) != 0) die("initFruit");
    abAppend(&ab, "\x1b[2J", 4);
    abAppend(&ab, "\x1b[?25l", 6);
    abFlush(&ab);
}

void progressGamePlay() {
    struct abuf ab = ABUF_INIT;
    printObjectSpace(&ab, termi, objspace);
    abFlush(&ab);
}
int main() {
    init();
    while (objspace.sn->len) {
        moveSnake(&objspace);
        progressGamePlay();
    }
    progressGamePlay();
    pexit(0);
    // printf("score %s, len %d", objspace.scoreboard->style.str, objspace.scoreboard->style.len);
    return 0;
}
