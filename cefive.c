#include <stdlib.h>
#include <psploadcore.h>
#include "cefiveconfig.h"
#include "cheatengine.h"
#include "searchengine.h"
#include "cefiveui.h"
#include "cefive.h"

static int wait_for_kernellib(CEFive* prCe);

CheatEngine* cefive_get_cheatengine(CEFive* prCe) {
    CheatEngine* prEngine = NULL;
    if (prCe != NULL) {
        prEngine = &prCe->rCheatEngine;
    }
    return prEngine;
}

CEFiveConfig* cefive_get_config(CEFive* prCe) {
    CEFiveConfig* prConfig = NULL;
    if (prCe != NULL) {
        prConfig = &prCe->rConfig;
    }
    return prConfig;
}

SearchEngine* cefive_get_searchengine(CEFive* prCe) {
    SearchEngine* prEngine = NULL;
    if (prCe != NULL) {
        prEngine = &prCe->rSearchEngine;
    }
    return prEngine;
}

CEFiveUi* cefive_get_ui(CEFive* prCe) {
    CEFiveUi* prUi = NULL;
    if (prCe != NULL) {
        prUi = &prCe->rUi;
    }
    return prUi;
}

int cefive_init(CEFive* prCe) {
    CEFiveConfig* prConfig = NULL;
    
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prConfig = cefive_get_config(prCe);
    cefiveconfig_init(prConfig);
    
    return CEFIVE_SUCCESS;
}

int cefive_run(CEFive* prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }

    return CEFIVE_SUCCESS;
}

int cefive_start(CEFive* prCe) {
    int r = 0;

    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    r = wait_for_kernellib(prCe);
    if (r != CEFIVE_SUCCESS) {
        return CEFIVE_FAILURE;
    }

    return CEFIVE_SUCCESS;
}

int cefive_stop(CEFive* prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }

    return CEFIVE_SUCCESS;
}

static int wait_for_kernellib(CEFive* prCe) {
    SceModule* prKernel = NULL;
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    while (prKernel == NULL) {
        prKernel = sceKernelFindModuleByName(CEFIVE_KERNEL_LIB);
        if (prKernel != NULL) {
            prCe->prKernelLib = prKernel;
        } else {
            sceKernelDelayThread(50000);
        }
    }

    return CEFIVE_SUCCESS;
}
