#ifndef H_PROMPT_MESSAGE
#define H_PROMPT_MESSAGE
#include <stdlib.h>

#define PROMPT_INIT_MESSAGE "Welcome 🎉"
#define PROMPT_WIN_MESSAFE "Winner Winner 🐍 Dinner"
#define PROMPT_SNAKE_EAT_SELF "Ops cannot eat 🐍"
#define PROMPT_HIT_BOUNDARY "Walls kill you stay awayyyyyy !!!"
#define PROMPT_CLOSE_CALL "Ops close call could have eaten self"
#define PROMPT_USERNAME_FORCE "Cant snake 🐍 around without name 👀"

typedef struct {
    char* msg;
    int len;
    int state;
} promptmessage;

extern promptmessage prompt;

void setMessage(const char* fmt, ...);
void updatePromptMessageState(int state);
void emptyMessage();
char* promptUser(const char* prompt, int need_data);
#endif