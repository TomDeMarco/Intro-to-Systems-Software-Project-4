// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <dlfcn.h>
// //#include "plugin.h"

// //plugin header file------------------------------------------------------
// #ifndef PLUGIN_H
// #define PLUGIN_H

// #define MAX_PLUGINS 10
// #define MAX_PLUGIN_NAME_LEN 20

// //plugin structure
// typedef struct {
//     char name[MAX_PLUGIN_NAME_LEN]; 
//     void *handle;
//     int (*initialize)(char **);                   
//     int (*run)(char **);          
// } Plugin;

// //list of plugins
// extern Plugin plugins[MAX_PLUGINS];
// extern int plugin_count;

// //function declarations
// int load_plugin(const char *name);      
// int find_plugin_and_run(char **argv);   
// void delete_all_plugins();           

// #endif
// //end plugin header file--------------------------------------------------