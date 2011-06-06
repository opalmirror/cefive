/* Cheat Engine of Five
 * Authors:
 * Sir Gee of Five
 * PSP_Lord
 */

//Crt0_prx
//Includes
//#define _PSP_FW_VERSION 150
#include "crt0_prx.h"

#define MAX_THREAD 64

PSP_MODULE_INFO("CEFive", 0x3007, CEFIVE_VERSION_MAJ, CEFIVE_VERSION_MIN);
PSP_MAIN_THREAD_ATTR(0); //0 for kernel mode too

extern SceUID sceKernelSearchModuleByName(unsigned char *);
static void drawLoadedScreen(u8 mode);
static void findGameId();
static void gamePause(SceUID thid);
static void gameResume(SceUID thid);
static void hideInterface();
unsigned int hookMac(unsigned char *a_mac);
static void setupController();
static void setupInitialVram();
static void showInterface();
static void start();
static void waitForKernelLibrary();
static void waitForVram();

static char gameDir[256];
static unsigned char gameId[10];
static unsigned char running = 0;
static SceUID krThreadId = 0;
static int thread_count_start = -1;
static int thread_count_now = -1;
static int krPauseId = -1;
static SceUID thread_buf_start[MAX_THREAD];
static SceUID thread_buf_now[MAX_THREAD];
static Block block[CHEATENGINE_BLOCK_MAX];
static Cheat cheat[CHEATENGINE_CHEAT_MAX];
static unsigned char cheatStatus = 0;
static unsigned int menuKey = PSP_CTRL_VOLUP | PSP_CTRL_VOLDOWN;
static unsigned int triggerKey = PSP_CTRL_NOTE;
static unsigned int screenKey = PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE;
static CheatEngine krCheatEngine;
static CEFiveUi krUi;
static CEFiveConfig krConfig;
static SearchEngine krSearchEngine;
static CEState krRunState;
static ECEStartState krStartState;

static Hook hookA[1] = { 
    { { 0, NULL }, 
    "sceWlan_Driver", 
    "sceWlanDrv", 
    0x0c622081,
    hookMac }, 
    };
static Hook hookB[1] = { 
    { { 0, NULL }, 
    "sceOpenPSID_Service", 
    "sceOpenPSID_driver",
    0xc69bebce, 
    NULL }, 
    };

static const unsigned char patchA[] = { 
    0x21, 0x88, 0x02, 0x3c, //lui v0, $8822
    0x21, 0x10, 0x42, 0x34, //ori v0, v0, $0008
    0x08, 0x00, 0x40, 0x00, //jr v0
    0x00, 0x00, 0x00, 0x00 };

//Mac Address hooking module
static unsigned char cfg[] = { 'C', 'F', 'G', 0x88, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x40,
        0x80, 0x00, 0x00 };

void buttonCallback(int curr, int last, void *arg) {
    int cvd = (curr & PSP_CTRL_VOLDOWN);
    int cvu = (curr & PSP_CTRL_VOLUP);
    int cno = (curr & PSP_CTRL_NOTE);
    int lvd = (last & PSP_CTRL_VOLDOWN);
    int lvu = (last & PSP_CTRL_VOLUP);
    int lno = (last & PSP_CTRL_NOTE);

    if (krUi.vram == NULL) {
        geelog_log(LOG_WARN, 
                "buttonCallback: Current VRAM pointer is NULL.");
        return;
    }

    /* If the UI is already showing, delegate to the ui. */
    if (krUi.running == 1) {
        CEFiveUi* prUi = &krUi;
        if (cvd && cvu && (!lvd || !lvu)) {
            geelog_log(LOG_DEBUG, 
                    "buttonCallback: User Interface Requested while visible.");
        }
        cefiveui_buttoncallback(curr, last, prUi);
        return;
    }

    /* If the Volume Up and Volume Down keys are being held */
    if (cvd && cvu && (!lvd || !lvu)) {
        geelog_log(LOG_DEBUG,
                "buttonCallback: User Interface Requested.");
        krUi.drawn = 0;
        krRunState = CES_UIRequest;
    }

    /* If the Music Button has been pressed. */
    if (cno && !lno) {
        cheatStatus = !cheatStatus;
        if (krCheatEngine.trigger_active == 0) {
            geelog_log(LOG_DEBUG, "buttonCallback: Activating Cheats.");
            cheatengineActivateCheats(&krCheatEngine);
        } else {
            geelog_log(LOG_DEBUG, 
                    "buttonCallback: Deactivating Cheats.");
            cheatengineDeactivateCheats(&krCheatEngine);
        }
        sceKernelDelayThread(500000);
    }
}

