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
typedef struct {
    int snake_init_len;
    int gameboard_width;
    int gameboard_height;
} objectspaceconfigs;

extern struct objectspace objspace;
int initObjectSpace(objectspaceconfigs configs);
#endif  // OBJECTSPACE_H