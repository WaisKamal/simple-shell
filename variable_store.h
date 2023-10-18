#ifndef VARIABLE_STORE
#define VARIABLE_STORE

struct VariableStore {
    int count;
    struct Variable* variables;
};

struct Variable {
    char* name;
    char* value;
};

// Sets the value of a variable
void setVariable(struct VariableStore* varStore, char* name, char* value);

// Deletes an environment variable
void removeVariable(struct VariableStore* varStore, char* cmdString);

// Returns the environment variable's value if found, or an empty string otherwise
char* getVariable(struct VariableStore* varStore, char* name);

#endif
