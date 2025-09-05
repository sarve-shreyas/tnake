#ifndef H_PROMPT_MESSAGE
#define H_PROMPT_MESSAGE
#include <stdlib.h>

#include "space.h"

#define PROMPT_INIT_MESSAGE "Welcome 🎉"
#define PROMPT_WIN_MESSAFE "Winner Winner 🐍 Dinner"
#define PROMPT_SNAKE_EAT_SELF "Ops cannot eat 🐍"
#define PROMPT_HIT_BOUNDARY "Walls kill you stay awayyyyyy !!!"
#define PROMPT_CLOSE_CALL "Ops close call could have eaten self"
#define PROMPT_USERNAME_FORCE "Can't snake 🐍 around without name 👀"
#define PROMPT_SCREEN_DEFAULT_FOOTER "Press any key to continue..."
#define PROMPT_SCREEN_DEFAULT_TITLE "Message"

typedef struct {
    char* msg;
    int len;
    int state;
    int width;
    int referenceHeight;
    struct SpaceRepresentationStyle style;
    struct coordinate start_cordi;
    struct coordinate end_cordi;
} promptmessage;

extern promptmessage prompt;

typedef struct {
    char** megs;
    char* footer;
    char* title;
    int len;
} screenpromptmegs;

int initPromptMessage(int width, int referenceHeight, struct SpaceRepresentationStyle* style);
void setMessage(struct SpaceRepresentationStyle* style, int state, const char* fmt, ...);
void updatePromptMessageState(int state);
void emptyMessage();
char* promptUser(struct SpaceRepresentationStyle* style, const char* prompt, int need_data);
int screenPromptMessage(int len, char** msg, char* footer, char* title);
int getScreenpromptmegs(screenpromptmegs* megs);
#endif // H_PROMPT_MESSAGE
