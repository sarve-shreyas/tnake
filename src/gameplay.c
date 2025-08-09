#include "gameplay.h"

#include "keyboard.h"
#include "snake.h"
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
