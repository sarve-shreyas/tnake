#include "printter.h"

#include <stdio.h>
#include <stdlib.h>

#include "ansi.h"

void printStyleAt(struct abuf* ab, int x, int y, struct SpaceRepresentationStyle style) {
    char buf[200];
    int bytes = snprintf(buf, 200, "\x1b[%d;%dH%s", x, y, colorStr(style.str, style.bgcolor, style.fgcolor));
    abAppend(ab, buf, bytes);
}

void printStringAt(struct abuf* ab, int x, int y, char* str) {
    char buf[200];
    int bytes = snprintf(buf, 200, "\x1b[%d;%dH%s", x, y, str);
    abAppend(ab, buf, bytes);
}

void clearScreen(struct abuf* ab) {
    abAppend(ab, "\x1b[H", 3);
}

void clearRow(struct abuf* ab, int row) {
    char buffer[20];
    int written = snprintf(buffer, 20, "\x1b[%d;%dH\x1b[2K", row, 1);
    abAppend(ab, buffer, written);
}
