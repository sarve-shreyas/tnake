#include "message.h"

#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "utils.h"

promptmessage prompt;

void setMessage(const char* msg) {
    emptyMessage();
    prompt.len = strlen(msg);
    int buffer_size = prompt.len + 2;

    prompt.msg = malloc(buffer_size * sizeof(char));
    if (prompt.msg == NULL) {
        error("Error while allocating memory to prompt message , killing the process");
        die("setMessage");
    }
    int written = snprintf(prompt.msg, buffer_size, "%s", msg);
    debug("Written %d bytes to prompt message", written);
}

void emptyMessage() {
    if (prompt.msg) {
        free(prompt.msg);
        prompt.msg = NULL;
    }
    prompt.len = 0;
}
