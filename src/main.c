#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "abuffer.h"
#include "gameplay.h"
#include "logger.h"
#include "objectspace.h"
#include "screen.h"
#include "terminal.h"
#include "utils.h"

#define SNAKE_INIT_LEN 20
#define GAMEBOARD_INIT_WIDTH 40
#define GAMEBOARD_INIT_HEIGHT 32

game gameplay;
objectspaceconfigs configs = {.gameboard_height = GAMEBOARD_INIT_HEIGHT, .gameboard_width = GAMEBOARD_INIT_WIDTH, .snake_init_len = SNAKE_INIT_LEN};

void init(void) {
    struct abuf ab = ABUF_INIT;
    if (initTerminal() != 0) die("initTerminal");
    if (initObjectSpace(configs) != 0) die("initObjectspace");
    abAppend(&ab, "\x1b[2J", 4);
    abAppend(&ab, "\x1b[?25l", 6);
    abFlush(&ab);
}

int main() {
    init();
    initGameplay(NULL);
    while (objspace.sn->len) {
        moveSnake(&objspace);
        refreshScreen();
    }
    refreshScreen();
    pexit(0);
    return 0;
}
