#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include "abuffer.h"
#include "ansi.h"
#include "fruit.h"
#include "gameboard.h"
#include "snake.h"
#include "space.h"

struct objectspace {
    struct snake* sn;
    struct gameboard board;
    struct fruit* fruit;
};

void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace);
int initFruit(struct objectspace* space);
#endif  // OBJECTSPACE_H