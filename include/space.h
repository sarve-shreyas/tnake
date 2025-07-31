#ifndef SPACE_H
#define SPACE_H

enum SpaceComponent {
    HEAD = 1000,
    BODY_PART,
    BOARD_BLOCK,
    NONE
};

enum Direction {
    UP = 10,
    RIGHT = 1,
    DOWN = 11,
    LEFT = -1,
};

struct SpaceRepresentationStyle {
    char* str;
    int fgcolor;
    int bgcolor;
};

struct coordinate {
    int x;
    int y;
};

#endif  // SPACE_H