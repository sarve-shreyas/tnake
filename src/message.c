#include "message.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "keyboard.h"
#include "logger.h"
#include "printter.h"
#include "screen.h"
#include "space.h"
#include "utils.h"

promptmessage prompt;
screenpromptmegs* userPromptMessages = NULL;

void setMessage(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int needed = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    emptyMessage();

    prompt.len = needed;
    int buffer_size = prompt.len + 2;

    prompt.msg = malloc(buffer_size * sizeof(char));
    if (prompt.msg == NULL) {
        error("Error while allocating memory to prompt message , killing the process");
        die("setMessage");
    }
    vsnprintf(prompt.msg, buffer_size, fmt, ap);
    va_end(ap);
    refreshPromptMessage();
}

void emptyMessage() {
    if (prompt.msg) {
        free(prompt.msg);
        prompt.msg = NULL;
    }
    prompt.len = 0;
}

void updatePromptMessageState(int state) {
    switch (state) {
        case ALIVE:
            debug("Updating prompt state to alive");
            prompt.state = ALIVE;
            break;
        case DEAD:
            debug("Updating prompt state to dead");
            prompt.state = DEAD;
            break;
        default:
            debug("Invalid message state");
            break;
    }
}

char* returnValueWithResetState(char* buf) {
    changeCusrsorState(DEAD);
    updatePromptMessageState(ALIVE);
    return buf;
}

char* promptUser(const char* prompt, int need_data) {
    size_t bufsize = 128;
    char* buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';
    if (need_data) changeCusrsorState(ALIVE);
    updatePromptMessageState(DEAD);
    while (1) {
        setMessage(prompt, buf);
        refreshPromptMessage();

        int c = editorReadKeyRaw(1000000);
        if (c == EXIT) {
            debug("Exiting from process");
            pexit(0);
        }
        if (!need_data) {
            setMessage("");
            free(buf);
            return returnValueWithResetState(NULL);
        }
        if (c == DELETE_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
            if (buflen != 0) buf[--buflen] = '\0';
        } else if (c == ESCAPE) {
            setMessage("");
            free(buf);
            return returnValueWithResetState(NULL);
        } else if (c == '\r') {
            if (buflen != 0) {
                setMessage("");
                return returnValueWithResetState(buf);
            }
        } else if (!iscntrl(c) && c < 128) {
            if (buflen == bufsize - 1) {
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }
    }
    returnValueWithResetState(NULL);
}

void resetScreenpromptmegs() {
    if (userPromptMessages != NULL) {
        free(userPromptMessages);
        userPromptMessages = NULL;
    }
}

void setScreenpromptmegs(int len, char** msg, char* footer, char* title) {
    resetScreenpromptmegs();
    userPromptMessages = malloc(sizeof(screenpromptmegs));
    char* defaultFooter = PROMPT_SCREEN_DEFAULT_FOOTER;
    char* defaultTitle = PROMPT_SCREEN_DEFAULT_TITLE;
    userPromptMessages->megs = msg;
    userPromptMessages->len = len;
    userPromptMessages->footer = footer == NULL ? defaultFooter : footer;
    userPromptMessages->title = title == NULL ? defaultTitle : title;
}

int screenPromptMessage(int len, char** msgs, char* footer, char* title) {
    setScreenpromptmegs(len, msgs, footer, title);
    while (1) {
        refreshScreenPromptMessageScreen();
        int key = editorReadKeyRaw(10000);
        if (key == EXIT) {
            info("Pressed exit key killing process");
            resetScreenpromptmegs();
            pexit(0);
            return EXIT;
        } else if (key != KEY_TIMEOUT) {
            resetScreenpromptmegs();
            return key;
        }
    }
    return -1;
}

int getScreenpromptmegs(screenpromptmegs* megs) {
    if (userPromptMessages != NULL) {
        *megs = *userPromptMessages;
        return 0;
    }
    megs = NULL;
    return 1;
}
