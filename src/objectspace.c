#include "objectspace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "digitaldisplay.h"
#include "fruit.h"
#include "logger.h"
#include "snake.h"
#include "styles.h"
#include "utils.h"

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

struct SpaceRepresentationStyle getRepresentationStyle(int i, int j, int object) {
    switch (object) {
        case HEAD:
            return snake_head_style;
        case BODY_PART:
            return snake_body_style;
        case BOARD_BLOCK:
            return gameboard_block_style;
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

void clearScreen(struct abuf* ab) {
    abAppend(ab, "\x1b[H", 3);
}

void printStyleAt(struct abuf* ab, int x, int y, struct SpaceRepresentationStyle style) {
    char buf[200];
    int bytes = snprintf(buf, 200, "\x1b[%d;%dH%s", x, y, colorStr(style.str, style.bgcolor, style.fgcolor));
    abAppend(ab, buf, bytes);
}

void printGameboard(struct abuf* ab, struct objectspace space) {
    struct gameboard board = space.board;
    for (int i = board.top_left.x - 1; i <= board.bottom_right.x + 1; i++) {
        for (int j = board.top_left.y - 1; j <= board.bottom_right.y + 1; j++) {
            int objectType = NONE;

            if (getBoardBoundaryType(i, j, board) != NO_BOUNDARY) {
                objectType = BOARD_BOUNDRY;
            } else {
                objectType = getObjectOnBoard((i - board.top_left.x), (j - board.top_left.y) / 2, space.sn, space.fruit);
            }
            struct SpaceRepresentationStyle style = objectType == BOARD_BOUNDRY ? getBoardBoundrySpaceStyle(i, j, space.board) : getRepresentationStyle(i, j, objectType);
            printStyleAt(ab, i, j, style);
            j += (style.len - 1);
        }
    }
}

void printDigit(struct abuf* ab, int x, int y, char digit) {
    int r = 0;
    int c = 0;
    int** arr = get_digi_repr(digit, &r, &c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int repr = arr[i][j];
            struct SpaceRepresentationStyle style = (repr) ? digital_display_set_style : digital_display_uset_style;
            printStyleAt(ab, x + i, y + style.len * j, style);
        }
    }
}

void printScore(struct abuf* ab, struct objectspace space) {
    struct gameboard board = space.board;
    int score = space.sn->len;
    char buffer[100];
    int written = snprintf(buffer, 100, "SCORE - %d", score);
    int x = board.bottom_left.x + 2;
    int y = board.bottom_left.y + 4;
    for (int i = 0; i < written; i++) {
        printDigit(ab, x, y + 7 * i, buffer[i]);
    }
}

void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace objspace) {
    clearScreen(ab);
    printGameboard(ab, objspace);
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
