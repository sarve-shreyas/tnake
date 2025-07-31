#ifndef SPACE_H
#define SPACE_H

enum SpaceComponent {
    HEAD = 1000,
    BODY_PART,
    BOARD_BLOCK,
    BOARD_BOUNDRY,
    NONE
};

enum Direction {
    UP = 10,
    RIGHT = 1,
    DOWN = 11,
    LEFT = -1,
};

enum BOUNDARY_TYPE {
    TOP,
    BOTTOM,
    LEFT_BOUNDARY,
    RIGHT_BOUNDARY,
    TOP_LEFT_CORNER,
    TOP_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER,
    BOTTOM_RIGHT_CORNER,
    NO_BOUNDARY
};

struct SpaceRepresentationStyle {
    char* str;
    int fgcolor;
    int bgcolor;
    int len;
};

struct coordinate {
    int x;
    int y;
};

#endif  // SPACE_H