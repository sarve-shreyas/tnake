#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include "ansi.h"
#include "gameboard.h"
#include "snake.h"
#include "space.h"
#include "abuffer.h"

struct objectspace {
    struct snake* sn;
    struct gameboard board;
};

int getObjectRepresentationOnCordinate(char* buf, int i, int j, struct objectspace);

void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace);
#endif  // OBJECTSPACE_H