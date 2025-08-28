
#include "gameboard.h"

#include <unistd.h>

#include "ansi.h"
#include "space.h"


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

int getBoardBoundaryType(int i, int j, struct gameboard board) {
    if (i == board.top_left.x - 1 && j == board.top_left.y - 1)
        return TOP_LEFT_CORNER;
    else if (i == board.top_left.x - 1 && j == board.bottom_right.y + 1)
        return TOP_RIGHT_CORNER;
    else if (i == board.bottom_right.x + 1 && j == board.top_left.y - 1)
        return BOTTOM_LEFT_CORNER;
    else if (i == board.bottom_right.x + 1 && j == board.bottom_right.y + 1)
        return BOTTOM_RIGHT_CORNER;
    else if (i == board.top_left.x - 1 && j >= board.top_left.y && j <= board.bottom_right.y)
        return TOP;
    else if (i == board.bottom_right.x + 1 && j >= board.top_left.y && j <= board.bottom_right.y)
        return BOTTOM;
    else if (j == board.top_left.y - 1 && i >= board.top_left.x && i <= board.bottom_right.x)
        return LEFT_BOUNDARY;
    else if (j == board.bottom_right.y + 1 && i >= board.top_left.x && i <= board.bottom_right.x)
        return RIGHT_BOUNDARY;
    else
        return NO_BOUNDARY;
}
