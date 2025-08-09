#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
struct terminal {
    int row;
    int col;
};
int getWindowSize(int* rows, int* cols);
void enableRawMode(void);

extern struct termios orig_terminos;
#endif