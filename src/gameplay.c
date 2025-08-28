#include "gameplay.h"

#include <unistd.h>

#include "keyboard.h"
#include "logger.h"
#include "message.h"
#include "snake.h"
#include "space.h"
#include "utils.h"

void processKeyPressedAction(struct objectspace* space) {
    int keyPressed = editorReadKey(100);
    switch (keyPressed) {
        case ARROW_UP: {
            return changeSnakeDirection(space->sn, DIRECTION_UP);
        }
        case ARROW_DOWN: {
            return changeSnakeDirection(space->sn, DIRECTION_DOWN);
        }
        case ARROW_LEFT: {
            return changeSnakeDirection(space->sn, DIRECTION_LEFT);
        }
        case ARROW_RIGHT: {
            return changeSnakeDirection(space->sn, DIRECTION_RIGHT);
        }
        case EXIT: {
            pexit(0);
        }
    }
}

int assignNewFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;
    int occ_len = 0;
    info("Assigning new fruit for gameplay");
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, space->fruit) != 0) {
        free(occ);
        error("Assigning new fruit failled killing the process");
        die("assignNewFruit");
        return -1;
    }
    free(occ);
    info("New fruit assigned for gameplay at {%d, %d}", space->fruit->coordinate.x, space->fruit->coordinate.y);
    return 0;
}

int checkIfEachHimself(struct snake* sn) {
    struct snakenode* headenode = sn->headpos;
    if (!headenode || sn->state == DEAD) return 0;
    struct snakenode* nextnode = headenode->nextnode;
    while (nextnode) {
        if (nextnode->data.coordinate.x == headenode->data.coordinate.x && nextnode->data.coordinate.y == headenode->data.coordinate.y) {
            sn->state = DEAD;
            deleteSnakeSegment(sn, nextnode);
            setMessage(PROMPT_SNAKE_EAT_SELF);
            return 1;
        }
        nextnode = nextnode->nextnode;
    }
    return 0;
}

int checkIfSnakeHitBoundary(struct snake* sn, struct gameboard board) {
    if (sn->state == DEAD) return 1;
    struct coordinate head = sn->headpos->data.coordinate;
    if (head.x < 0 || head.y < 0 || head.x >= board.height || head.y >= board.width) {
        sn->state = DEAD;
        info("Snake hit the boundary killing snake & changing snake state to %d", sn->state);
        setMessage(PROMPT_HIT_BOUNDARY);
        return 1;
    }
    return 0;
}

void checkIfFruitConsumed(struct objectspace* space) {
    struct snake* sn = space->sn;
    struct fruit* ft = space->fruit;
    if (sameCordinates(sn->headpos->data.coordinate, ft->coordinate)) {
        struct snakepartdata part = sn->tail->data;
        appendSnakePart(sn, BODY_PART, part);
        info("Snake consumed the fruit at {%d, %d}", ft->coordinate.x, ft->coordinate.y);
        assignNewFruit(space);
    }
}

void moveSnake(struct objectspace* space) {
    switch (space->sn->state) {
        case ALIVE:
            checkIfFruitConsumed(space);
            processKeyPressedAction(space);
            checkIfSnakeHitBoundary(space->sn, space->board);
            moveBodyParts(space->sn);
            updatePositionWithDirection(space->sn);
            checkIfEachHimself(space->sn);
            break;
        case DEAD:
            removeSnakeSegment(space->sn);
            usleep(250000);
            break;
        default:
            die("moveSnake");
    }
}
