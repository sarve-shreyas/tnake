#include "gameplay.h"

#include "keyboard.h"
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
        case UNKNOWN_KEY:
            // do nothing
        default:
            // do nothing
    }
}
int assignNewFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;
    int occ_len = 0;
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, space->fruit) != 0) {
        free(occ);
        die("assignNewFruit");
        return -1;
    }
    free(occ);
    return 0;
}
int checkIfSnakeHitBoundary(struct snake* sn, struct gameboard board) {
    if (sn->state == DEAD) return;
    struct coordinate head = sn->headpos->data.coordinate;
    if (head.x < 0 || head.y < 0 || head.x >= board.height || head.y >= board.width) {
        sn->state = DEAD;
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
            updateSnakeState(space->sn);
            break;
        case DEAD:
            removeSnakeSegment(space->sn);
            break;
        default:
            die("moveSnake");
    }
}
