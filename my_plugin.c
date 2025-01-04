#include <stdio.h>

int initialize() {
    printf("Plugin: Initializing...\n");
    return 0;
}

int run() {
    printf("Plugin: Running...\n");
    return 0;
}

int cleanup() {
    printf("Plugin: Cleaning up...\n");
    return 0;
}
