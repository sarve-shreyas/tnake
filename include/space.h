#ifndef SPACE_H
#define SPACE_H

#define OUTSIDE_SPACE_CORDINATE (struct coordinate){-1, -1}

enum SpaceComponent {
    HEAD = 0x50,
    BODY_PART = 0x51,
    BOARD_BLOCK = 0x52,
    BOARD_BOUNDRY = 0x53,
    FRUIT = 0x54,
    GEMENU_SELECTED_ITEM = 0x55,
    GEMENU_U_SELECTED_ITEM = 0x56,
    GMENU_TITLE = 0x57,
    GMENU_FOOTER = 0x58,
    NONE = 0x59
};

enum Direction {
    DIRECTION_UP = 0x30,
    DIRECTION_RIGHT = 0x31,
    DIRECTION_DOWN = 0x32,
    DIRECTION_LEFT = 0x33,
};

enum BOUNDARY_TYPE {
    TOP = 0x20,
    BOTTOM = 0x22,
    LEFT_BOUNDARY = 0x23,
    RIGHT_BOUNDARY = 0x24,
    TOP_LEFT_CORNER = 0x25,
    TOP_RIGHT_CORNER = 0x26,
    BOTTOM_LEFT_CORNER = 0x27,
    BOTTOM_RIGHT_CORNER = 0x28,
    NO_BOUNDARY = 0x29
};

enum LivingState { ALIVE = 1, DEAD = -1 };

enum Alignments {
    START_ALIGNMENT = 0x10,
    MIDDLE_ALIGNMENT = 0x11,
    END_ALIGNMENT = 0x12,
};

typedef struct {
    int vAlign;
    int hAlign;
} Alignment;

struct SpaceRepresentationStyle {
    char* str;
    int fgcolor;
    int bgcolor;
    int len;
    Alignment* contentAlign;
    Alignment* align;
};

struct coordinate {
    int x;
    int y;
};

int sameCordinates(struct coordinate a, struct coordinate b);
#endif // SPACE_H
