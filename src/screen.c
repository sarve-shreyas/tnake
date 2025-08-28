#include "screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "digitaldisplay.h"
#include "fruit.h"
#include "logger.h"
#include "message.h"
#include "objectspace.h"
#include "printter.h"
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
            struct SpaceRepresentationStyle style = objectType == BOARD_BOUNDRY ? getBoundrySpaceStyle(getBoardBoundaryType(i, j, board)) : getRepresentationStyle(objectType);
            printStyleAt(ab, i, j, style);
            j += (style.len - 1);
        }
    }
}

void printDigit(struct abuf* ab, int* x, int* y, char digit) {
    int r = 0;
    int c = 0;
    int old_x = *x;
    int** arr = get_digi_repr(digit, &r, &c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int repr = arr[i][j];
            struct SpaceRepresentationStyle style = (repr) ? digital_display_set_style : digital_display_uset_style;
            printStyleAt(ab, *x + i, *y + style.len * j, style);
        }
    }
    *x = old_x;
    *y += c + 1;
}

void printScore(struct abuf* ab, struct objectspace space) {
    struct gameboard board = space.board;
    int score = space.sn->len;
    if (space.sn->state == DEAD) return;
    char buffer[100];
    int written = snprintf(buffer, 100, "SCORE - %d", score);
    int x = board.bottom_left.x + 2;
    int y = board.bottom_left.y + 4;
    for (int i = 0; i < written; i++) {
        printDigit(ab, &x, &y, buffer[i]);
    }
}

void printPromptMessage(struct abuf* ab, struct coordinate s_cord, struct coordinate e_cord) {
    static struct coordinate* cordi = NULL;

    int x = s_cord.x - 2;
    int startY = s_cord.y - 1;
    int endY = e_cord.y + 1;
    int startPromptY = cordi == NULL ? -1 : cordi->y;
    int messagePromptUIWidth = getDisplayWidth(prompt.msg, prompt.len);
    int endPromptY = cordi == NULL ? -1 : cordi->y + messagePromptUIWidth;

    if (cordi == NULL) {
        cordi = malloc(sizeof(struct coordinate));
        cordi->x = x;
        cordi->y = startY + 1;
    } else if (endPromptY == (endY - 1)) {
        cordi->y = startY + 1;
    } else {
        cordi->y += 1;
    }
    clearRow(ab, x);
    printStringAt(ab, x, startY, "[");
    printStringAt(ab, x, endY, "]");
    printStringAt(ab, cordi->x, cordi->y, prompt.msg);
}

void printObjectSpace(struct abuf* ab, struct objectspace objspace) {
    clearScreen(ab);
    printGameboard(ab, objspace);
    printScore(ab, objspace);
    printPromptMessage(ab, objspace.board.top_left, objspace.board.top_right);
}

void refreshScreen() {
    struct abuf ab = ABUF_INIT;
    printObjectSpace(&ab, objspace);
    abFlush(&ab);
}