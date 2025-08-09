#include "fruit.h"

#include <stdlib.h>
#include <time.h>

#include "styles.h"

#define RANDOM_ITERATIONS 100
struct coordinate getNewCordinates(int height, int width) {
    srand(time(NULL));
    int i = rand() % height;
    int j = rand() % width;
    struct coordinate coordinate = {.x = i, .y = j};
    return coordinate;
}

int getNewFruitCoordinates(int height, int width, struct coordinate* occupied, int occupied_count, struct fruit* ft) {
    struct coordinate coord;
    int isOccupied;
    int iterations = 0;
    do {
        coord = getNewCordinates(height, width);
        isOccupied = 0;
        for (int k = 0; k < occupied_count; k++) {
            if (occupied[k].x == coord.x && occupied[k].y == coord.y) {
                isOccupied = 1;
                break;
            }
        }
        iterations++;
    } while (isOccupied && iterations <= RANDOM_ITERATIONS);
    if (iterations > RANDOM_ITERATIONS) return 1;
    ft->coordinate = coord;
    ft->style = fruit_block_style;
    ft->displayState = SHOW_FRUIT;
    return 0;
}

