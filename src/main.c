#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "abuffer.h"
#include "ansi.h"
#include "gameboard.h"
#include "objectspace.h"
#include "snake.h"
#include "space.h"
#include "terminal.h"
#include "translate.h"
#include "utils.h"

struct terminal termi;
struct objectspace objspace;

#define SNAKE_INIT_LEN 5
#define GAMEBOARD_INIT_WIDTH 5
#define GAMEBOARD_INIT_HEIGHT 4

void init(void) {
    struct snake* sn = malloc(sizeof(struct snake));
    struct gameboard board;
    struct abuf ab = ABUF_INIT;
    if (getWindowSize(&termi.row, &termi.col) == -1) die("getWindowSize");
    if (configureSnake(SNAKE_INIT_LEN, sn) != 0) die("configgureSnake");
    if (configureGameBoard(GAMEBOARD_INIT_HEIGHT, GAMEBOARD_INIT_WIDTH, &board, termi) != 0) die("defineGameBoard");
    objspace.board = board;
    objspace.sn = sn;
    abAppend(&ab, "\x1b[2J", 4);
    abAppend(&ab, "\x1b[?25l", 6);
    abFlush(&ab);
}

void moveSnake(struct snake* sn, struct gameboard board) {
    switch (sn->state) {
        case LIVE:
            translateCyclic(sn, board);
            break;
        case DEAD:
            removeSnakeSegment(sn);
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
        usleep(250000);
        moveSnake(objspace.sn, objspace.board);
    }
    printf("exited out of loop");
    progressGamePlay();
    usleep(250000);
    write(STDOUT_FILENO, "\x1b[?25h", 6);
    return 0;
}
