#include "cefive.h"

static int cefive_init(CEFive* prCe);
static int cefive_run(SceSize rLen, void* argv);
static int load_config(CEFive* prCe, const char* sFile);

static SceUID krThreadId;

static int cefive_init(CEFive* prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    if (cefiveconfig_init(&prCe->rConfig) < 0) {
        return CEFIVE_FAILURE;
    }
    return CEFIVE_SUCCESS;
}

static int cefive_run(SceSize rLen, void* argv) {
    CEFive rCe;
    
    if (cefive_init(&rCe) < 0) {
        return CEFIVE_FAILURE;
    }
    if (load_config(&rCe, "ms0:/seplugins/CEFive.cdf") < 0) {
        /* This is not that big of a deal, just use defaults. */
    }
}

int cefive_start() {
    krThreadId = sceKernelCreateThread(CEFIVE_THREAD_NAME, cefive_run, 
            CEFIVE_INIT_PRIO, CEFIVE_STACK_SIZE, 0, NULL);
    if (krThreadId < 0) {
        return CEFIVE_FAILURE;
    }
    sceKernelStartThread(krThreadId, 0, NULL);
    return CEFIVE_SUCCESS;
}

int cefive_stop() {
    
}

static int load_config(CEFive* prCe, const char* sFile) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    if (sFile == NULL) {
        return CEFIVE_FAILURE;
    }
    if (cefiveconfig_load(&prCe->rConfig, sFile) < 0) {
        return CEFIVE_FAILURE;
    }
    strncpy(prCe->rConfig.plugins_dir, CEFIVE_PLUGIN_DIR, CEFIVE_PATH_LEN);
    strncpy(prCe->rConfig.cefive_dir, CEFIVE_DATA_DIR, CEFIVE_PATH_LEN);
    return CEFIVE_SUCCESS;
}
