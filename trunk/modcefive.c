#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include <pspmoduleinfo.h>
#include "cefive.h"

PSP_MODULE_INFO("CEFive", 0x3007, CEFIVE_VERSION_MAJ, CEFIVE_VERSION_MIN);
PSP_MAIN_THREAD_ATTR(0); //0 for kernel mode too

void module_start(SceSize args, void *argp) {
    if (cefive_start() < 0) {
        return -1;
    }
    return 0;
}

void module_stop(SceSize args, void *argp) {
    if (cefive_stop() < 0) {
        return -1;
    }
    return 0;
}
