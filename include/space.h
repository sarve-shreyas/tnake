#ifndef SPACE_H
#define SPACE_H

enum SpaceComponent {
    HEAD = 1000,
    BODY_PART,
    BOARD_BLOCK,
    BOARD_BOUNDRY,
    FRUIT,
    GEMENU_SELECTED_ITEM,
    GEMENU_U_SELECTED_ITEM,
    GMENU_TITLE,
    GMENU_FOOTER,
    NONE
};

enum Direction {
    DIRECTION_UP = 10,
    DIRECTION_RIGHT = 1,
    DIRECTION_DOWN = 11,
    DIRECTION_LEFT = -1,
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

enum LivingState {
    ALIVE = 1,
    DEAD = -1
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
int sameCordinates(struct coordinate a, struct coordinate b);
#endif  // SPACE_H
