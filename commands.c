#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "string_utils.h"

// Extracts command name from cmdString
char* getCommandName(char* cmdString) {
    // Copy cmdString because strtok mutates the passed string
    char* cmdStringCopy = malloc((strlen(cmdString) + 1) * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* result = malloc(sizeof commandName);
    strcpy(result, commandName);
    free(cmdStringCopy);
    return result;
}

// Removes extra spaces in cmdString
void removeExtraSpaces(char* cmdString) {
    char* newCmdString = malloc((strlen(cmdString) + 1) * sizeof(char));
    bool inQuote = false;
    int ptr = 0;
    for (int i = 0; i < strlen(cmdString); i++) {
        if (cmdString[i] == '"') {
            inQuote = !inQuote;
        } else if (cmdString[i] == ' ') {
            if (!inQuote && (i == 0 || cmdString[i - 1] == ' ')) {
                continue;
            }
        }
        newCmdString[ptr++] = cmdString[i];
    }
    newCmdString[ptr] = '\0';
    strcpy(cmdString, newCmdString);
}

// Returns an array of arguments from a command string
char** getArgs(char* cmdString) {
    int argCount = 0;
    int quotePos = -1;
    int argStartPos = -1;
    // Count arguments
    for (int i = 0; i < strlen(cmdString); i++) {
        if (cmdString[i] == ' ') {
            if (quotePos == -1 && argStartPos != -1) {
                // Argument found in [argStartPos, i - 1]
                argCount++;
                argStartPos = quotePos = -1;
            }
        } else if (cmdString[i] == '"' && (i == 0 || cmdString[i - 1] != '\\')) {
            if (quotePos == -1) {
                quotePos = argStartPos = i;
            } else {
                // Argument found in [quotePos + 1, i - 1]
                argCount++;
                quotePos = argStartPos = -1;
            }
        } else if (i == strlen(cmdString) - 1) {
            if (argStartPos != -1) {
                // Argument found in [argStartPos, i]
                argCount++;
                quotePos = argStartPos = -1;
            }
        } else {
            if (argStartPos == -1) {
                argStartPos = i;
            }
        }
    }
    // Allocate argument array
    char** args = malloc((argCount + 1) * sizeof(char*));
    args[argCount] = (char* )0;
    quotePos = argStartPos = -1;
    argCount = 0;
    // Extract arguments
    for (int i = 0; i < strlen(cmdString); i++) {
        if (cmdString[i] == ' ') {
            if (quotePos == -1 && argStartPos != -1) {
                // Argument found in [argStartPos, i - 1]
                args[argCount] = substr(cmdString, argStartPos, i - 1);
                argCount++;
                argStartPos = quotePos = -1;
            }
        } else if (cmdString[i] == '"' && (i == 0 || cmdString[i - 1] != '\\')) {
            if (quotePos == -1) {
                quotePos = i;
                argStartPos = i + 1;
            } else {
                // Argument found in [quotePos + 1, i - 1]
                args[argCount] = substr(cmdString, quotePos + 1, i - 1);
                argCount++;
                quotePos = argStartPos = -1;
            }
        } else if (i == strlen(cmdString) - 1) {
            if (argStartPos != -1) {
                // Argument found in [argStartPos, i]
                args[argCount] = substr(cmdString, argStartPos, i);
                argCount++;
                quotePos = argStartPos = -1;
            }
        } else {
            if (argStartPos == -1) {
                argStartPos = i;
            }
        }
    }
    return args;
}