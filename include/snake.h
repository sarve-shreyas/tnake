#ifndef SNAKE_H
#define SNAKE_H

#include "space.h"

struct snakepartdata {
    int direction;
    struct coordinate coordinate;
};

struct snakenode {
    struct snakepartdata data;
    struct snakenode* nextnode;
    struct snakenode* prevnode;
    int type;
};

extern struct SpaceRepresentationStyle snake_head_style;
extern struct SpaceRepresentationStyle snake_body_style;

struct snake {
    struct snakenode* headpos;
    struct snakenode* tail;
    int len;
};

void addSnakeSegment(struct snake* sn, int type, struct snakepartdata data);
void removeSnakeSegment(struct snake* sn);
void updatePositionWithDirection(struct snake* sn);
void moveBodyParts(struct snake* sn);
int configureSnake(int len, struct snake* sn);
#endif // SNAKE_H