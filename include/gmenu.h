#ifndef MENU_H
#define MENU_H

#define MENU_SCREEN_INFO_MSG "Use Up/Down and press Space/Enter to select."
typedef struct {
    const char* title;
    char** options;
    int len;
    int selected;
} menudata;


/**
 * @brief Sets up menu data and returns operation status.
 * @param m Pointer to menudata structure to be initialized
 * @return {int} 0 on success, non-zero on error
 *
 * This function initializes the provided menu data structure
 * and sets up the menu options. Returns 0 when the operation
 * succeeds, non-zero value indicates an error occurred.
 */
int getmenu(menudata* m);

/**
 * Sets menu data and handles the menu screen interaction.
 *
 * This function displays a menu interface and processes user input to select
 * an option from the menu. It also manages menu data initialization.
 *
 * @return The zero-based index of the selected menu option.
 *         Returns a negative value in case of any errors.
 */
int drawmenu(menudata m);
#endif  // MENU_H
