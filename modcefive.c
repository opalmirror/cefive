#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include "cefive.h"
#include "geelog.h"

#define CEFIVETHREAD "CEFiveThread"
#define CEFIVEIPRIO (0x18)
#define CEFIVESSIZE (0x1000)

static SceUID krThreadId;
static CEFive krCefive;
static GeeLog krLog;

static int cmodule_run() {
    GeeLog* prLog = &krLog;
    int r = 0;
    geelog_init(prLog, LOG_DEBUG, "ms0:/seplugins/CEFive.log");
    geelog_start(prLog);
    
    geelog_log(prLog, LOG_DEBUG, "cmodule_run: Initializing cefive.");
    if (cefive_init(&krCefive) != CEFIVE_SUCCESS) {
        geelog_log(prLog, LOG_ERROR, 
                "cmodule_run: Failed to initialize cefive.");
        return 1;
    }
    geelog_log(prLog, LOG_DEBUG, "cmodule_run: Starting cefive.");
    if (cefive_start(&krCefive) != CEFIVE_SUCCESS) {
        geelog_log(prLog, LOG_ERROR, "cmodule_run: Failed to start cefive.");
        return 1;
    }
    geelog_log(prLog, LOG_DEBUG, "cmodule_run: Running cefive.");
    r = cefive_run(&krCefive);
    if (r != CEFIVE_SUCCESS) {
        geelog_log(prLog, LOG_ERROR, "cmodule_run: Error running cefive.");
        return 1;
    }
    geelog_log(prLog, LOG_DEBUG, "cmodule_run: Stopping cefive.");
    if (cefive_stop(&krCefive) != CEFIVE_SUCCESS) {
        geelog_log(prLog, LOG_ERROR, "cmodule_run: Failed to stop cefive.");
        return 1;
    }
    geelog_log(prLog, LOG_DEBUG, "cmodule_run: Finished.");
    return 0;
}

void module_start(SceSize args, void *argp) {
    krThreadId = sceKernelCreateThread(CEFIVETHREAD, cmodule_run, CEFIVEIPRIO, 
            CEFIVESSIZE, NULL);
    if (krThreadId >= 0) {
        sceKernelStartThread(krThreadId, 0, NULL);
    }
}

void module_stop(SceSize args, void *argp) {
    sceKernelTerminateThread(krThreadId);
}
