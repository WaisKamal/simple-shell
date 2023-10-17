#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "path.h"
#include "env.h"

char* getCommandName(char* cmdString, int length) {
    // Copy cmdString because strtok mutates the passed string
    char* cmdStringCopy = malloc(length * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* result = malloc(sizeof commandName);
    strcpy(result, commandName);
    free(cmdStringCopy);
    return result;
}

// Builds the cwd linked list from the current directory string
void buildCwd(struct Path* cwd, char* cwdString) {
    // Copy cwdString
    char* cwdStringCopy = malloc(strlen(cwdString) * sizeof(char));
    strcpy(cwdStringCopy, cwdString);
    char* currentDirName = strtok(cwdStringCopy, "/");
    addDirectoryToPath(cwd, currentDirName);
    while (currentDirName != NULL) {
        currentDirName = strtok(NULL, "/");
        if (currentDirName != NULL) {
            addDirectoryToPath(cwd, currentDirName);
        }
    }
}

// Builds cwd string from Path* cwd struct;
char* buildCwdString(struct Path* cwd) {
    // Calculate the cwd string length
    struct Dir* currentDir = cwd->firstDir;
    int cwdStrLength = strlen(currentDir->dirName);
    while (currentDir->nextDir != NULL) {
        cwdStrLength += strlen(currentDir->nextDir->dirName) + 1;
        currentDir = currentDir->nextDir;
    }
    // Now build the cwd string
    char* cwdString = malloc(cwdStrLength * sizeof(char));
    currentDir = cwd->firstDir;
    strcpy(cwdString, currentDir->dirName);
    int currentIndex = strlen(currentDir->dirName);
    while (currentDir->nextDir != NULL) {
        strcpy(cwdString + currentIndex, "/");
        strcpy(cwdString + currentIndex + 1, currentDir->nextDir->dirName);
        currentIndex += strlen(currentDir->nextDir->dirName) + 1;
        currentDir = currentDir->nextDir;
    }
    return cwdString;
}

// Executes the cd command and returns the new cwd string
char* exec_cd(char* cmdString, struct Path* cwd) {
    char* cmdStringCopy = malloc(strlen(cmdString) * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* currentDirName = strtok(NULL, "/\n");
    do {
        if (strcmp(currentDirName, "..")) {
            addDirectoryToPath(cwd, currentDirName);
        } else {
            // Navigate back to the previous directory
            removeLastDirectory(cwd);
        }
        currentDirName = strtok(NULL, "/");
    } while (currentDirName != NULL);
    char* newCwdString = buildCwdString(cwd);
    return newCwdString;
}

int main(int argc, char** argv, char** env) {
    // Read environment variables
    struct EnvironmentVariables envVars;
    loadEnvironmentVariables(env, &envVars);

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
        fgets(cmdString, sizeof cmdString, stdin);
        // Remove trailing '\n' from command
        cmdString[strcspn(cmdString, "\n")] = 0;
        // strcpy(cmdString, "cd aaa/bbb/ccc/ddd");
        char* commandName = getCommandName(cmdString, sizeof cmdString);
        // Process command
        if (!strcmp(commandName, "cd")) {
            // Copy the new cwd string and deallocate the relevant memory block
            char* newCwdString = exec_cd(cmdString, cwd);
            cwdString = malloc(strlen(newCwdString) * sizeof(char));
            strcpy(cwdString, newCwdString);
            free(newCwdString);
        } else if (!strcmp(commandName, "setenv")) {
            exec_setenv(&envVars, cmdString);
        } else if (!strcmp(commandName, "unsetenv")) {
            exec_unsetenv(&envVars, cmdString);
        }
        printf("> %s> ", cwdString);
    }
    return 0;
}