/** Draw the Loaded Screen
*
* @param mode u8 specifying the color mode assigned in waitForVram()
*/
static void drawLoadedScreen(u8 mode) {
    u32 text = (u32)0x00000000;
    static int r = 0;
    static int d = 1;
    
    pspDebugScreenSetColorMode(mode);
    pspDebugScreenSetXY(0, 0);
    r += 1 * d;
    if (r < 0) {
        r = 0;
        d = 1;
    }
    if (r > 0xFF) {
        r = 0xFF;
        d = -1;
    }
    text |= r;
    pspDebugScreenSetTextColor(text);
    pspDebugScreenPuts("Cheat Engine of Five -- Press HOME Twice");
}

static void findGameId() {
    signed int fd;
    //Find the GAME ID
    do {
        fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
        sceKernelDelayThread(1000);
    } while (fd <= 0);
    sceIoRead(fd, gameId, 10);
    sceIoClose(fd);
    sprintf(gameDir, "ms0:/seplugins/nitePR/%s.txt", gameId);
    sprintf(krUi.game_id, "%s", gameId);
    strcpy(krConfig.game_id, gameId);
}

static void gamePause(SceUID thid) {
    if (krPauseId >= 0)
        return;
    krPauseId = thid;
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, thread_buf_now,
            MAX_THREAD, &thread_count_now);
    int x, y, match;
    for (x = 0; x < thread_count_now; x++) {
        match = 0;
        SceUID tmp_thid = thread_buf_now[x];
        for (y = 0; y < thread_count_start; y++) {
            if ((tmp_thid == thread_buf_start[y]) || (tmp_thid == thid)) {
                match = 1;
                break;
            }
        }
        if (match == 0)
            sceKernelSuspendThread(tmp_thid);
    }
}

static void gameResume(SceUID thid) {
    if (krPauseId != thid)
        return;
    krPauseId = -1;
    int x, y, match;
    for (x = 0; x < thread_count_now; x++) {
        match = 0;
        SceUID tmp_thid = thread_buf_now[x];
        for (y = 0; y < thread_count_start; y++) {
            if ((tmp_thid == thread_buf_start[y]) || (tmp_thid == thid)) {
                match = 1;
                break;
            }
        }
        if (match == 0)
            sceKernelResumeThread(tmp_thid);
    }
}

/* Hide the Cheat Engine of Five interface, return to the game screen, return
 * input to the game, attempt to save the configuration.
 */
static void hideInterface() {
    CEFiveConfig* prConfig = &krConfig;
    
    geelog_log(LOG_DEBUG, "hideInterface: Clearing Interface.");
    pspDebugScreenSetBackColor((u32)0x00000000);
    pspDebugScreenClear();
    
    geelog_log(LOG_DEBUG, "hideInterface: Returning frame buffer.");
    //Return the standard VRAM
    sceDisplaySetFrameBufferInternal(0, 0, 512, 0, 1);

    geelog_log(LOG_DEBUG, "hideInterface: Clearing input mask.");
    //Allow the game to receive input
    sceCtrlSetButtonMasks(0x10000, 0); // Unset HOME key
    sceCtrlSetButtonMasks(0xFFFF, 0); // Unset mask
    
    geelog_log(LOG_DEBUG, "hideInterface: Saving configuration.");
    /* Try to save the current CEFiveConfig. */
    cefiveconfig_save(prConfig, "ms0:/seplugins/CEFive.cdf");
    krRunState = CES_Running;
    geelog_log(LOG_DEBUG, "hideInterface: Resuming Game.");
    gameResume(krThreadId);
}

unsigned int hookMac(unsigned char *a_mac) {
    memcpy(a_mac, cfg + 5, 6);
    return 0;
}

/* Main thread of cefive.
 */
