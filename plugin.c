// #include <stdio.h>
// #include <stdlib.h>
// #include <dlfcn.h>
// #include <string.h> 
// #include "plugin.h"        

// //start plugin file----------------------------------------------------------
// Plugin plugins[MAX_PLUGINS]; 
// int plugin_count = 0;         

// int load_plugin(const char *name) {
    
//     //edge case 1
//     if (plugin_count >= MAX_PLUGINS) {
//         fprintf(stderr, "Error: Maximum number of plugins loaded.\n");
//         return 0;
//     }

//     //check if the plugin is already loaded: edge case 2
//     for (int i = 0; i < plugin_count; i++) {
//         if (strcmp(plugins[i].name, name) == 0) {
//             fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
//             return 0;
//         }
//     }

//     //general case
//     //construct the plugin file path
//     char path[200];
//     snprintf(path, sizeof(path), "./%s.so", name);

//     //dynamically load library
//     void *handle = dlopen(path, RTLD_LAZY);
//     if (!handle) {
//         fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
//         return 0;
//     }
    
//     //call initialize function
//     int (*initialize)(char **) = dlsym(handle, "initialize");
//     if (!initialize) {
//         fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
//         dlclose(handle);
//         return 0;
//     }

//     //get the plugin's run function
//     int (*run)(char **) = dlsym(handle, "run");
//     if (!run) {
//         fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
//         dlclose(handle);
//         return 0;
//     }

//     //call `initialize` and check its return value
//     if (initialize != 0) {
//         fprintf(stderr, "Error: Plugin %s initialization failed!\n", name);
//         dlclose(handle);
//         return 0;
//     }

//     //add plugin to plugin list
//     strncpy(plugins[plugin_count].name, name, MAX_PLUGIN_NAME_LEN - 1);
//     plugins[plugin_count].name[MAX_PLUGIN_NAME_LEN - 1] = '\0';
//     plugins[plugin_count].handle = handle;
//     plugins[plugin_count].initialize = initialize;
//     plugins[plugin_count].run = run;
//     //update index
//     plugin_count++;

//     //printf("Plugin '%s' loaded successfully.\n", name);
//     return 1;
// }

// //run a plugin by its name: loop through the array
// int find_plugin_and_run(char **argv) {
//     for (int i = 0; i < plugin_count; i++) {
//         if (strcmp(plugins[i].name, argv[0]) == 0) {
//             //printf("Running plugin '%s'.\n", plugins[i].name);
//             return plugins[i].run(argv);
//         }
//     }
//     return 0; // Plugin not found
// }

// //delete all plug ins
// void delete_all_plugins() {
//     for (int i = 0; i < plugin_count; i++) {
//         //close the shared library
//         dlclose(plugins[i].handle);
//         //printf("Plugin '%s' unloaded.\n", plugins[i].name);
//     }
//     plugin_count = 0;
// }

// //-------------------------------------End plugin file