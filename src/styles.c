#include "styles.h"

#include "ansi.h"

// Gameboard
#define GAMEBOARD_TOP_LEFT "┌"
#define GAMEBOARD_TOP_RIGHT "┐"
#define GAMEBOARD_BOTTOM_LEFT "└"
#define GAMEBOARD_BOTTOM_RIGHT "┘"
#define GAMEBOARD_HORIZONTAL "─"
#define GAMEBOARD_VERTICAL "│"
#define GAMEBOARD_BLOCK "**"
// No Object
#define NO_OBJECT " "

// Snake
#define SNAKE_HEAD ":)"
#define SNAKE_BODY_PART "  "

// Fruit
#define FRUIT_BLOCK "  "

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