int mainThread() {
    const char* sFunc = "mainThread";
    CEFiveUi* prUi = &krUi;
    SearchEngine* prSearch = &krSearchEngine;
    CheatEngine* prEngine = &krCheatEngine;
    GameInfo* prInfo = &prUi->gameinfo;

    running = 1;

    geelog_flog(LOG_DEBUG, sFunc, "Starting CEFive.");
    start();
    geelog_flog(LOG_DEBUG, sFunc, "CEFive Started.");

    //Do the loop-de-loop
    while (running) {
        if (prUi->vram == NULL) {
            waitForVram();
            continue;
        }
        if (prInfo->loaded == 0) {
            geelog_flog(LOG_DEBUG, sFunc, "Loading Game Info.");
            gameinfo_load(prInfo);
        }
        if (krRunState == CES_UIRequest) {
            geelog_flog(LOG_DEBUG, sFunc, "Showing Interface.");
            showInterface();
            while (prUi->running == 1) {
                if (prUi->vram == NULL) {
                    geelog_flog(LOG_DEBUG, sFunc, 
                            "Invalid VRAM Pointer, shutting down UI.");
                    prUi->running = 0;
                    continue;
                }
                if (krRunState == CES_UIRequest) {
                    geelog_flog(LOG_DEBUG, sFunc, 
                            "UI Requested, Showing Interface.");
                    showInterface();
                }
                cefiveui_update_controls(prUi);
                cefiveuiRedraw(prUi);
                searchengine_run(prSearch);
                cheatengine_refresh(prEngine);
                sceKernelDelayThread(50000);
            }
            geelog_flog(LOG_DEBUG, sFunc, "Hiding Interface.");
            hideInterface();
        }
        searchengine_run(prSearch);
        cheatengine_refresh(prEngine);
        sceKernelDelayThread(50000);
    }
    
    geelog_flog(LOG_INFO, sFunc, "CEFive Stopped.");
    geelog_stop();
    return 0;
}

int module_start(SceSize args, void *argp) {
    //Load the CFG
    if (cfg[4]) {
        SceModule *mod;
        while (1) {
            mod = sceKernelFindModuleByName(hookA[0].modname);
            if (mod == NULL) {
                sceKernelDelayThread(100);
                continue;
            }
            break;
        }
        moduleHookFunc(&hookA[0].modfunc, sceKernelSearchModuleByName(
                hookA[0].modname), hookA[0].libname, hookA[0].nid,
                hookA[0].func);
    }
    memcpy(&triggerKey, cfg + 11, 4);
    memcpy(&menuKey, cfg + 15, 4);
    memcpy(&screenKey, cfg + 20, 4);

    /* Start the Logger. */
    geelog_init("ms0:/seplugins/CEFive.log");
    geelog_start(LOG_ERROR);
    
    //Create thread
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, thread_buf_start,
            MAX_THREAD, &thread_count_start);
    krThreadId = sceKernelCreateThread("CEFive_Thread", &mainThread, 0x18, 0x1000, 0,
            NULL);

    //Start thread
    if (krThreadId >= 0)
        sceKernelStartThread(krThreadId, 0, NULL);

    return 0;
}

int module_stop(SceSize args, void *argp) {
    running = 0;
    geelog_stop();
    sceKernelTerminateThread(krThreadId);
    return 0;
}

int pollFrameBuffer() {
    unsigned int address = 0;
    int bufferwidth = 0;
    int pixelformat = 0;
    int r = 0;

    r = sceDisplayGetFrameBufferInternal(0, &address, &bufferwidth,
            &pixelformat, PSP_DISPLAY_SETBUF_IMMEDIATE);
    if (address == 0) {
        krUi.running = 0;
        krUi.vram = NULL;
    }
    return r;
}

