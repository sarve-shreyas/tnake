#ifndef UTILS_H
#define UTILS_H

// Utility function to print error and exit
void die(const char* message);
void pexit(int code);
char* get_system_time();
void stringAppend(int len, char* buf, ...);
#endif  // UTILS_H
