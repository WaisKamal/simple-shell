#ifndef ALIAS
#define ALIAS

#include "variable_store.h"

// Creates a new alias or sets an existing alias
void setAlias(struct VariableStore* aliasStore, char* name, char* value);

// Removes an alias
void removeAlias(struct VariableStore* aliasStore, char* name);

// Gets the value of an alias
char* getAlias(struct VariableStore* aliasStore, char* name);

// Executes the alias command
void exec_alias(struct VariableStore* aliasStore, char* cmdString);

// Evaluates alias in cmdString if found
void evaluateAlias(struct VariableStore* aliasStore, char* cmdString);

#endif