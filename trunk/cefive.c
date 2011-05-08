#include "cefive.h"

static int load_gameid(CEFive* prCe);
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

GameInfo* cefive_get_gameinfo(CEFive* prCe) {
    GameInfo* prInfo = NULL;
    if (prCe != NULL) {
        prInfo = &prCe->rGameInfo;
    }
    return prInfo;
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

int cefive_init(CEFive* prCe, GeeLog* prLog) {
    CEFiveConfig* prConfig = NULL;
    SearchEngine* prSearch = NULL;
    int r = 0;
    
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prCe->prLogger = prLog;
    prConfig = cefive_get_config(prCe);
    cefiveconfig_init(prConfig);
    prSearch = cefive_get_searchengine(prCe);
    searchengine_init(prSearch);
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

static int load_gameid(CEFive* prCe) {
    GameInfo* prInfo = NULL;
    SceUID fh = 0;
    char sGameId[CEFIVE_GAMEID_LEN + 1];
    
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prInfo = cefive_get_gameinfo(prCe);
    if (prInfo == NULL) {
        return CEFIVE_NULLPTR;
    }
    while (fh <= 0) {
        fh = sceIoOpen(CEFIVE_UMD_PATH, PSP_O_RDONLY, 0777);
        sceKernelDelayThread(10000);
    }
    sceIoRead(fh, sGameId, CEFIVE_GAMEID_LEN);
    sceIoClose(fh);
    strcpy(prInfo->sGameId, sGameId);
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
