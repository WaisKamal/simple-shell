#ifndef ENV
#define ENV

struct EnvironmentVariables {
    int count;
    struct Variable* variables;
};

struct Variable {
    char* name;
    char* value;
};

// Fetches environment variables into envVars struct
void loadEnvironmentVariables(char** env, struct EnvironmentVariables* envVars);

// Sets the value of an environment variable
void setEnvironmentVariable(struct EnvironmentVariables* envVars, char* name, char* value);

// Deletes an environment variable
void removeEnvironmentVariable(struct EnvironmentVariables* envVars, char* name);

// Returns the variable's value if found, or an empty string otherwise
char* getEnvironmentVariable(struct EnvironmentVariables* envVars, char* name);

// Executes the setenv command
void exec_setenv(struct EnvironmentVariables* envVars, char* cmdString);

// Executes the unsetenv command
void exec_unsetenv(struct EnvironmentVariables* envVars, char* cmdString);

// Replaces environment variables in command string with their respoective values
void evaluateEnvironmentVariables(struct EnvironmentVariables* envVars, char* cmdString);

#endif