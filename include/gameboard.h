#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "space.h"
#include "terminal.h"

/**
 * @brief Represents the game board structure for the game.
 *
 * This structure holds all relevant data for managing the state
 * and properties of the game board.
 */
struct gameboard {
    int height;
    int width;
    struct coordinate top_left;
    struct coordinate top_right;
    struct coordinate bottom_right;
    struct coordinate bottom_left;
};
extern struct SpaceRepresentationStyle gameboard_block_style;

int configureGameBoard(int height, int width, struct gameboard* board, struct terminal termi);

#endif  // GAMEBOARD_H