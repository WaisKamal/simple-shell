#include <stdlib.h>
#include <string.h>
#include "variable_store.h"

// Sets the value of a variable
void setVariable(struct VariableStore* varStore, char* name, char* value) {
    // If variable exists, update its value
    for (int i = 0; i < varStore->count; i++) {
        if (!strcmp(varStore->variables[i].name, name)) {
            varStore->variables[i].value = value;
            return;
        }
    }
    // Create a new variable if the variable does not exist
    struct Variable* newVars = malloc((varStore->count + 1) * sizeof(struct Variable));
    for (int i = 0; i < varStore->count; i++) {
        newVars[i].name = varStore->variables[i].name;
        newVars[i].value = varStore->variables[i].value;
    }
    newVars[varStore->count].name = name;
    newVars[varStore->count].value = value;
    free(varStore->variables);
    varStore->variables = newVars;
    varStore->count++;
}

// Deletes an environment variable
void removeVariable(struct VariableStore* varStore, char* cmdString) {
    char* cmdStringCopy = malloc((strlen(cmdString) + 1) * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* varName = strtok(NULL, " ");
    // Check if environment variable exists
    int varIndex = -1;
    for (int i = 0; i < varStore->count; i++) {
        if (!strcmp(varStore->variables[i].name, varName)) {
            varIndex = i;
            break;
        }
    }
    if (varIndex == -1) return;
    // Do nothing if variable does not exist, otherwise remove variable
    struct Variable* newVars = malloc((varStore->count - 1) * sizeof(struct Variable));
    for (int i = 0; i < varIndex; i++) {
        newVars[i].name = varStore->variables[i].name;
        newVars[i].value = varStore->variables[i].value;
    }
    for (int i = varIndex + 1; i < varStore->count; i++) {
        newVars[i - 1].name = varStore->variables[i].name;
        newVars[i - 1].value = varStore->variables[i].value;
    }
    free(varStore->variables);
    varStore->variables = newVars;
    varStore->count--;
}

// Returns the environment variable's value if found, or an empty string otherwise
char* getVariable(struct VariableStore* varStore, char* name) {
    // If variable exists, update its value
    for (int i = 0; i < varStore->count; i++) {
        if (!strcmp(varStore->variables[i].name, name)) {
            return varStore->variables[i].value;
        }
    }
    return "";
}