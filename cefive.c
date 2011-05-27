#include "cefive.h"
#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include <psploadcore.h>
#include "cefiveconfig.h"
#include "geelog.h"
#include "cheatengine.h"
#include "searchengine.h"
#include "cefiveui.h"
#include "ggame.h"

static int cefive_input(int cur, int last, void* pvArg);
static int cefive_flog(ELogLevel rLevel, const char* sFunc, const char* sMsg);
static int cefive_log(ELogLevel rLevel, const char* sMsg);
static int cefive_run(int argl, void* argv);
static int load_config();

/** Wait for the Game Data to become available, then load the Game information.
 * 
 * @return 0 indicates success, less than 0 indicates failure.
 */
static int load_game();
static int start_cheatengine();
static int start_interface();
static int start_searchengine();
static int wait_for_kernel();

static CEFive krCe;
static SceUID kThreadId;

static int cefive_flog(ELogLevel rLevel, const char* sFunc, const char* sMsg) {
    if ((sFunc == NULL) || (sMsg == NULL)) {
        return CEFIVE_NULLPTR;
    }
}

CheatEngine* cefive_get_cheatengine() {
    return &krCe.rCheatEngine;
}

CEFiveConfig* cefive_get_config() {
    return &krCe.rConfig;
}

GGame* cefive_get_game() {
    return &krCe.rGame;
}

CEFiveUi* cefive_get_interface() {
    return &krCe.rUi;
}

SceModule* cefive_get_kernellib() {
    return krCe.prKernelLib;
}

SearchEngine* cefive_get_searchengine() {
    return &krCe.rSearchEngine;
}

SceUID cefive_get_threadid() {
    return kThreadId;
}

int cefive_init(GeeLog* prLog) {
    CEFiveConfig* prConfig = NULL;
    CheatEngine* prEng = NULL;
    
    krCe.prLogger = prLog;
    kThreadId = -1;
    krCe.prGame = NULL;
    krCe.prKernelLib = NULL;
    krCe.prVideoRam = NULL;
    krCe.rRunState = CES_Stopped;
    
    prConfig = cefive_get_config();
    if (cefiveconfig_init(prConfig) != CEFIVECONFIG_SUCCESS) {
        cefive_log(LOG_ERROR, 
                "cefive_init: Failed to initialize CEFiveConfig.");
        return CEFIVE_FAILURE;
    }
    
    return CEFIVE_SUCCESS;
}

static int cefive_input(int cur, int last, void* pvArg) {
    
}

static int cefive_log(ELogLevel rLevel, const char* sMsg) {
    if (krCe.prLogger != NULL) {
        geelog_log(krCe.prLogger, rLevel, sMsg);
    }
    return CEFIVE_SUCCESS;
}

static int cefive_run(int argl, void* argv) {
    const char* sFunc = "cefive_run";
    if (load_config() != CEFIVE_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to Load Configuration.");
        return CEFIVE_FAILURE;
    }
    
    if (start_cheatengine() != CEFIVE_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to start Cheat Engine.");
        return CEFIVE_FAILURE;
    }

    if (start_searchengine() != CEFIVE_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to start Search Engine.");
        return CEFIVE_FAILURE;
    }
    
    if (start_interface() != CEFIVE_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to start User Interface.");
        return CEFIVE_FAILURE;
    }
    
    if (wait_for_kernel() != CEFIVE_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to locate Kernel Library.");
        return CEFIVE_FAILURE;
    }
    
    if (load_game() != CEFIVE_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to Load Game Information.");
        return CEFIVE_FAILURE;
    }
    return CEFIVE_SUCCESS;
}

int cefive_start() {
    if (kThreadId != -1) {
        return CEFIVE_SUCCESS;
    }
    kThreadId = sceKernelCreateThread(CEFIVE_THREAD_NAME, cefive_run,
            CEFIVE_INIT_PRIO, CEFIVE_STACK_SIZE, 0, NULL);
    if (kThreadId < 0) {
        return CEFIVE_FAILURE;
    }
    if (sceKernelStartThread(kThreadId, 0, NULL) < 0) {
        return CEFIVE_FAILURE;
    }

    return CEFIVE_SUCCESS;
}

int cefive_stop() {

}

