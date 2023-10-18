#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "alias.h"
#include "variable_store.h"
#include "path.h"
#include "env.h"

// UNIX-specific directives
#ifndef WINDOWS
#include <unistd.h>
#include <sys/wait.h>
#endif

int main(int argc, char** argv, char** env) {
    // Read environment variables
    struct VariableStore envVars;
    loadEnvironmentVariables(env, &envVars);

    // Initialize alias store
    struct VariableStore aliasStore;
    aliasStore.count = 0;

    // To be replaced with POSIX getcwd()
    char* cwdString = malloc(1024 * sizeof(char));
    #ifndef WINDOWS
    getcwd(cwdString, 1024);
    #endif

    // Initialize cwd
    struct Path cwd;
    cwd.firstDir = cwd.lastDir = NULL;
    buildCwd(&cwd, cwdString);

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
                char* chdirArg = malloc((strlen(cwdString) + 2) * sizeof(char));
                chdirArg[0] = '/';
                strcpy(chdirArg + 1, cwdString);
                chdir(chdirArg);
                execvp(commandName, args);
                free(chdirArg);
                exit(127);
            } else {
                void exitChild() {
                    kill(pid, 9);
                }
                void exitParent() {
                    kill(0, 9);
                }
                signal(SIGINT, exitChild);
                waitpid(pid, 0, 0);
                signal(SIGINT, exitParent);
            }
            #endif
        }
        printf("> %s> ", cwdString);
    }
    return 0;
}
