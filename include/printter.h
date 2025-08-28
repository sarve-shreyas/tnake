#ifndef PRINTTER_H
#define PRINTTER_H

#include "space.h"
#include "abuffer.h"

void printStyleAt(struct abuf* ab, int x, int y, struct SpaceRepresentationStyle style);
void printStringAt(struct abuf* ab, int x, int y, char* str);
void clearScreen(struct abuf* ab);
void clearRow(struct abuf* ab, int row);
void moveCursor(struct abuf* ab, int x, int y);
void changeCusrsorState(int state);
void changeCursorStateToBuf(struct abuf* ab, int state);
#endif  // PRINTTER_H