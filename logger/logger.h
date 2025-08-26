#ifndef H_LOGGER_MODULE
#define H_LOGGER_MODULE

#include <stdio.h>
#include <stdarg.h>

enum LogLevels {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
};

struct loggerinformation {
    int log_level;
    char* timestamp;
    char* msg;
    va_list args;
};
void log_message(int log_level, char* msg, ...);
void info(char* msg, ...);
void error(char* msg, ...);
void warn(char* msg, ...);
void debug(char* msg, ...);

extern FILE* logger_file;

#endif