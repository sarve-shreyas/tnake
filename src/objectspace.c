#include "objectspace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "digitaldisplay.h"
#include "fruit.h"
#include "logger.h"
#include "message.h"
#include "snake.h"
#include "styles.h"
#include "utils.h"

int initFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;

    int occ_len = 0;
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    struct fruit* ft = malloc(sizeof *ft);
    if (!ft) {
        free(occ);
        return -1;
    }
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, ft) != 0) {
        free(occ);
        free(ft);
        return -1;
    }
    space->fruit = ft;
    free(occ);
    return 0;
}

int initMessagePrompt() {
    prompt.msg = NULL;
    char* welcomeMessage = PROMPT_INIT_MESSAGE;
    setMessage(welcomeMessage);
    return 0;
}

int initObjectSpace(struct objectspace* space, struct terminal termi, objectspaceconfigs configs) {
    struct snake* sn = malloc(sizeof(struct snake));
    struct gameboard board;

    if (configureSnake(configs.snake_init_len, sn) != 0) {
        error("[configureSnake] Error while configuring snake returning -1");
        return -1;
    }
    if (configureGameBoard(configs.gameboard_height, configs.gameboard_width, &board, termi) != 0) {
        error("[configureGameBoard] Error while configuring gameboard returning -1");
        return -1;
    };
    space->board = board;
    space->sn = sn;
    if (initFruit(space) != 0) {
        error("[initFruit] Error while initialising fruit returning -1");
        return -1;
    }
    if (initMessagePrompt() != 0) {
        error("[initMessagePrompt] Error while initialising message prompt returning -1");
        return -1;
    }
    info("Initialising objectspace success");
    return 0;
}

int getObjectOnBoard(int i, int j, struct snake* sn, struct fruit* ft) {
    int snakepart = BOARD_BLOCK;
    struct snakenode* parts = sn->headpos;
    while (parts) {
        if (parts->data.coordinate.x == i && parts->data.coordinate.y == j) {
            snakepart = parts->type;
            break;
        }
        parts = parts->nextnode;
    }
    if (snakepart == BOARD_BLOCK) {
        if (ft->coordinate.x == i && ft->coordinate.y == j) {
            return FRUIT;
        }
    }
    return snakepart;
}

void clearScreen(struct abuf* ab) {
    abAppend(ab, "\x1b[H", 3);
}

void clearRow(struct abuf* ab, int row) {
    char buffer[20];
    int written = snprintf(buffer, 20, "\x1b[%d;%dH\x1b[2K", row, 1);
    abAppend(ab, buffer, written);
}

void printStyleAt(struct abuf* ab, int x, int y, struct SpaceRepresentationStyle style) {
    char buf[200];
    int bytes = snprintf(buf, 200, "\x1b[%d;%dH%s", x, y, colorStr(style.str, style.bgcolor, style.fgcolor));
    abAppend(ab, buf, bytes);
}

void printStringAt(struct abuf* ab, int x, int y, char* str) {
    char buf[200];
    int bytes = snprintf(buf, 200, "\x1b[%d;%dH%s", x, y, str);
    abAppend(ab, buf, bytes);
}

void printGameboard(struct abuf* ab, struct objectspace space) {
    struct gameboard board = space.board;
    for (int i = board.top_left.x - 1; i <= board.bottom_right.x + 1; i++) {
        for (int j = board.top_left.y - 1; j <= board.bottom_right.y + 1; j++) {
            int objectType = NONE;

            if (getBoardBoundaryType(i, j, board) != NO_BOUNDARY) {
                objectType = BOARD_BOUNDRY;
            } else {
                objectType = getObjectOnBoard((i - board.top_left.x), (j - board.top_left.y) / 2, space.sn, space.fruit);
            }
            struct SpaceRepresentationStyle style = objectType == BOARD_BOUNDRY ? getBoundrySpaceStyle(getBoardBoundaryType(i, j, board)) : getRepresentationStyle(objectType);
            printStyleAt(ab, i, j, style);
            j += (style.len - 1);
        }
    }
}

void printDigit(struct abuf* ab, int* x, int* y, char digit) {
    int r = 0;
    int c = 0;
    int old_x = *x;
    int** arr = get_digi_repr(digit, &r, &c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int repr = arr[i][j];
            struct SpaceRepresentationStyle style = (repr) ? digital_display_set_style : digital_display_uset_style;
            printStyleAt(ab, *x + i, *y + style.len * j, style);
        }
    }
    *x = old_x;
    *y += c + 1;
}

void printScore(struct abuf* ab, struct objectspace space) {
    struct gameboard board = space.board;
    int score = space.sn->len;
    char buffer[100];
    int written = snprintf(buffer, 100, "SCORE - %d", score);
    int x = board.bottom_left.x + 2;
    int y = board.bottom_left.y + 4;
    for (int i = 0; i < written; i++) {
        printDigit(ab, &x, &y, buffer[i]);
    }
}

void printPromptMessage(struct abuf* ab, struct objectspace space) {
    static struct coordinate* cordi = NULL;

    int x = space.board.top_left.x - 2;
    int startY = space.board.top_left.y - 1;
    int endY = space.board.top_right.y + 1;
    int startPromptY = cordi == NULL ? -1 : cordi->y;
    int messagePromptUIWidth = getDisplayWidth(prompt.msg, prompt.len);
    int endPromptY = cordi == NULL ? -1 : cordi->y + messagePromptUIWidth;

    debug("MessageUI widht %d", messagePromptUIWidth);
    debug("{start, end} - {%d, %d}", startPromptY, endPromptY);
    if (cordi == NULL) {
        cordi = malloc(sizeof(struct coordinate));
        cordi->x = x;
        cordi->y = startY + 1;
    } else if (endPromptY == (endY - 1)) {
        cordi->y = startY + 1;
    } else {
        cordi->y += 1;
    }
    clearRow(ab, x);
    printStringAt(ab, x, startY, "[");
    printStringAt(ab, x, endY, "]");
    printStringAt(ab, cordi->x, cordi->y, prompt.msg);
}

void printObjectSpace(struct abuf* ab, struct terminal _, struct objectspace objspace) {
    clearScreen(ab);
    printGameboard(ab, objspace);
    printScore(ab, objspace);
    printPromptMessage(ab, objspace);
}
