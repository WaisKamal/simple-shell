#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "variable_store.h"
#include "env.h"
#include "string_utils.h"

// Fetches environment variables into envVars struct
void loadEnvironmentVariables(char** env, struct VariableStore* envVars) {
    // Calculate number of variables and allocate enough memory
    int envCount = 0;
    while (env[envCount]) envCount++;
    envVars->count = envCount;
    envVars->variables = malloc(envCount * sizeof(struct Variable));
    // Loop through environment variables and allocate memory as needed
    for (int i = 0; i < envCount; i++) {
        envVars->variables[i].name = strtok(env[i], "=");
        envVars->variables[i].value = strtok(NULL, "=");
    }
}

// Sets the value of an environment variable
void setEnvironmentVariable(struct VariableStore* envVars, char* name, char* value) {
    setVariable(envVars, name, value);
}

// Deletes an environment variable
void removeEnvironmentVariable(struct VariableStore* envVars, char* name) {
    // Check if environment variable exists
    int varIndex = -1;
    for (int i = 0; i < envVars->count; i++) {
        if (!strcmp(envVars->variables[i].name, name)) {
            varIndex = i;
            break;
        }
    }
    if (varIndex == -1) return;
    // Do nothing if variable does not exist, otherwise remove variable
    struct Variable* newEnvVars = malloc((envVars->count - 1) * sizeof(struct Variable));
    for (int i = 0; i < varIndex; i++) {
        newEnvVars[i].name = envVars->variables[i].name;
        newEnvVars[i].value = envVars->variables[i].value;
    }
    for (int i = varIndex + 1; i < envVars->count; i++) {
        newEnvVars[i - 1].name = envVars->variables[i].name;
        newEnvVars[i - 1].value = envVars->variables[i].value;
    }
    free(envVars->variables);
    envVars->variables = newEnvVars;
    envVars->count--;
}

// Returns the environment variable's value if found, or an empty string otherwise
char* getEnvironmentVariable(struct VariableStore* envVars, char* name) {
    // If variable exists, update its value
    for (int i = 0; i < envVars->count; i++) {
        if (!strcmp(envVars->variables[i].name, name)) {
            return envVars->variables[i].value;
        }
    }
    return "";
}

void exec_setenv(struct VariableStore* envVars, char* cmdString) {
    char* cmdStringCopy = malloc((strlen(cmdString) + 1) * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* varName = strtok(NULL, " ");
    if (varName == NULL) {
        // Print all environment variables and return
        for (int i = 0; i < envVars->count; i++) {
            printf("%s=%s\n", envVars->variables[i].name, envVars->variables[i].value);
        }
        return;
    }
    char* varValue = strtok(NULL, " ");
    if (varValue == NULL) {
        setEnvironmentVariable(envVars, varName, "");
    } else {
        setEnvironmentVariable(envVars, varName, varValue);
    }
}

void exec_unsetenv(struct VariableStore* envVars, char* cmdString) {
    char* cmdStringCopy = malloc((strlen(cmdString) + 1) * sizeof(char));
    strcpy(cmdStringCopy, cmdString);
    char* commandName = strtok(cmdStringCopy, " ");
    char* varName = strtok(NULL, " ");
    removeEnvironmentVariable(envVars, varName);
}

void evaluateEnvironmentVariables(struct VariableStore* envVars, char* cmdString) {
    // The extra space required for the parsed cmdString
    int extraChars = 0;
    // Position of dollar sign (-1 if no dollar sign encountered)
    int dollarSignPos = -1;
    // The current length of the command string
    int cmdStringLength = strlen(cmdString);
    for (int i = 0; i < cmdStringLength; i++) {
        if (cmdString[i] == '$') {
            dollarSignPos = i;
        } else if (cmdString[i] == ' ' || i == strlen(cmdString) - 1) {
            if (dollarSignPos != -1) {
                // Environment variable encountered
                bool isLastChar = (i == strlen(cmdString) - 1 && cmdString[i] == ' ');
                int envVarLength = i - dollarSignPos - isLastChar;
                char* envVarName = malloc((envVarLength + 1) * sizeof(char));
                strncpy(envVarName, cmdString + dollarSignPos + 1, envVarLength);
                char* envVarValue = getEnvironmentVariable(envVars, envVarName);
                // Subtracting 1 because the dollar sign will be replaced
                int extraSpace = strlen(envVarValue) - envVarLength - 1;
                shiftString(cmdString, dollarSignPos + envVarLength + 1, strlen(cmdString), extraSpace);
                for (int j = 0; j < strlen(envVarValue); j++) {
                    cmdString[j + dollarSignPos] = envVarValue[j];
                }
                // Move the string pointer to the last location and update length
                cmdStringLength += extraSpace;
                i = strlen(envVarValue) + dollarSignPos;
                // Reset dollarSignPos
                dollarSignPos = -1;
            }
        }
    }
}