static void setupController() {
    //Setup the controller
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

static void setupInitialVram() {
    //Set the VRAM to null, use the current screen
    pspDebugScreenInitEx(0x44000000, 0, 0);
    //vram = NULL;
    krUi.vram = NULL;
}

/* Pause the game if configured to do so, stop the game from receiving button
 * input, then swap the vram for the debug frame buffer, and initialize the 
 * screen.
 */
static void showInterface() {
    CEFiveUi* prUi = &krUi;
    CEFiveConfig* prConfig = &krConfig;

    if (prConfig->pause_during_ui == 1) {
        geelog_log(LOG_DEBUG, "showInterface: Pausing Game.");
        gamePause(krThreadId);
    }
    
    geelog_log(LOG_DEBUG, "showInterface: Blocking input from Game.");
    //Stop the game from receiving input (USER mode input block)
    sceCtrlSetButtonMasks(0xFFFF, 1); // Mask lower 16bits
    sceCtrlSetButtonMasks(0x10000, 2); // Always return HOME key

    geelog_log(LOG_DEBUG, "showInterface: Initializing frame buffer.");
    //Setup a custom VRAM
    sceDisplaySetFrameBufferInternal(0, prUi->vram, 512, 0, 1);
    pspDebugScreenInitEx(prUi->vram, 0, 0);
    pspDebugScreenSetMaxX(69);
    pspDebugScreenSetMaxY(34);
    pspDebugScreenClearLineDisable();
    prUi->running = 1;
    geelog_log(LOG_DEBUG, 
            "showInterface: Setting Run State to UI Showing.");
    krRunState = CES_UIShowing;
}

static void start() {
    CEFiveConfig* prCfg = &krConfig;
    CheatEngine* prEng = &krCheatEngine;
    SearchEngine* prSearch = &krSearchEngine;
    CEFiveUi* prUi = &krUi;
    int r = 0;

    /* Initialize the CeFive Configuration. */
    geelog_log(LOG_DEBUG, "start: Initializing CEFiveConfig.");
    cefiveconfig_init(prCfg);
    
    /* Attempt to load the current configuration. */
    geelog_log(LOG_DEBUG, "start: Loading configuration.");
    r = cefiveconfig_load(prCfg, "ms0:/seplugins/CEFive.cdf");
    if (r != CEFIVECONFIG_SUCCESS) {
        geelog_log(LOG_ERROR, "start: Error loading configuration.");
    } else {
        geelog_log(LOG_INFO, "start: Loaded configuration.");
    }
    sprintf(prCfg->plugins_dir, "seplugins");
    sprintf(prCfg->cefive_dir, "nitePR");

    geelog_log(LOG_DEBUG, "start: Initializing CheatEngine.");
    /* Initialize the CheatEngine */
    r = cheatengine_init(prEng, prCfg, cheat, block);

    geelog_log(LOG_DEBUG, "start: Initializing SearchEngine.");
    // Initialize the SearchEngine
    searchengine_init(prSearch);
    searchengine_start(prSearch);

    geelog_log(LOG_DEBUG, "start: Initializing UI.");
    /* Initialize the UI */
    prUi->prCEConfig = prCfg;
    cefiveui_init(prUi, prEng, prSearch);
    krRunState = CES_Starting;
    krStartState = CESS_WaitKernelLib;

    geelog_log(LOG_DEBUG, "start: waiting for Kernel.");
    waitForKernelLibrary();
    geelog_log(LOG_DEBUG, "start: loading Game Id.");
    findGameId();
    geelog_log(LOG_DEBUG, "start: loading cheats.");
    niteprio_import(prEng, gameDir);
    
    geelog_log(LOG_DEBUG, "start: Getting Initial VRAM.");
    setupInitialVram();
    geelog_log(LOG_DEBUG, "start: Initializing Controller.");
    setupController();

    geelog_log(LOG_DEBUG, "start: Registering Button Callbacks.");
    //Register the button callbacks
    unsigned int bmask = 0;
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

    sceCtrlRegisterButtonCallback(3, bmask, buttonCallback, NULL);
    geelog_log(LOG_DEBUG, "start: started.");
}

static void waitForKernelLibrary() {
    //Wait for the kernel to boot
    sceKernelDelayThread(100000);
    while (!sceKernelFindModuleByName("sceKernelLibrary"))
        sceKernelDelayThread(100000);
    sceKernelDelayThread(100000);
}

/** Wait for Video RAM to be available.  Display a message over the default
 * active FrameBuffer prompting the user to press the HOME button twice.
 * 
 */
static void waitForVram() {
    //Has the HOME button been pressed?
    unsigned int a_address = 0;
    unsigned int a_bufferWidth = 0;
    unsigned int a_pixelFormat = 0;
    unsigned int a_sync;
    
    /* variable to hold the pixelFormat of default frame buffer */
    unsigned int a_pixelFormat2 = 0;
    
    /* retrieve the pixel format of the default frame buffer we setted up to print the info line */
    sceDisplayGetFrameBuf(&a_address, &a_bufferWidth, &a_pixelFormat2, &a_sync);

    sceDisplayGetFrameBufferInternal(0, &a_address, &a_bufferWidth,
            &a_pixelFormat, PSP_DISPLAY_SETBUF_IMMEDIATE);
       

    if (a_address) {
        krUi.vram = (void*) (0xA0000000 | a_address);
        krStartState = CESS_Finished;     
    }
        /* apply the correct color mode so the info line will be printed in correct size */
        else if(a_pixelFormat2 > 2) {
            drawLoadedScreen(3);
        }
    else {
            drawLoadedScreen(0);
    }
    sceDisplayWaitVblank();
}
