#include "terminal.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*
 * Return 0 when success -1 when failed
 * captures the cursor position by sending the escape sequence to terminal
 */
int getCursorPosition(int* rows, int* cols) {
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';
    // Checking if returned sequence is escape sequence or not
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    // returned size is format \x1b[H;W
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
    return 0;
}
/*
 * Return -1 if not able to get window size 0 incase of success
 */
int getWindowSize(int* rows, int* cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}
