#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
//#include "plugin.h"

//plugin header file------------------------------------------------------
#ifndef PLUGIN_H
#define PLUGIN_H

#define MAX_PLUGINS 10
#define MAX_PLUGIN_NAME_LEN 20

//plugin structure
typedef struct {
    char name[MAX_PLUGIN_NAME_LEN]; 
    void *handle;
    int (*initialize)();                   
    int (*run)(char **);          
} Plugin;

//list of plugins
extern Plugin plugins[MAX_PLUGINS];
extern int plugin_count;

//function declarations
int load_plugin(const char *name);      
int find_plugin_and_run(char **argv);   
void delete_all_plugins();           

#endif
//end plugin header file--------------------------------------------------

//start plugin file----------------------------------------------------------
Plugin plugins[MAX_PLUGINS]; 
int plugin_count = 0;         

int load_plugin(const char *name) {
    
    //edge case 1
    if (plugin_count >= MAX_PLUGINS) {
        fprintf(stderr, "Error: Maximum number of plugins loaded.\n");
        return 0;
    }

    //check if the plugin is already loaded: edge case 2, if loaded, run
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, name) == 0) {
            fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
            return 0;
        }
    }

    //general case
    //construct the plugin file path
    char path[200];
    snprintf(path, sizeof(path), "./%s.so", name);

    //dynamically load library
    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
        return 0;
    }
    
    //get initialize function
    int (*initialize)() = dlsym(handle, "initialize");
    if (!initialize) {
        fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
        dlclose(handle);
        return 0;
    }

    //call initialize function
    if (initialize() != 0) {
        fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
        dlclose(handle);
        return 0;
    }

    //get the plugin's run function
    int (*run)(char **) = dlsym(handle, "run");
    if (!run) {
        fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
        dlclose(handle);
        return 0;
    }

    //add plugin to plugin list
    strncpy(plugins[plugin_count].name, name, MAX_PLUGIN_NAME_LEN - 1);
    plugins[plugin_count].name[MAX_PLUGIN_NAME_LEN - 1] = '\0';
    plugins[plugin_count].handle = handle;
    plugins[plugin_count].initialize = initialize;
    plugins[plugin_count].run = run;
    //update index
    plugin_count++;

    //printf("Plugin '%s' loaded successfully.\n", name);
    return 1;
}

//run a plugin by its name: loop through the array
int find_plugin_and_run(char **argv) {
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, argv[0]) == 0) {
            //printf("Running plugin '%s'.\n", plugins[i].name);
            return plugins[i].run(argv);
        }
    }
    return 0; //plugin not found
}

//delete all plug ins
void delete_all_plugins() {
    for (int i = 0; i < plugin_count; i++) {
        //close the shared library
        dlclose(plugins[i].handle);
        //printf("Plugin '%s' unloaded.\n", plugins[i].name);
    }
    plugin_count = 0;
}

//-------------------------------------End plugin file

#define MAX_COMMAND_LEN 200
#define MAX_ARGS 20

//Function declarations:
//Shell Loop part 1
void print_prompt();
char *read_command();
int parse_command(char *input, char **argv);
//----------------------------------------
//built in commands(load and exit) part 2
int handle_built_in_commands(char **argv);
//----------------------------------------
//to execute external commands part 3
void execute_external_command(char **argv);


int main() {
    char input[MAX_COMMAND_LEN];
    char *argv[MAX_ARGS + 1];

    while (1) {

        //print shell prompt to user
        print_prompt();

        //if time to exit gracefully
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        //otherwise read user input

        //remove trailing newline character that fgets adds
        input[strcspn(input, "\n")] = '\0';


        //parse the command into arguments
        int argc = parse_command(input, argv);
        if (argc == 0) {
            //if no command is entered, prompt again
            continue;
        }


        //print the parsed command and arguments
        // printf("Command: %s\n", argv[0]);
        // printf("Arguments: ");
        // for (int i = 0; argv[i] != NULL; i++) {
        //     printf("%s ", argv[i]);
        // }
        // printf("\n");


        //handle built in commands (return 1 if command is handled)
        if(handle_built_in_commands(argv) == 1){
            continue;
        }

        //if the command is not a built-in, check if it's a plugin
        if (find_plugin_and_run(argv) == 0) {
            //if not a plug in, run external command
            execute_external_command(argv);
            //printf("Command not found.\n");
        }

        //clear plugins before exiting: no memory leak for extra credit?
        delete_all_plugins();
    }

    return 0;
}

//print the shell prompt
void print_prompt() {
    printf("> ");
}

//parse the command into arguments, returns number of args
int parse_command(char *input, char **argv) {
    int argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && argc < MAX_ARGS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL; //null-terminate the argument list
    return argc;
}

//handle built in commands: returns 1 if handled, 0 otherwise
int handle_built_in_commands(char **argv) {
    if (strcmp(argv[0], "exit") == 0) {
        //exit the shell
        printf("Exiting shell...\n");
        exit(0); //use exit() to terminate the program
    } else if (strcmp(argv[0], "load") == 0) {
        if (argv[1] == NULL) {
        //no argument = throw error
        fprintf(stderr, "Error: Plugin <some_string> initialization failed!\n");
        return 1;
        } else {
            load_plugin(argv[1]);
        }
        return 1; //command handled: success
    }
    return 0; //command not handled: failed :(
}

//execute commands that arent builtin or plugins
void execute_external_command(char **argv) {
    
    pid_t pid = fork();

    if (pid == 0) {

        //child process: execute the command
        if (execvp(argv[0], argv) == -1) {
            //perror("Error executing command");
        }

        //terminate child process if exec fails
        exit(EXIT_FAILURE);

    } else if (pid < 0) {
        
        //fork failed
        perror("Fork failed");

    } else {
        
        //parent process: wait for the child to complete
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            //printf("Process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            //printf("Process terminated by signal %d\n", WTERMSIG(status));
        }
    }
}