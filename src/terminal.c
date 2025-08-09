#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"
struct termios orig_terminos;
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
    *cols = 182;
    *rows = 49;
    return 0;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void disableRawMode(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_terminos) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode(void) {
    /*
     * `tcgetattr` -> current terminal flags
     */
    if (tcgetattr(STDIN_FILENO, &orig_terminos) == -1) {
        die("tcgetattr");
    }
    atexit(disableRawMode);
    struct termios raw = orig_terminos;
    /* Input processing flags
     *  Ctrl + S stops data from being transmitted to terminal, Ctrl + Q enables the data transmission to terminal to disable this flags we can use flag `IXON`
     *  Ctrl + M -> terminal translates any carriage return `\r` (13, `\r`) as newline (10, `\n`) we can stop this processing using flag `ICRNL`
     *  `BRKINT` -> a break condition will cause `SIGINT` signal to be sent to program, pressing Ctrl+C.
     *  `INPCK` -> enables parity checking
     *  `ISTRIP` -> causes 8th bit of each input byte to be stripped, it will set it to 0.
     */
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);  // input flags
    /* Local Flags
     * `ECHO` -> flag is responsible for echoing what key has been pressed
     * `ICANON` -> cannonical mode flag
     *  Ctrl + C sends signal `SIGINT` which causes process to terminal , Ctrl + Z sends signal `SIGTSTP` which causes process to suspend to turn of this both processes to turn this of we can turn of flag ISIG this unables application to read `Ctrl + [C|Z]`
     *  Ctrl + V enables us to send anything when typed after `Ctrl + V` then any singal will be captured as byte from ternimal to diable this we can use flag `IEXTEN`
     */
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);  // local flags
    /* Output Processing Flags
     * terminal translates `\n` into carriage return followed by newline (`\r\n`). the carriage return moves cursor to the begining of current line & newline moves cursor to nextline, also scrolls if required. we can use flag `OPOST` to diable this.
     */
    raw.c_oflag &= ~(OPOST);
    /* Control Flags
     * `CS8` -> it sets character size to 8bits per byte.
     */
    raw.c_cflag |= (CS8);

    /* Control Character
     * `VMIN` -> minimum number of bytes if unput needed before read() can return
     * `VTIME` -> maximum amount of time to wait before read() return, if timesout then read() will return 0 which means 0 bytes read.
     */
    raw.c_cc[VMIN] = 0;   // Return as soon as there is there is any input to be read.
    raw.c_cc[VTIME] = 1;  // 1/10 of the second or 100 milliseconds.
    /*
     * `tcsetarrt` -> set terminal flags to diable this.
     * `TCSAFLUSH` specifies the changes to be applied after all outputs are writtern to terminal & discard all input thet hasn't been read
     */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}
