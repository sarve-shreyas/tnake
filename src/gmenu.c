#include "gmenu.h"

#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "screen.h"
#include "utils.h"

static menudata* tmenu = NULL;

void setmenu(const menudata* m) {
    if (tmenu != NULL) {
        free(tmenu);
    }
    tmenu = malloc(sizeof(menudata));
    *tmenu = *m;
}

void resetmenu() {
    if (tmenu != NULL) {
        free(tmenu);
        tmenu = NULL;
    }
}

int getmenu(menudata* m) {
    if (tmenu == NULL) {
        m = NULL;
        return -1;
    }
    *m = *tmenu;
    return 0;
}

int returnWithIndex(int index) {
    resetmenu();
    return index;
}
/**
 * Draws and handles the menu interface.
 *
 * This function creates an interactive menu where users can navigate using arrow keys
 * and make selections using space or enter. The menu continues to run until a selection
 * is made or the exit command is given.
 *
 * @param m The menu data structure containing menu information
 *
 * @return The index of the selected menu item if a selection was made,
 *         -1 if the menu was exited or an error occurred
 *
 * Key controls:
 * - SPACEBAR/ENTER: Select current menu item
 * - ARROW_UP: Move selection up
 * - ARROW_DOWN: Move selection down
 * - EXIT: Exit the menu
 */
int drawmenu(menudata m) {
    setmenu(&m);
    tmenu->selected = 0;
    while (1) {
        refreshMenuScreen();
        int key = editorReadKey(10000);
        switch (key) {
            case SPACEBAR:
            case ENTER_KEY:
                return returnWithIndex(tmenu->selected);
            case ARROW_UP:
                tmenu->selected = (tmenu->selected - 1 + tmenu->len) % tmenu->len;
                break;
            case ARROW_DOWN:
                tmenu->selected = (tmenu->selected + 1) % tmenu->len;
                break;
            case EXIT:
                pexit(0);
                return returnWithIndex(-1);
        }
    }
    return returnWithIndex(-1);
}
