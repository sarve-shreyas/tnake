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

void printObjectSpace(struct abuf* ab, struct terminal termi, struct objectspace);
int initObjectSpace(struct objectspace* space, struct terminal termi, objectspaceconfigs configs);
#endif  // OBJECTSPACE_H