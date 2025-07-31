#include "objectspace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "styles.h"
#include "utils.h"

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

int getBoardBoundaryType(int i, int j, struct gameboard board) {
    if (i == board.top_left.x - 1 && j == board.top_left.y - 1)
        return TOP_LEFT_CORNER;
    else if (i == board.top_left.x - 1 && j == board.bottom_right.y + 1)
        return TOP_RIGHT_CORNER;
    else if (i == board.bottom_right.x + 1 && j == board.top_left.y - 1)
        return BOTTOM_LEFT_CORNER;
    else if (i == board.bottom_right.x + 1 && j == board.bottom_right.y + 1)
        return BOTTOM_RIGHT_CORNER;
    else if (i == board.top_left.x - 1 && j >= board.top_left.y && j <= board.bottom_right.y)
        return TOP;
    else if (i == board.bottom_right.x + 1 && j >= board.top_left.y && j <= board.bottom_right.y)
        return BOTTOM;
    else if (j == board.top_left.y - 1 && i >= board.top_left.x && i <= board.bottom_right.x)
        return LEFT_BOUNDARY;
    else if (j == board.bottom_right.y + 1 && i >= board.top_left.x && i <= board.bottom_right.x)
        return RIGHT_BOUNDARY;
    else
        return NO_BOUNDARY;
}
struct SpaceRepresentationStyle getBoardBoundrySpaceStyle(int i, int j, struct gameboard board) {
    char* str;
    switch (getBoardBoundaryType(i, j, board)) {
        case TOP_LEFT_CORNER:
            return gameboard_top_left_style;
        case TOP_RIGHT_CORNER:
            return gameboard_top_right_style;
        case BOTTOM_RIGHT_CORNER:
            return gameboard_bottom_right_style;
        case BOTTOM_LEFT_CORNER:
            return gameboard_bottom_left_style;
        case TOP:
        case BOTTOM:
            return gameboard_horizontal_style;
        case LEFT_BOUNDARY:
        case RIGHT_BOUNDARY:
            return gameboard_vertical_style;
        default:
            die("getBoardBoundrySpaceStyle");
            break;
    }
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
    } else if (getBoardBoundaryType(i, j, board) != NO_BOUNDARY) {
        object = BOARD_BOUNDRY;
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
        case BOARD_BOUNDRY:
            style = getBoardBoundrySpaceStyle(i, j, board);
            break;
        case NONE:
            style = no_object_style;
            break;
        default:
            die("error invalid snake part");
            break;
    }
    strcpy(buf, colorStr(style.str, style.bgcolor, style.fgcolor));
    return style.len;
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
