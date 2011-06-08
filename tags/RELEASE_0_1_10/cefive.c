#include "cefive.h"

static int cefive_init(CEFive* prCe);
static int cefive_run(SceSize rLen, void* argv);
static int load_config(CEFive* prCe, const char* sFile);
static int register_button_callbacks(CEFive* prCe);
static int wait_for_kernel(CEFive* prCe);

static SceUID krThreadId;

static void button_callback(int curr, int last, void* pvCe) {
    CEFive* prCe = NULL;
    int cvd = 0;
    int cvu = 0;
    int cno = 0;
    int lvd = 0;
    int lvu = 0;
    int lno = 0;
    
    if (pvCe == NULL) {
        return;
    }
    prCe = (CEFive*)pvCe;
    cvd = (curr & PSP_CTRL_VOLDOWN);
    lvd = (last & PSP_CTRL_VOLDOWN);
    cvu = (curr & PSP_CTRL_VOLUP);
    lvu = (last & PSP_CTRL_VOLUP);
    cno = (curr & PSP_CTRL_NOTE);
    lno = (last & PSP_CTRL_NOTE);
    
    if (cvu && cvd) {
        if (!lvu || !lvd) {
            /* Volume Up and Volume Down are pressed. */
            sceKernelSetEventFlag(prCe->eventUID, CEFIVE_E_SHOW_UI);
            return;
        }
    }
    
    if (cno && !lno) {
        sceKernelSetEventFlag(prCe->eventUID, CEFIVE_E_ACTIVATE);
        return;
    }
}

static int cefive_init(CEFive* prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    if (cefiveconfig_init(&prCe->rConfig) < 0) {
        return CEFIVE_FAILURE;
    }
    if (searchengine_init(&prCe->rSearchEngine) < 0) {
        return CEFIVE_FAILURE;
    }
    if (ggame_init(&prCe->rGame) < 0) {
        return CEFIVE_FAILURE;
    }
    prCe->eventUID = sceKernelCreateEventFlag(CEFIVE_EVENT_NAME, 
            PSP_EVENT_WAITMULTIPLE, 0, NULL);
    if (prCe->eventUID < 0) {
        return CEFIVE_FAILURE;
    }
    prCe->prKernelLib = NULL;
    return CEFIVE_SUCCESS;
}

static int cefive_run(SceSize rLen, void* argv) {
    const char* sFunc = "cefive_run";
    char sFile[CEFIVE_PATH_LEN + 1];
    CEFive rCe;
    SceUInt32 match = 0;
    
    if (cefive_init(&rCe) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to initialize CEFive.");
        return CEFIVE_FAILURE;
    }
    sprintf(sFile, "ms0:/%s/CEFive.cdf", CEFIVE_PLUGIN_DIR);
    if (load_config(&rCe, sFile) < 0) {
        /* This is not that big of a deal, just use defaults. */
        geelog_flog(LOG_WARN, sFunc, 
                "Failed to load configuration, using defaults.");
    }
    if (cheatengine_init(&rCe.rCheatEngine, &rCe.rConfig, NULL, NULL) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to initialize Cheat Engine.");
        return CEFIVE_FAILURE;
    }
    if (searchengine_start(&rCe.rSearchEngine) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to start Search Engine.");
        return CEFIVE_FAILURE;
    }
    rCe.rUi.prCEConfig = &rCe.rConfig;
    cefiveui_init(&rCe.rUi, &rCe.rCheatEngine, &rCe.rSearchEngine);
    
    /* Wait for the Kernel */
    if (wait_for_kernel(&rCe) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to wait for Kernel Library.");
        return CEFIVE_FAILURE;
    }
    
    /* Load the Game Id */
    if (ggame_load_gameid(&rCe.rGame) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to load Game Id.");
        return CEFIVE_FAILURE;
    }
    
    /* Load the Cheat Database */
    sprintf(sFile, "ms0:/%s/%s/%s.txt", rCe.rConfig.plugins_dir, 
            rCe.rConfig.cefive_dir, rCe.rGame.sGameId);
    if (niteprio_import(&rCe.rCheatEngine, sFile) < 0) {
        geelog_flog(LOG_WARN, sFunc, "Failed to load Cheat Database.");
    }
    
    /* Set up initial frame buffer */
    pspDebugScreenInitEx(0x44000000, 0, 0);
    rCe.rUi.vram = NULL;
    
    /* Set up controller */
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    
    /* Register Button Callbacks */
    if (register_button_callbacks(&rCe) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to register button callbacks.");
        return CEFIVE_FAILURE;
    }
    rCe.rRunState = CES_Running;
    
    /* While Running */
    while (rCe.rRunState == CES_Running) {
        /* Refresh the Cheat Engine */
        if (cheatengine_refresh(&rCe.rCheatEngine) < 0) {
            geelog_flog(LOG_ERROR, sFunc, "Error refreshing Cheat Engine.");
            rCe.rRunState = CES_Fault;
            continue;
        }
        /* Refresh the Search Engine */
        /* Poll for Events */
        sceKernelPollEventFlag(rCe.eventUID, 0xF, PSP_EVENT_WAITOR, &match);
        /* UI Request Event */
        /* UI Dismiss Event */
    }
    
    return CEFIVE_SUCCESS;
}

int cefive_start() {
    geelog_init("ms0:/seplugins/CEFive.log");
    geelog_start(LOG_DEBUG);
    krThreadId = sceKernelCreateThread(CEFIVE_THREAD_NAME, cefive_run, 
            CEFIVE_INIT_PRIO, CEFIVE_STACK_SIZE, 0, NULL);
    if (krThreadId < 0) {
        return CEFIVE_FAILURE;
    }
    sceKernelStartThread(krThreadId, 0, NULL);
    return CEFIVE_SUCCESS;
}

int cefive_stop() {
    if (geelog_stop() < 0) {
        return CEFIVE_FAILURE;
    }
    return CEFIVE_SUCCESS;
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

static int register_button_callbacks(CEFive* prCe) {
    unsigned int bmask = 0;
    
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    bmask |= PSP_CTRL_CIRCLE;
    bmask |= PSP_CTRL_CROSS;
    bmask |= PSP_CTRL_DOWN;
    bmask |= PSP_CTRL_LEFT;
    bmask |= PSP_CTRL_LTRIGGER;
    bmask |= PSP_CTRL_NOTE;
    bmask |= PSP_CTRL_RIGHT;
    bmask |= PSP_CTRL_RTRIGGER;
    bmask |= PSP_CTRL_SELECT;
    bmask |= PSP_CTRL_SQUARE;
    bmask |= PSP_CTRL_START;
    bmask |= PSP_CTRL_TRIANGLE;
    bmask |= PSP_CTRL_UP;
    bmask |= PSP_CTRL_VOLDOWN;
    bmask |= PSP_CTRL_VOLUP;
    
    sceCtrlRegisterButtonCallback(3, bmask, button_callback, (void*) prCe);
    
    return CEFIVE_SUCCESS;
}

static int wait_for_kernel(CEFive* prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    while (prCe->prKernelLib == NULL) {
        sceKernelDelayThread(100000);
        prCe->prKernelLib = sceKernelFindModuleByName(CEFIVE_KERNEL_LIB);
    }
    return CEFIVE_SUCCESS;
}
