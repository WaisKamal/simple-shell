#ifndef ENV
#define ENV

#include "variable_store.h"

void loadEnvironmentVariables(char** env, struct VariableStore* envVars);

// Sets the value of an environment variable
void setEnvironmentVariable(struct VariableStore* envVars, char* name, char* value);

// Deletes an environment variable
void removeEnvironmentVariable(struct VariableStore* envVars, char* name);

// Returns the environment variable's value if found, or an empty string otherwise
char* getEnvironmentVariable(struct VariableStore* envVars, char* name);

// Executes setenv command
void exec_setenv(struct VariableStore* envVars, char* cmdString);

// Executes unsetenv command
void exec_unsetenv(struct VariableStore* envVars, char* cmdString);

// Replaces environment variables in cmdString with their values
void evaluateEnvironmentVariables(struct VariableStore* envVars, char* cmdString);

#endif
