#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

#define SNAKE_INIT_LEN 10
#define GAMEBOARD_INIT_WIDTH 40
#define GAMEBOARD_INIT_HEIGHT 32

void init(void) {
    struct snake* sn = malloc(sizeof(struct snake));
    struct gameboard board;
    if (getWindowSize(&termi.row, &termi.col) == -1) die("getWindowSize");
    if (configureSnake(SNAKE_INIT_LEN, sn) != 0) die("configgureSnake");
    if (configureGameBoard(GAMEBOARD_INIT_HEIGHT, GAMEBOARD_INIT_WIDTH, &board, termi) != 0) die("defineGameBoard");
    objspace.board = board;
    objspace.sn = sn;
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[?25l", 6);
}

void moveSnake(struct snake* sn, struct gameboard board) {
    translateZigZag(sn, board);
}

void snakeDie(struct snake* sn) {
    removeSnakeSegment(sn);
}

int main() {
    init();

    while (objspace.sn->len) {
        printObjectSpace(termi, objspace);
        usleep(50000);
        moveSnake(objspace.sn, objspace.board);
    }
    printObjectSpace(termi, objspace);
    write(STDOUT_FILENO, "\x1b[?25h", 6);
    return 0;
}
