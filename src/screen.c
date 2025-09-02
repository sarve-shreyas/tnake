#include "screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "digitaldisplay.h"
#include "fruit.h"
#include "gmenu.h"
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

void printGameboard(struct abuf* ab) {
    struct gameboard board = objspace.board;
    for (int i = board.top_left.x - 1; i <= board.bottom_right.x + 1; i++) {
        for (int j = board.top_left.y - 1; j <= board.bottom_right.y + 1; j++) {
            int objectType = NONE;

            if (getBoardBoundaryType(i, j, board) != NO_BOUNDARY) {
                objectType = BOARD_BOUNDRY;
            } else {
                objectType = getObjectOnBoard((i - board.top_left.x), (j - board.top_left.y) / 2, objspace.sn, objspace.fruit);
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

void printScore(struct abuf* ab) {
    struct gameboard board = objspace.board;
    int score = objspace.sn->len;
    if (objspace.sn->state == DEAD) return;
    char buffer[100];
    int written = snprintf(buffer, 100, "SCORE - %d", score);
    int x = board.bottom_left.x + 2;
    int y = board.bottom_left.y + 4;
    for (int i = 0; i < written; i++) {
        printDigit(ab, &x, &y, buffer[i]);
    }
}

void printPromptMessage(struct abuf* ab) {
    static struct coordinate* cordi = NULL;

    int x = objspace.board.top_left.x - 2;
    int startY = objspace.board.top_left.y - 1;
    int endY = objspace.board.top_right.y + 1;
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
    if (prompt.state == DEAD) cordi = NULL;
}

void printObjectSpace(struct abuf* ab) {
    clearScreen(ab);
    printGameboard(ab);
    printScore(ab);
    printPromptMessage(ab);
}

void refreshGameScreen() {
    refreshPromptMessage();
    refreshScore();
    refreshGameboard();
}

void refreshScore() {
    struct abuf ab = ABUF_INIT;
    printScore(&ab);
    abFlush(&ab);
}

void refreshGameboard() {
    struct abuf ab = ABUF_INIT;
    printGameboard(&ab);
    abFlush(&ab);
}

void refreshPromptMessage() {
    struct abuf ab = ABUF_INIT;
    printPromptMessage(&ab);
    abFlush(&ab);
}

void printRowCenter(struct abuf* ab, int row, const char* str, struct SpaceRepresentationStyle style) {
    int cols = terminal.col;
    int len = strlen(str);
    int col = (cols > len) ? (cols - len) / 2 : 1;
    clearRow(ab, row);
    printStringAtWithStyle(ab, row, col, str, style);
}

void refreshMenuScreen() {
    menudata m;
    if (getmenu(&m) != 0) die("menudata");
    struct abuf ab = ABUF_INIT;
    clearScreen(&ab);
    int rows = terminal.row;
    int start_row = rows / 2 - m.len;
    char title[strlen(m.title) + 20];
    snprintf(title, strlen(m.title) + 20, "=== %s ===", m.title);
    printRowCenter(&ab, start_row - 2, title, getRepresentationStyle(GMENU_TITLE));
    for (int i = 0; i < m.len; i++) {
        struct SpaceRepresentationStyle style = i == m.selected ? getRepresentationStyle(GEMENU_SELECTED_ITEM) : getRepresentationStyle(GEMENU_U_SELECTED_ITEM);
        int len = strlen(m.options[i]);
        int row = start_row + i * 2;
        char buffer[len + 20];
        if (i == m.selected)
            snprintf(buffer, len + 20, " %s       ", m.options[i]);
        else
            snprintf(buffer, len + 20, "%s", m.options[i]);
        printRowCenter(&ab, row, buffer, style);
    }
    printRowCenter(&ab, start_row + m.len * 3, MENU_SCREEN_INFO_MSG, getRepresentationStyle(GMENU_FOOTER));
    abFlush(&ab);
}

void refreshScreenPromptMessageScreen() {
    screenpromptmegs megs;
    if (getScreenpromptmegs(&megs) != 0) die("getScreenpromptmegs");
    struct abuf ab = ABUF_INIT;
    clearScreen(&ab);
    clearScreen(&ab);
    int rows = terminal.row;
    int cols = terminal.col;
    int start_row = (rows - megs.len - 2) / 2;
    start_row = start_row <= 0 ? 1 : start_row;
    debug("start row %d", start_row);
    char title[strlen(megs.title) + 20];
    snprintf(title, 50, "=== %s ===", megs.title);
    struct SpaceRepresentationStyle style = no_object_style;
    printRowCenter(&ab, start_row, title, style);
    start_row += 2;
    int max_len = -1;
    for (int i = 0; i < megs.len; i++) {
        int displayWidth = getDisplayWidth(megs.megs[i], strlen(megs.megs[i]));
        if (max_len < displayWidth) {
            max_len = displayWidth;
        }
    }
    int col = (cols > max_len) ? (cols - max_len) / 2 : 1;
    for (int i = 0; i < megs.len; i++) {
        int row = start_row + i;
        printStringAtWithStyle(&ab, row, col, megs.megs[i], style);
    }
    printRowCenter(&ab, rows - 2, megs.footer, no_object_style);
    abFlush(&ab);
}
