#include "message.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "logger.h"
#include "utils.h"

promptmessage prompt;

void setMessage(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int needed = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    debug("Reuired size for fmt & ap %d", needed);
    emptyMessage();

    prompt.len = needed;
    int buffer_size = prompt.len + 2;

    prompt.msg = malloc(buffer_size * sizeof(char));
    if (prompt.msg == NULL) {
        error("Error while allocating memory to prompt message , killing the process");
        die("setMessage");
    }
    int written = vsnprintf(prompt.msg, buffer_size, fmt, ap);
    debug("Written %d bytes to prompt message", written);
    va_end(ap);
}

void emptyMessage() {
    if (prompt.msg) {
        free(prompt.msg);
        prompt.msg = NULL;
    }
    prompt.len = 0;
}
