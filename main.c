#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "variable_store.h"
#include "string_utils.h"
#include "path.h"
#include "env.h"
#include "alias.h"

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
    struct Path* cwd;
    cwd->firstDir = cwd->lastDir = NULL;
    buildCwd(cwd, "C:/Users/Wais/Desktop/simple-shell");

    // Initialize command string with maximum 1024 characters
    char cmdString[1024];

    // Print the starting prompt
    printf("> %s> ", cwdString);

    while (true) {
        // Read command from stdin
        fgets(cmdString, sizeof cmdString, stdin);
        // Remove trailing '\n' from command
        cmdString[strcspn(cmdString, "\n")] = 0;
        // Evaluate alias if found
        evaluateAlias(&aliasStore, cmdString);
        // Replace environment variables in cmdString
        evaluateEnvironmentVariables(&envVars, cmdString);
        // strcpy(cmdString, "cd aaa/bbb/ccc/ddd");
        char* commandName = getCommandName(cmdString);
        // Process command
        if (!strcmp(commandName, "cd")) {
            // Copy the new cwd string and deallocate the relevant memory block
            char* newCwdString = exec_cd(cmdString, cwd);
            cwdString = malloc((strlen(newCwdString) + 1) * sizeof(char));
            strcpy(cwdString, newCwdString);
            free(newCwdString);
        } else if (!strcmp(commandName, "env") || !strcmp(commandName, "setenv")) {
            exec_setenv(&envVars, cmdString);
        } else if (!strcmp(commandName, "unsetenv")) {
            exec_unsetenv(&envVars, cmdString);
        } else if (!strcmp(commandName, "alias")) {
            exec_alias(&aliasStore, cmdString);
        } else if (!strcmp(commandName, "exit")) {
            return 0;
        }
        printf("> %s> ", cwdString);
    }
    return 0;
}