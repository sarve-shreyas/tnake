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
    struct fruit* ft = malloc(sizeof(struct fruit));
    if (getWindowSize(&termi.row, &termi.col) == -1) die("getWindowSize");
    if (configureSnake(SNAKE_INIT_LEN, sn) != 0) die("configgureSnake");
    if (configureGameBoard(GAMEBOARD_INIT_HEIGHT, GAMEBOARD_INIT_WIDTH, &board, termi) != 0) die("defineGameBoard");
    objspace = (struct objectspace){.board = board, .sn = sn};
    if (initFruit(&objspace) != 0) die("initFruit");
    abAppend(&ab, "\x1b[2J", 4);
    abAppend(&ab, "\x1b[?25l", 6);
    abFlush(&ab);
}

void moveSnake(struct objectspace* space) {
    switch (space->sn->state) {
        case ALIVE:
            processKeyPressedAction(space);
            moveBodyParts(space->sn);
            updatePositionWithDirection(space->sn);
            updateSnakeState(space->sn);
            break;
        case DEAD:
            removeSnakeSegment(space->sn);
            break;
        default:
            die("moveSnake");
    }
}
void snakeDie(struct snake* sn) {
    removeSnakeSegment(sn);
}
void progressGamePlay() {
    struct abuf ab = ABUF_INIT;
    printObjectSpace(&ab, termi, objspace);
    abFlush(&ab);
}
int main() {
    init();

    while (objspace.sn->len) {
        progressGamePlay();
        moveSnake(&objspace);
    }
    progressGamePlay();
    pexit(0);
    return 0;
}
