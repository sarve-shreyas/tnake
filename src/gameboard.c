
#include "gameboard.h"

#include <unistd.h>

#include "ansi.h"
#include "space.h"

struct SpaceRepresentationStyle gameboard_block_style = {"..", ANSI_COLOR_FG_WHITE, ANSI_COLOR_BG_BLACK};

int configureGameBoard(int height, int width, struct gameboard* board, struct terminal termi) {
    board->width = width;
    board->height = height;
    if (board->height > termi.row || 2 * board->width > termi.col) return -1;

    int rCol = 2 * board->width;
    int rRow = board->height;

    int bcx = (termi.col - rCol) / 2;
    int brx = (termi.row - rRow) / 2;

    struct coordinate top_left = {brx, bcx};
    struct coordinate top_right = {brx, bcx + rCol - 1};
    struct coordinate bottom_right = {brx + rRow - 1, bcx + rCol - 1};
    struct coordinate bottom_left = {brx + rRow - 1, bcx};
    board->bottom_left = bottom_left;
    board->bottom_right = bottom_right;
    board->top_left = top_left;
    board->top_right = top_right;
    return 0;
}
