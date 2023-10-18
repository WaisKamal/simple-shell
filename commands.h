#ifndef COMMANDS
#define COMMANDS

// Extracts command name from cmdString
char* getCommandName(char* cmdString);

// Removes extra spaces in cmdString
void removeExtraSpaces(char* cmdString);

// Returns an array of arguments from a command string
char** getArgs(char* cmdString);

#endif
