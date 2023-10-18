#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "variable_store.h"
#include "string_utils.h"
#include "path.h"
#include "env.h"
#include "alias.h"

// UNIX-specific directives
#ifndef WINDOWS
#include <unistd.h>
#endif

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

int main(int argc, char** argv, char** env) {
    // Read environment variables
    struct VariableStore envVars;
    loadEnvironmentVariables(env, &envVars);

    // Initialize alias store
    struct VariableStore aliasStore;
    aliasStore.count = 0;

    // To be replaced with POSIX getcwd()
    char* cwdString = "C:/Users/Wais/Desktop/simple-shell";

    // Initialize cwd
    struct Path cwd;
    cwd.firstDir = cwd.lastDir = NULL;
    buildCwd(&cwd, "C:/Users/Wais/Desktop/simple-shell");

    // Initialize command string with maximum 1024 characters
    char cmdString[1024];

    // Print the starting prompt
    printf("> %s> ", cwdString);

    while (true) {
        // Read command from stdin
        fgets(cmdString, sizeof cmdString, stdin);
        // Remove trailing '\n' from command
        cmdString[strcspn(cmdString, "\n")] = 0;
        // Remove extra spaces
        removeExtraSpaces(cmdString);
        // Evaluate alias if found
        evaluateAlias(&aliasStore, cmdString);
        // Replace environment variables in cmdString
        evaluateEnvironmentVariables(&envVars, cmdString);
        // strcpy(cmdString, "cd aaa/bbb/ccc/ddd");
        char* commandName = getCommandName(cmdString);
        // Process command
        if (!strcmp(commandName, "cd")) {
            // Copy the new cwd string and deallocate the relevant memory block
            char* cdResult = exec_cd(cmdString, &cwd);
            if (strlen(cdResult) > 0) {
                char* newCwdString = cdResult;
                cwdString = malloc((strlen(newCwdString) + 1) * sizeof(char));
                strcpy(cwdString, newCwdString);
                free(newCwdString);
            }
        } else if (!strcmp(commandName, "env") || !strcmp(commandName, "setenv")) {
            exec_setenv(&envVars, cmdString);
        } else if (!strcmp(commandName, "unsetenv")) {
            exec_unsetenv(&envVars, cmdString);
        } else if (!strcmp(commandName, "alias")) {
            exec_alias(&aliasStore, cmdString);
        } else if (!strcmp(commandName, "exit")) {
            return 0;
        } else {
            #ifndef WINDOWS
            char** args = getArgs(cmdString);
            pid_t pid = fork();
            if (pid == 0) {
                execv(commandName, args);
                exit(127);
            } else {
                waitpid(pid, 0, 0);
            }
            #endif
        }
        printf("> %s> ", cwdString);
    }
    return 0;
}