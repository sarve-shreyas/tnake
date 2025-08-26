#include "objectspace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fruit.h"
#include "logger.h"
#include "snake.h"
#include "styles.h"
#include "utils.h"
#include "digitaldisplay.h"

int getObjectOnBoard(int i, int j, struct snake* sn, struct fruit* ft) {
    int snakepart = BOARD_BLOCK;
    struct snakenode* parts = sn->headpos;
    while (parts) {
        if (parts->data.coordinate.x == i && parts->data.coordinate.y == j) {
            snakepart = parts->type;
            break;
        }
        parts = parts->nextnode;
    }
    if (snakepart == BOARD_BLOCK) {
        if (ft->coordinate.x == i && ft->coordinate.y == j) {
            return FRUIT;
        }
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
    return no_object_style;
}

struct SpaceRepresentationStyle getObjectRepresentationOnCordinate(int i, int j, struct objectspace space) {
    int object = NONE;
    struct gameboard board = space.board;
    struct snake* sn = space.sn;
    struct fruit* ft = space.fruit;
    struct SpaceRepresentationStyle style;
    if (i >= board.top_left.x && j >= board.top_left.y && i <= board.bottom_right.x && j <= board.bottom_right.y) {
        object = BOARD_BLOCK;
        if ((j - board.top_left.y) % 2 == 0)
            object = getObjectOnBoard((i - board.top_left.x), (j - board.top_left.y) / 2, sn, ft);
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
        case FRUIT:
            style = fruit_block_style;
            break;
        case NONE:
            style = no_object_style;
            break;
        default:
            die("error invalid snake part");
            break;
    }
    return style;
}

struct SpaceRepresentationStyle getRepresentationStyle(int i, int j, int object, struct objectspace space) {
    switch (object) {
        case HEAD:
            return snake_head_style;
        case BODY_PART:
            return snake_body_style;
        case BOARD_BLOCK:
            return gameboard_block_style;
        case BOARD_BOUNDRY:
            return getBoardBoundrySpaceStyle(i, j, space.board);
        case FRUIT:
            return fruit_block_style;
        case NONE:
            return no_object_style;
        default:
            die("error invalid snake part");
            break;
    }
    return no_object_style;
}

void printStyleAt(struct abuf* ab, int x, int y, struct SpaceRepresentationStyle style) {
    char buf[128];
    int bytes = snprintf(buf, 128, "\x1b[%d;%dH%s", x, y, colorStr(style.str, style.bgcolor, style.fgcolor));
    abAppend(ab, buf, bytes);
}

void printAt(int x, int y, char* val) {
    char buffer[128];
    int bytes = snprintf(buffer, 128, "\x1b[%d;%dH%s", x, y, val);
    write(STDOUT_FILENO, buffer, bytes);
}

void printDigit(struct abuf* ab, int x, int y, int digit) {
    int inc_x = 0;
    int inc_y = 0;
    int repr = get_digi_repr(digit);
    for (int i = 0; i < BITS_SIZE; i++) {
        if (i % DISPLAY_COLS == 0) {
            inc_x++;
            inc_y = 0;
        }
        if (repr & (1 << i)) {
            struct SpaceRepresentationStyle style = digital_display_set_style;
            printStyleAt(ab, x + inc_x, y + style.len * inc_y, style);
        }
        inc_y++;
    }
}

void printScore(struct abuf* ab, struct objectspace space) {
    struct gameboard board = space.board;
    int score = space.sn->len;
    char buffer[100];
    int written = snprintf(buffer, 100, "%d", score);
    int x = board.bottom_left.x + 2;
    int y = (board.bottom_left.y + board.bottom_right.y) / 2;
    for (int i = 0; i < written; i++) {
        printDigit(ab, x, y + 7 * i, buffer[i]);
    }
}
void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace objspace) {
    abAppend(ab, "\x1b[H", 3);
    for (int i = 1; i <= termi.row; i++) {
        for (int j = 1; j <= termi.col; j++) {
            char writeBuf[128];
            struct SpaceRepresentationStyle objectStyle = getObjectRepresentationOnCordinate(i, j, objspace);
            printStyleAt(ab, i, j, objectStyle);
            int objectSize = objectStyle.len;
            if (objectSize) j += (objectSize - 1);
        }
    }
    printScore(ab, objspace);
}

int initFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;

    int occ_len = 0;
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    struct fruit* ft = malloc(sizeof *ft);
    if (!ft) {
        free(occ);
        return -1;
    }
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, ft) != 0) {
        free(occ);
        free(ft);
        return -1;
    }
    space->fruit = ft;
    free(occ);
    return 0;
}