static int load_config() {
    CEFiveConfig* prCfg = NULL;
    char sPath[CEFIVE_PATH_LEN + 1];
    int r = 0;
    
    /* Initialize and load the Configuration */
    prCfg = cefive_get_config();
    sprintf(sPath, "%s/%s", CEFIVE_PLUGIN_PATH, CEFIVE_CONFIG_FILE);

    r = cefiveconfig_load(prCfg, sPath);
    if (r != CEFIVECONFIG_SUCCESS) {
        cefive_log(LOG_WARN, "load_config: Could not load Configuration.");
    }
    if (cefiveconfig_set_cefivedir(
            prCfg, CEFIVE_DATA_DIR) != CEFIVECONFIG_SUCCESS) {
        cefive_log(LOG_ERROR, "load_config: Failed to assign CEFive Dir.");
        return CEFIVE_FAILURE;
    }
    if (cefiveconfig_set_plugindir(
            prCfg, CEFIVE_PLUGIN_DIR) != CEFIVECONFIG_SUCCESS) {
        cefive_log(LOG_ERROR, "load_config: Failed to assign Plugin Dir.");
        return CEFIVE_FAILURE;
    }
    
    return CEFIVE_SUCCESS;
}

static int load_game() {
    const char* sFunc = "load_game";
    GGame* prGame = NULL;
    int r = GGAME_FAILURE;
    
    prGame = cefive_get_game();
    cefive_flog(LOG_DEBUG, sFunc, "Loading Game Information.");
    while (r == GGAME_FAILURE) {
        r = ggame_load_gameid(prGame);
    }
    if (r != GGAME_SUCCESS) {
        cefive_flog(LOG_ERROR, sFunc, "Failed to Load Game Information.");
        return CEFIVE_FAILURE;
    }
    cefive_flog(LOG_INFO, sFunc, "Loaded Game Information.");
    return CEFIVE_SUCCESS;
}

static int start_cheatengine() {
    CEFiveConfig* prCfg = NULL;
    CheatEngine* prEng = NULL;
    int r = 0;
    
    prCfg = cefive_get_config();
    prEng = cefive_get_cheatengine();
    r = cheatengineInit(prEng, prCfg, NULL, NULL);
    if (r != CHEATENGINE_SUCCESS) {
        cefive_log(LOG_ERROR, 
                "start_cheatengine: Failed to initialize Cheat Engine.");
        return CEFIVE_FAILURE;
    }
    cefive_log(LOG_INFO,
            "start_cheatengine: Cheat Engine started.");
    return CEFIVE_SUCCESS;
}

static int start_interface() {
    CEFiveUi* prUi = NULL;
    CheatEngine* prEngine = NULL;
    SearchEngine* prSearch = NULL;
    CEFiveConfig* prConfig = NULL;
    
    prUi = cefive_get_interface();
    prEngine = cefive_get_cheatengine();
    prSearch = cefive_get_searchengine();
    prConfig = cefive_get_config();
    prUi->prCEConfig = prConfig;
    cefiveuiInit(prUi, prEngine, prSearch);
    
    return CEFIVE_SUCCESS;
}

static int start_searchengine() {
    SearchEngine* prSearch = NULL;
    int r = 0;
    
    /* Initialize and start the Search Engine */
    prSearch = cefive_get_searchengine();
    r = searchengine_init(prSearch);
    if (r != SEARCHENGINE_SUCCESS) {
        cefive_log(LOG_ERROR, 
                "start_searchengine: Failed to initialize Search Engine.");
        return CEFIVE_FAILURE;
    }
    cefive_log(LOG_INFO,
            "start_searchengine: Search Engine started.");
    
    return CEFIVE_SUCCESS;
}

static int wait_for_kernel() {
    const char* sFunc = "wait_for_kernel";
    SceModule* prModule = NULL;
    
    cefive_flog(LOG_DEBUG, sFunc, "Waiting for Kernel Library.");
    while (prModule == NULL) {
        sceKernelDelayThread(100000);
        prModule = sceKernelFindModuleByName(CEFIVE_KERNEL_LIB);
    }
    krCe.prKernelLib = prModule;
    cefive_flog(LOG_INFO, sFunc, "Located Kernel Library.");
    return CEFIVE_SUCCESS;
}
