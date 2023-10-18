#ifndef STRING_UTILS
#define STRING_UTILS

#include <stdbool.h>

// Checks whether character matches [a-zA-z0-9_]
bool isValidEnvironmentVariableChar(char ch);

// Shifts string by specified amount
void shiftString(char* str, int from, int to, int shiftAmount);

// Returns a dynamically allocated substring from the given strign
char* substr(char* str, int from, int to);

// Removes leading and trailing whitespace from string
char* trim(char* str);

#endif
