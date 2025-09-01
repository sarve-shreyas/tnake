#include "keyboard.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "logger.h"
#include "utils.h"

/**
 * Waits briefly (5 milliseconds) to check if there is input available on stdin.
 *
 * Uses the select() system call to monitor stdin for readability.
 * Returns a non-zero value if input is available, zero otherwise.
 * If select() fails for reasons other than being interrupted by a signal (EINTR),
 * calls die("select") to handle the error.
 *
 * @return int Non-zero if input is available on stdin, zero otherwise.
 */
int wait_more(void) {
    fd_set s;
    FD_ZERO(&s);
    FD_SET(STDIN_FILENO, &s);
    struct timeval tiny = {0, 5000};  // 5 ms
    int r = select(STDIN_FILENO + 1, &s, NULL, NULL, &tiny);
    if (r == -1 && errno != EINTR) die("select");
    return r > 0;
}

/**
 * Reads a key press from the keyboard with an optional timeout.
 *
 * @param timeout_ms The timeout in milliseconds to wait for a key press. If zero or negative, waits indefinitely.
 * @return The integer value of the key pressed, or -1 if the timeout expires without a key press.
 */
int editorReadKey(int timeout_ms) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int ready = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
    if (ready == -1) {
        if (errno == EINTR) return KEY_TIMEOUT;
        die("select");
    }
    if (ready == 0) return KEY_TIMEOUT;

    char c;
    int nread;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno == EAGAIN) return KEY_TIMEOUT;
        if (nread == -1 && errno == EINTR) continue;
        die("read");
    }

    if (c == '\x1b') {
        char seq[3];

        if (!wait_more() || read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (!wait_more() || read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (!wait_more() || read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    return UNKNOWN_KEY;
                }
            } else {
                switch (seq[1]) {
                    case 'A':
                        return ARROW_UP;
                    case 'B':
                        return ARROW_DOWN;
                    case 'C':
                        return ARROW_RIGHT;
                    case 'D':
                        return ARROW_LEFT;
                    default:
                        return UNKNOWN_KEY;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H':
                    return UNKNOWN_KEY;
                case 'F':
                    return UNKNOWN_KEY;
            }
        }
        return EXIT;
    }
    switch (c) {
        case 'W':
        case 'w':
            return ARROW_UP;
        case 'D':
        case 'd':
            return ARROW_RIGHT;
        case 'S':
        case 's':
            return ARROW_DOWN;
        case 'A':
        case 'a':
            return ARROW_LEFT;
        case ' ':
            return SPACEBAR;
        case '\r':
            return ENTER_KEY;
        case CTRL_KEY('Q'):
        case CTRL_KEY('C'):
            return EXIT;
        default:
            return c;
    }
}

int editorReadKeyRaw(int timeout_ms) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int ready = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
    if (ready == -1) {
        if (errno == EINTR) return KEY_TIMEOUT;
        die("select");
    }
    if (ready == 0) return KEY_TIMEOUT;

    char c;
    int nread;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno == EAGAIN) return KEY_TIMEOUT;
        if (nread == -1 && errno == EINTR) continue;
        die("read");
    }

    if (c == '\x1b') {
        char seq[3];

        if (!wait_more() || read(STDIN_FILENO, &seq[0], 1) != 1) return ESCAPE;
        if (!wait_more() || read(STDIN_FILENO, &seq[1], 1) != 1) return ESCAPE;

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (!wait_more() || read(STDIN_FILENO, &seq[2], 1) != 1) return ESCAPE;
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '3':
                            return DELETE_KEY;
                        default:
                            return UNKNOWN_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A':
                        return ARROW_UP;
                    case 'B':
                        return ARROW_DOWN;
                    case 'C':
                        return ARROW_RIGHT;
                    case 'D':
                        return ARROW_LEFT;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H':
                    return UNKNOWN_KEY;
                case 'F':
                    return UNKNOWN_KEY;
            }
        }
        return ESCAPE;
    }

    switch (c) {
        case CTRL_KEY('Q'):
        case CTRL_KEY('C'):
            return EXIT;
    }
    return c;
}
