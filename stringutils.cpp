#include "string_utils.h"

bool isValidEnvironmentVariableChar(char ch) {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch >= '9') || ch == '_');
}