#ifndef FRUIT_H
#define FRUIT_H

#include "styles.h"

enum Display {
    SHOW_FRUIT,
    HIDE_FRUIT,
};

struct fruit {
    struct coordinate coordinate;
    struct SpaceRepresentationStyle style;
    int displayState;
};

int getNewFruitCoordinates(int height, int width, struct coordinate* occupied, int occupied_count, struct fruit* ft);
#endif  // FRUIT_H
