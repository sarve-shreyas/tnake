#include "styles.h"

#include "ansi.h"

#define GAMEBOARD_TOP_LEFT "┌"
#define GAMEBOARD_TOP_RIGHT "┐"
#define GAMEBOARD_BOTTOM_LEFT "└"
#define GAMEBOARD_BOTTOM_RIGHT "┘"
#define GAMEBOARD_HORIZONTAL "─"
#define GAMEBOARD_VERTICAL "│"

// No object space styling
struct SpaceRepresentationStyle no_object_style = {" ", ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};

// Snake space styling
struct SpaceRepresentationStyle snake_head_style = {":)", ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_MAGENTA, 2};
struct SpaceRepresentationStyle snake_body_style = {"  ", ANSI_COLOR_FG_BLACK, ANSI_COLOR_BG_GREEN, 2};

// Gameboard space styling
struct SpaceRepresentationStyle gameboard_block_style = {"..", ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 2};
struct SpaceRepresentationStyle gameboard_top_left_style = {GAMEBOARD_TOP_LEFT, ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};
struct SpaceRepresentationStyle gameboard_top_right_style = {GAMEBOARD_TOP_RIGHT, ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};
struct SpaceRepresentationStyle gameboard_bottom_left_style = {GAMEBOARD_BOTTOM_LEFT, ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};
struct SpaceRepresentationStyle gameboard_bottom_right_style = {GAMEBOARD_BOTTOM_RIGHT, ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};
struct SpaceRepresentationStyle gameboard_horizontal_style = {GAMEBOARD_HORIZONTAL, ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};
struct SpaceRepresentationStyle gameboard_vertical_style = {GAMEBOARD_VERTICAL, ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK, 1};

