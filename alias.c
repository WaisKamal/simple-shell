#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "variable_store.h"
#include "string_utils.h"
#include "alias.h"

void setAlias(struct VariableStore* aliasStore, char* name, char* value) {
    setVariable(aliasStore, name, value);
}

void removeAlias(struct VariableStore* aliasStore, char* name) {
    removeVariable(aliasStore, name);
}

char* getAlias(struct VariableStore* aliasStore, char* name) {
    return getVariable(aliasStore, name);
}

void exec_alias(struct VariableStore* aliasStore, char* cmdString) {
    char* cmdStringCopy = malloc((strlen(cmdString) + 1) * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* varName = strtok(NULL, "=");
    char* varValue = strtok(NULL, "=");
    setAlias(aliasStore, varName, varValue);
}

// Evaluates alias in cmdString if found
void evaluateAlias(struct VariableStore* aliasStore, char* cmdString) {
    int firstNonSpaceChar = -1;
    for (int i = 0; i < strlen(cmdString); i++) {
        if (cmdString[i] != ' ' && firstNonSpaceChar == -1) {
            firstNonSpaceChar = i;
        } else if ((cmdString[i] == ' ' || i == strlen(cmdString) - 1) && firstNonSpaceChar != -1) {
            bool isLastChar = (i == strlen(cmdString) - 1 && cmdString[i] != ' ');
            int cmdLength = i - firstNonSpaceChar + isLastChar;
            char* cmdName = malloc((cmdLength + 1) * sizeof(char));
            strncpy(cmdName, cmdString + firstNonSpaceChar, cmdLength);
            cmdName[cmdLength] = '\0';
            char* aliasValue = getAlias(aliasStore, cmdName);
            if (strlen(aliasValue) > 0) {
                // Calculate extra space required
                int extraSpace = strlen(aliasValue) - cmdLength;
                shiftString(cmdString, firstNonSpaceChar + cmdLength, strlen(cmdString), extraSpace);
                for (int j = 0; j < strlen(aliasValue); j++) {
                    cmdString[j + firstNonSpaceChar] = aliasValue[j];
                }
            }
            free(cmdName);
            break;
        }
    }
}
