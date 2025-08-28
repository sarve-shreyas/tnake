#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abuffer.h"
#include "terminal.h"

void die(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    pexit(1);
}

void pexit(int code) {
    struct abuf ab = ABUF_INIT;
    abAppend(&ab, "\x1b[H", 3);
    abAppend(&ab, "\x1b[?25h", 6);
    abFlush(&ab);
    disableRawMode();
    return exit(code);
}

char* get_system_time() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char* buffer = malloc(100 * sizeof(char));
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", local);
    return buffer;
}
