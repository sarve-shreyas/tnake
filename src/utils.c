#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "abuffer.h"
#include "printter.h"
#include "terminal.h"

void die(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    pexit(1);
}

void pexit(int code) {
    struct abuf ab = ABUF_INIT;
    changeCursorStateToBuf(&ab, ALIVE);
    clearScreen(&ab);
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

void stringAppend(int len, char* buf, ...) {
    va_list args;
    va_start(args, buf);
    buf[0] = '\0';
    for (int i = 0; i < len; i++) {
        const char* s = va_arg(args, const char*);
        strcat(buf, s);
    }

    va_end(args);
}
