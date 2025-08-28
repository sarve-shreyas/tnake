#ifndef STYLES_H
#define STYLES_H

#include "space.h"

// No object in space
extern const struct SpaceRepresentationStyle no_object_style;

// Snake 
extern const struct SpaceRepresentationStyle snake_head_style;
extern const struct SpaceRepresentationStyle snake_body_style;

// Gameboard
extern const struct SpaceRepresentationStyle gameboard_block_style;
extern const struct SpaceRepresentationStyle gameboard_top_left_style;
extern const struct SpaceRepresentationStyle gameboard_top_right_style;
extern const struct SpaceRepresentationStyle gameboard_bottom_left_style;
extern const struct SpaceRepresentationStyle gameboard_bottom_right_style;
extern const struct SpaceRepresentationStyle gameboard_horizontal_style;
extern const struct SpaceRepresentationStyle gameboard_vertical_style;

// Fruit
extern const struct SpaceRepresentationStyle fruit_block_style;

// Digital Display
extern const struct SpaceRepresentationStyle digital_display_set_style;
extern const struct SpaceRepresentationStyle digital_display_uset_style;


struct SpaceRepresentationStyle getRepresentationStyle(int object);
struct SpaceRepresentationStyle getBoundrySpaceStyle(int boundry_type);
#endif // STYLES_H
