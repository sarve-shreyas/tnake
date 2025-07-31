#include "objectspace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

// Define the global style variable
struct SpaceRepresentationStyle no_object_style = {"  ", ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK};

int getObjectOnBoard(int i, int j, struct snake* sn) {
    int snakepart = BOARD_BLOCK;
    struct snakenode* parts = sn->headpos;
    while (parts) {
        if (parts->data.coordinate.x == i && parts->data.coordinate.y == j) {
            snakepart = parts->type;
            break;
        }
        parts = parts->nextnode;
    }
    return snakepart;
}

int getObjectRepresentationOnCordinate(char* buf, int i, int j, struct objectspace space) {
    int object = NONE;
    struct gameboard board = space.board;
    struct snake* sn = space.sn;
    struct SpaceRepresentationStyle style;

    if (i >= board.top_left.x && j >= board.top_left.y && i <= board.bottom_right.x && j <= board.bottom_right.y) {
        object = BOARD_BLOCK;
        if ((j - board.top_left.y) % 2 == 0)
            object = getObjectOnBoard((i - board.top_left.x), (j - board.top_left.y) / 2, sn);
    }
    switch (object) {
        case HEAD:
            style = snake_head_style;
            break;
        case BODY_PART:
            style = snake_body_style;
            break;
        case BOARD_BLOCK:
            style = gameboard_block_style;
            break;
        case NONE:
            style = no_object_style;
            break;
        default:
            die("error invalid snake part");
            break;
    }
    strcpy(buf, colorStr(style.str, style.bgcolor, style.fgcolor));
    return strlen(style.str);
}

void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace objspace) {
    abAppend(ab, "\x1b[H", 3);
    for (int i = 1; i <= termi.row; i++) {
        for (int j = 1; j <= termi.col; j++) {
            char writeBuf[128];
            int objectSize = getObjectRepresentationOnCordinate(writeBuf, i, j, objspace);
            char buf[128];
            int bytes = snprintf(buf, 128, "\x1b[%d;%dH%s", i, j, writeBuf);
            abAppend(ab, buf, bytes);
            j += (objectSize - 1);
        }
    }
}