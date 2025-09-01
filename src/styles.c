#include "styles.h"

#include "ansi.h"

// Gameboard
#define GAMEBOARD_TOP_LEFT "┌"
#define GAMEBOARD_TOP_RIGHT "┐"
#define GAMEBOARD_BOTTOM_LEFT "└"
#define GAMEBOARD_BOTTOM_RIGHT "┘"
#define GAMEBOARD_HORIZONTAL "─"
#define GAMEBOARD_VERTICAL "│"
#define GAMEBOARD_BLOCK "* "
// No Object
#define NO_OBJECT " "

// Snake
#define SNAKE_HEAD "  "
#define SNAKE_BODY_PART "  "

// Fruit
#define FRUIT_BLOCK "  "

// Digital Display
#define DIGITAL_DISPLAY_SET "▄"
#define DIGITAL_DISPLAY_USET " "

// No object space styling
const struct SpaceRepresentationStyle no_object_style = {.str = NO_OBJECT, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};

// Snake space styling
const struct SpaceRepresentationStyle snake_head_style = {.str = SNAKE_HEAD, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_MAGENTA, .len = 2};
const struct SpaceRepresentationStyle snake_body_style = {.str = SNAKE_BODY_PART, .fgcolor = ANSI_COLOR_FG_BLACK, .bgcolor = ANSI_COLOR_BG_GREEN, .len = 2};

// Gameboard space styling
const struct SpaceRepresentationStyle gameboard_block_style = {.str = GAMEBOARD_BLOCK, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 2};
const struct SpaceRepresentationStyle gameboard_top_left_style = {.str = GAMEBOARD_TOP_LEFT, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};
const struct SpaceRepresentationStyle gameboard_top_right_style = {.str = GAMEBOARD_TOP_RIGHT, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};
const struct SpaceRepresentationStyle gameboard_bottom_left_style = {.str = GAMEBOARD_BOTTOM_LEFT, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};
const struct SpaceRepresentationStyle gameboard_bottom_right_style = {.str = GAMEBOARD_BOTTOM_RIGHT, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};
const struct SpaceRepresentationStyle gameboard_horizontal_style = {.str = GAMEBOARD_HORIZONTAL, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};
const struct SpaceRepresentationStyle gameboard_vertical_style = {.str = GAMEBOARD_VERTICAL, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};

// Fruit
const struct SpaceRepresentationStyle fruit_block_style = {.str = FRUIT_BLOCK, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLUE, .len = 2};

// Digital Display
const struct SpaceRepresentationStyle digital_display_set_style = {.str = DIGITAL_DISPLAY_SET, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_GREEN, .len = 1};
const struct SpaceRepresentationStyle digital_display_uset_style = {.str = DIGITAL_DISPLAY_USET, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 1};

// Menu
const struct SpaceRepresentationStyle gmenu_selected_style = {.str = NULL, .fgcolor = ANSI_COLOR_FG_BLACK, .bgcolor = ANSI_COLOR_BG_WHITE, .len = 0};
const struct SpaceRepresentationStyle gmenu_uselected_style = {.str = NULL, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 0};
const struct SpaceRepresentationStyle gmenu_title_style = {.str = NULL, .fgcolor = ANSI_COLOR_FG_WHITE, .bgcolor = ANSI_COLOR_BG_BLACK, .len = 0};

struct SpaceRepresentationStyle getBoundrySpaceStyle(int boundry_type) {
    switch (boundry_type) {
        case TOP_LEFT_CORNER:
            return gameboard_top_left_style;
        case TOP_RIGHT_CORNER:
            return gameboard_top_right_style;
        case BOTTOM_RIGHT_CORNER:
            return gameboard_bottom_right_style;
        case BOTTOM_LEFT_CORNER:
            return gameboard_bottom_left_style;
        case TOP:
        case BOTTOM:
            return gameboard_horizontal_style;
        case LEFT_BOUNDARY:
        case RIGHT_BOUNDARY:
            return gameboard_vertical_style;
        default:
            return no_object_style;
    }
    return no_object_style;
}

struct SpaceRepresentationStyle getRepresentationStyle(int object) {
    switch (object) {
        case HEAD:
            return snake_head_style;
        case BODY_PART:
            return snake_body_style;
        case BOARD_BLOCK:
            return gameboard_block_style;
        case FRUIT:
            return fruit_block_style;
        case NONE:
            return no_object_style;
        case GEMENU_SELECTED_ITEM:
            return gmenu_selected_style;
        case GEMENU_U_SELECTED_ITEM:
            return gmenu_uselected_style;
        case GMENU_TITLE:
        case GMENU_FOOTER:
            return gmenu_title_style;
        default:
            return no_object_style;
    }
    return no_object_style;
}
