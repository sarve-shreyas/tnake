#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "gameboard.h"
#include "snake.h"

void translateZigZag(struct snake* sn, struct gameboard board) {
    moveBodyParts(sn);
    sn->headpos->data.coordinate.y += sn->headpos->data.direction;
    if (sn->headpos->data.coordinate.y >= board.width) {
        sn->headpos->data.coordinate.x++;
        sn->headpos->data.coordinate.x = sn->headpos->data.coordinate.x % board.height;
        sn->headpos->data.direction = LEFT;
        sn->headpos->data.coordinate.y = board.width - 1;
    } else if (sn->headpos->data.coordinate.y < 0) {
        sn->headpos->data.coordinate.x++;
        sn->headpos->data.coordinate.x = sn->headpos->data.coordinate.x % board.height;
        sn->headpos->data.direction = RIGHT;
        sn->headpos->data.coordinate.y = 0;
    }
}

void translateCircle(struct snake* sn, struct gameboard board) {
    moveBodyParts(sn);
    int tsx = sn->headpos->data.coordinate.x;
    int tsy = sn->headpos->data.coordinate.y;
    if (tsy == board.width - 1 && tsx == 0) {
        sn->headpos->data.direction = DOWN;
    } else if (tsy == 0 && tsx == board.height - 1) {
        sn->headpos->data.direction = UP;
    } else if (tsy == board.width - 1 && tsx == board.height - 1) {
        sn->headpos->data.direction = LEFT;
    } else if (tsy == 0 && tsx == 0) {
        sn->headpos->data.direction = RIGHT;
    }
    updatePositionWithDirection(sn);
}

void translateCyclic(struct snake* sn, struct gameboard board) {
    static int xdip = 0;
    static int ydip = 0;
    moveBodyParts(sn);
    int tsx = sn->headpos->data.coordinate.x;
    int tsy = sn->headpos->data.coordinate.y;
    int direction = sn->headpos->data.direction;
    if (tsx == 0 && tsy == 0) {
        sn->headpos->data.direction = RIGHT;
    } else if (tsx == xdip + 1 && tsy == ydip) {
        sn->headpos->data.direction = RIGHT;
        xdip++;
        ydip++;
    } else if (tsy == board.width - 1 - ydip && tsx == xdip) {
        sn->headpos->data.direction = DOWN;
    } else if (tsy == ydip && tsx == board.height - 1 - xdip) {
        sn->headpos->data.direction = UP;
    } else if (tsy == board.width - 1 - ydip && tsx == board.height - 1 - xdip) {
        sn->headpos->data.direction = LEFT;
    }
    updatePositionWithDirection(sn);
    updateSnakeState(sn);
}

#endif  // TRANSLATE_H
