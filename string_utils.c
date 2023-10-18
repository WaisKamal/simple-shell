#include <string.h>
#include <stdlib.h>
#include "string_utils.h"

// Checks whether character matches [a-zA-z0-9_]
bool isValidEnvironmentVariableChar(char ch) {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch >= '9') || ch == '_');
}

// Shifts string by specified amount
void shiftString(char* str, int from, int to, int shiftAmount) {
    if (shiftAmount > 0) {
        for (int i = to; i >= from; i--) {
            str[i + shiftAmount] = str[i];
        }
    } else {
        for (int i = from; i <= to; i++) {
            str[i + shiftAmount] = str[i];
        }
    }
}

// Returns a dynamically allocated substring from the given string
char* substr(char* str, int from, int to) {
    char* newStr = malloc(to - from + 2);
    for (int i = 0; i <= to - from + 1; i++) {
        newStr[i] = str[i + from];
    }
    newStr[to + 1] = '\0';
    return newStr;
}

// Removes leading and trailing whitespace from string
char* trim(char* str) {
    int from = 0;
    int to = strlen(str) - 1;
    while (str[from] == ' ') from++;
    while (str[to] == ' ') to--;
    return substr(str, from, to);
}