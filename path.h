#ifndef PATH
#define PATH

#include <stdlib.h>

// Directives that are only available under UNIX
#ifndef WINDOWS
#include <dirent.h>
#endif

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

// Builds the cwd linked list from the current directory string
void buildCwd(struct Path* cwd, char* cwdString);

// Builds cwd string from Path* cwd struct;
char* buildCwdString(struct Path* cwd);

// Executes the cd command and returns the new cwd string
char* exec_cd(char* cmdString, struct Path* cwd);

#endif