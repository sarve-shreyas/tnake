#include "printter.h"

#include <stdio.h>
#include <stdlib.h>

#include "ansi.h"
#include "terminal.h"

void printStyleAt(struct abuf* ab, int x, int y, struct SpaceRepresentationStyle style) {
    char buf[200];
    moveCursor(ab, x, y);
    int bytes = snprintf(buf, 200, "%s", colorStr(style.str, style.bgcolor, style.fgcolor));
    abAppend(ab, buf, bytes);
}

void printStringAt(struct abuf* ab, int x, int y, char* str) {
    char buf[200];
    moveCursor(ab, x, y);
    int bytes = snprintf(buf, 200, "%s", str);
    abAppend(ab, buf, bytes);
}

void printStringAtWithStyle(struct abuf* ab, int x, int y, const char* str, struct SpaceRepresentationStyle style) {
    char buf[200];
    moveCursor(ab, x, y);
    int bytes = snprintf(buf, 200, "%s", colorStr(str, style.bgcolor, style.fgcolor));
    abAppend(ab, buf, bytes);
}

void clearScreen(struct abuf* ab) {
    abAppend(ab, "\x1b[H", 3);
    for (int i = 1; i <= terminal.row; i++) {
        clearRow(ab, i);
    }
    abAppend(ab, "\x1b[H", 3);
}

void clearRow(struct abuf* ab, int row) {
    char buffer[20];
    moveCursor(ab, row, 0);
    int written = snprintf(buffer, 20, "\x1b[2K");
    abAppend(ab, buffer, written);
}

void moveCursor(struct abuf* ab, int x, int y) {
    char buffer[20];
    int bytes = snprintf(buffer, 20, "\x1b[%d;%dH", x, y);
    abAppend(ab, buffer, bytes);
}

void changeCusrsorState(int state) {
    struct abuf ab = ABUF_INIT;
    changeCursorStateToBuf(&ab, state);
    abFlush(&ab);
}

void changeCursorStateToBuf(struct abuf* ab, int state) {
    switch (state) {
        case ALIVE:
            abAppend(ab, "\x1b[?25h", 6);
            break;
        case DEAD:
            abAppend(ab, "\x1b[?25l", 6);
            break;
    }
}
