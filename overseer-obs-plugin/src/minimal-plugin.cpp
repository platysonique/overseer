#include <obs-module.h>
#include <util/base.h>

bool obs_module_load(void) {
    printf("[MINIMAL] Plugin loaded successfully!\n");
    fflush(stdout);
    blog(LOG_INFO, "[MINIMAL] Plugin loaded successfully!");
    return true;
}

void obs_module_unload(void) {
    blog(LOG_INFO, "[MINIMAL] Plugin unloaded");
}
