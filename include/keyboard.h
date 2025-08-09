#ifndef H_KEYBOARD
#define H_KEYBOARD

/*
 * Reading key pressed by user in raw mode
 * wait till user enter someting from keyboard
 * exit the process if there is some error except EAGAIN error
 */
enum SupportedKeys {
    ARROW_UP,
    ARROW_RIGHT,
    ARROW_DOWN,
    ARROW_LEFT,
    EXIT,
    UNKNOWN_KEY = -1,
    KEY_TIMEOUT = -2
};
int editorReadKey(int timeout_ms);

#endif
