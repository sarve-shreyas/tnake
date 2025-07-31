#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include "ansi.h"
#include "gameboard.h"
#include "snake.h"
#include "space.h"

struct objectspace {
    struct snake* sn;
    struct gameboard board;
};

// External declaration - defined in objectspace.c
extern struct SpaceRepresentationStyle no_object_style;

int getObjectRepresentationOnCordinate(char* buf, int i, int j, struct objectspace);

void printObjectSpace(struct terminal termi, struct objectspace);
#endif  // OBJECTSPACE_H