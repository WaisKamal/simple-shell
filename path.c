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