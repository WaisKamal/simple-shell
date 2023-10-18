#ifndef STRING_UTILS
#define STRING_UTILS

#include <stdbool.h>

// Checks whether character is valid for use in environment variable names
bool isValidEnvironmentVariableChar(char ch);

// Shifts string by specified amount
void shiftString(char* str, int from, int to, int shiftAmount);
#endif