#ifndef PATH
#define PATH

#include <stdlib.h>

struct Path {
    struct Dir* firstDir;
    struct Dir* lastDir;
};

struct Dir {
    char* dirName;
    struct Dir* prevDir;
    struct Dir* nextDir;
};

// Adds a directory to the end of the path
void addDirectoryToPath(struct Path* path, char* dirName);

// Removes the last directory in the path
void removeLastDirectory(struct Path* path);

#endif