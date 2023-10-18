#include <stdlib.h>
#include <string.h>
#include "path.h"

// Adds a directory to the end of the path
void addDirectoryToPath(struct Path* path, char* dirName) {
    if (path->firstDir != NULL) {
        path->lastDir->nextDir = malloc(sizeof(struct Dir));
        path->lastDir->nextDir->dirName = malloc(strlen(dirName) * sizeof(char));
        strcpy(path->lastDir->nextDir->dirName, dirName);
        path->lastDir->nextDir->prevDir = path->lastDir;
        path->lastDir = path->lastDir->nextDir;
        path->lastDir->nextDir = NULL;
    } else {
        path->firstDir = malloc(sizeof(struct Dir));
        path->firstDir->dirName = malloc((strlen(dirName) + 1) * sizeof(char));
        strcpy(path->firstDir->dirName, dirName);
        path->lastDir = path->firstDir;
        path->lastDir->nextDir = NULL;
    }
}

void removeLastDirectory(struct Path* path) {
    // Reassign pointer values
    path->lastDir = path->lastDir->prevDir;
    free(path->lastDir->nextDir->dirName);
    free(path->lastDir->nextDir);
    path->lastDir->nextDir = NULL;
}

// Builds the cwd linked list from the current directory string
void buildCwd(struct Path* cwd, char* cwdString) {
    // Copy cwdString
    char* cwdStringCopy = malloc((strlen(cwdString) + 1) * sizeof(char));
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
    char* cwdString = malloc((cwdStrLength + 1) * sizeof(char));
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
    char* cmdStringCopy = malloc((strlen(cmdString) + 1) * sizeof(char));
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
    free(cmdStringCopy);
    return newCwdString;
}