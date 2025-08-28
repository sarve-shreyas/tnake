#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "abuffer.h"
#include "gameplay.h"
#include "logger.h"
#include "objectspace.h"
#include "terminal.h"
#include "utils.h"

#define SNAKE_INIT_LEN 20
#define GAMEBOARD_INIT_WIDTH 40
#define GAMEBOARD_INIT_HEIGHT 32

struct terminal termi;
struct objectspace objspace;
objectspaceconfigs configs = {.gameboard_height = GAMEBOARD_INIT_HEIGHT, .gameboard_width = GAMEBOARD_INIT_WIDTH, .snake_init_len = SNAKE_INIT_LEN};

void init(void) {
    struct abuf ab = ABUF_INIT;
    enableRawMode();
    if (getWindowSize(&termi.row, &termi.col) == -1) die("getWindowSize");
    if (initObjectSpace(&objspace, termi, configs) != 0) die("initObjectspace");
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
    return 0;
}
