#include "string_utils.h"

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