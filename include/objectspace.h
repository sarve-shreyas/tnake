#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include "ansi.h"
#include "gameboard.h"
#include "snake.h"
#include "space.h"
#include "abuffer.h"
#include "fruit.h"

struct objectspace {
    struct snake* sn;
    struct gameboard board;
    struct fruit* fruit;
};

int getObjectRepresentationOnCordinate(char* buf, int i, int j, struct objectspace);
void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace);
int initFruit(struct objectspace* space);
#endif  // OBJECTSPACE_H