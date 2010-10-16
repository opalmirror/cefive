#include <pspthreadman.h>
#include <psploadcore.h>
#include <pspiofilemgr.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspctrl_kernel.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include "cefiveui.h"
#include "cheatengine.h"
#include "searchengine.h"
#include "cefive.h"

static int displayLoaded(CEFive *prCe);
static int hideUi(CEFive *prCe);
static int initCtrl(CEFive* prCe);
static int initVram(CEFive* prCe);
static int pollGameId(CEFive *prCe);
static int pollGameModule(CEFive *prCe);
static int pollKernelLibrary(CEFive *prCe);
static int pollVram(CEFive *prCe);
static int showUi(CEFive *prCe);

void cefive_button_callback(int curr, int last, void* arg) {
    CEFive* prCe = (CEFive*) arg;
    int uireq = PSP_CTRL_VOLDOWN | PSP_CTRL_VOLUP;
    int chtog = PSP_CTRL_NOTE;
    CEFiveUi* prUi = NULL;
    CheatEngine* prEngine = NULL;

    if (prCe == NULL) {
        return;
    }
    prUi = prCe->prUi;
    if (prUi == NULL) {
        return;
    }

    if (prUi->vram == NULL) {
        return;
    }

    if (curr & uireq == uireq) {
        if (prUi->running == 0) {
            prCe->runState = CES_UIRequest;
        }
    }
    if (curr & chtog == chtog) {
        prEngine = prCe->prEngine;
        if (prEngine == NULL) {
            return;
        }
        if (prEngine->trigger_active == 0) {
            cheatengineActivateCheats(prEngine);
        } else {
            cheatengineDeactivateCheats(prEngine);
        }
    }
}

int cefive_init(CEFive* prCe){
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prCe->config.cefive_dir[0] = (char)0;
    prCe->config.cheatfile_path[0] = (char)0;
    prCe->config.plugins_dir[0] = (char)0;
    prCe->config.screenshot_path[0] = (char)0;
    prCe->config.pause_during_ui = 0;

    prCe->game_id[0] = (char)0;
    prCe->rStart = CESS_Fault;
    prCe->runState = CES_Stopped;
    prCe->running = 0;

    return CEFIVE_SUCCESS;
}

void cefiveRun(CEFive *prCe) {
    CEState runstate;
    CheatEngine* prEngine = NULL;
    SearchEngine* prSearch = NULL;
    CEFiveUi* prUi = NULL;
    if (prCe == NULL) {
        return;
    }
    prEngine = prCe->prEngine;
    if (prEngine == NULL) {
        return;
    }
    prSearch = prCe->prSearch;
    if (prSearch == NULL) {
        return;
    }
    prUi = prCe->prUi;
    if (prUi == NULL) {
        return;
    }
    while (prCe->running == 1) {
        runstate = prCe->runState;
        switch (runstate) {
            case CES_Fault:
                prCe->running = 0;
                break;
            case CES_Stopped:
                prCe->running = 0;
                break;
            case CES_Starting:
                cefiveStarting(prCe);
                break;
            case CES_Started:
                prCe->runState = CES_Running;
                break;
            case CES_Running:
                cheatengineRefresh(prEngine);
                searchengine_run(prSearch);
                break;
            case CES_UIRequest:
                if (showUi(prCe) == CEFIVE_SUCCESS) {
                    prCe->runState = CES_UIShowing;
                } else {
                    prCe->runState = CES_Fault;
                }
                break;
            case CES_UIShowing:
                cheatengineRefresh(prEngine);
                searchengine_run(prSearch);
                cefiveuiHandleInput(prUi);
                cefiveuiRedraw(prUi);
                break;
            case CES_UIDismissed:
                if (hideUi(prCe) == CEFIVE_SUCCESS) {
                    prCe->runState = CES_Running;
                } else {
                    prCe->runState = CES_Fault;
                }
                break;
            case CES_CheatUpdate:
                cheatengineRefresh(prEngine);
                prCe->runState = CES_Running;
                break;
            case CES_Stopping:
                prCe->runState = CES_Stopped;
                break;
        }
        sceKernelDelayThread(50000);
    }
    sceKernelExitDeleteThread(0);
}

int cefiveStart(CEFive *prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    if (prCe->runState != CES_Stopped) {
        return CEFIVE_FAILURE;
    }
    prCe->runState = CES_Starting;
    prCe->rStart = CESS_WaitKernelLib;
    prCe->running = 1;
    prCe->rThreadId = sceKernelCreateThread("CEFiveThread", &cefiveRun, 0x18,
            0x1000, 0, NULL);
    if (prCe->rThreadId >= 0) {
        sceKernelStartThread(prCe->rThreadId, 1, prCe);
    }

    return CEFIVE_SUCCESS;
}

int cefiveStarting(CEFive* prCe) {
    ECEStartState rSS = CESS_Fault;
    int r = 0;

    if (prCe == NULL) {
        return CEFIVE_FAILURE;
    }
    rSS = prCe->rStart;
    switch (rSS) {
        case CESS_Fault:
            prCe->runState = CES_Fault;
            break;
        case CESS_WaitKernelLib:
            r = pollKernelLibrary(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            break;
        case CESS_WaitGameId:
            r = pollGameId(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            break;
        case CESS_WaitGameModule:
            r = pollGameModule(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            break;
        case CESS_WaitCheatLoad:
            prCe->rStart = CESS_InitVram;
            break;
        case CESS_InitVram:
            r = initVram(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            break;
        case CESS_InitCtrl:
            r = initCtrl(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            break;
        case CESS_WaitVram:
            r = displayLoaded(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            r = pollVram(prCe);
            if (r != CEFIVE_SUCCESS) {
                prCe->rStart = CESS_Fault;
            }
            break;
        case CESS_Finished:
            prCe->runState = CES_Started;
            break;
    }

    return CEFIVE_SUCCESS;
}

int cefiveStop(CEFive *prCe) {
    if (prCe == NULL) {
        return CEFIVE_FAILURE;
    }
    prCe->runState = CES_Stopping;
    sceKernelTerminateDeleteThread(prCe->rThreadId);
    return CEFIVE_SUCCESS;
}

static int displayLoaded(CEFive *prCe) {
    int mode = 0;
    int width = 0;
    int height = 0;
    int r = 0;

    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    r = sceDisplayGetMode(&mode, &width, &height);
    if (r != 0) {
        return CEFIVE_FAILURE;
    }
    pspDebugScreenSetColorMode(mode);
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenSetBackColor((u32)0x00000000);
    pspDebugScreenSetTextColor((u32)0xFFFFFFFF);
    pspDebugScreenPuts("Cheat Engine of Five -- Press HOME Twice");

    return CEFIVE_SUCCESS;
}

static int hideUi(CEFive *prCe) {
    CEFiveUi* prUi = NULL;
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prUi = prCe->prUi;
    if (prUi == NULL) {
        return CEFIVE_NULLPTR;
    }
    pspDebugScreenSetBackColor((u32)0xFF000000);
    pspDebugScreenSetTextColor((u32)0xFFFFFFFF);
    pspDebugScreenClear();
    sceDisplaySetFrameBufferInternal(0, 0, 512, 0, 1);
    //Allow the game to receive input
    sceCtrlSetButtonMasks(0x10000, 0); // Unset HOME key
    sceCtrlSetButtonMasks(0xFFFF, 0); // Unset mask
    prUi->running = 0;
    prUi->drawn = 1;
    return CEFIVE_SUCCESS;
}

static int initCtrl(CEFive* prCe) {
    int mask = 0;
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    mask |= PSP_CTRL_NOTE;
    mask |= PSP_CTRL_VOLDOWN;
    mask |= PSP_CTRL_VOLUP;
    sceCtrlRegisterButtonCallback(3, mask, cefive_button_callback, prCe);
    prCe->rStart = CESS_WaitVram;
    return CEFIVE_SUCCESS;
}

static int initVram(CEFive* prCe) {
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    pspDebugScreenInitEx(0x44000000, 0, 0);
    
    prCe->rStart = CESS_InitCtrl;
    return CEFIVE_SUCCESS;
}

static int pollGameId(CEFive *prCe) {
    int fh = 0;

    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    fh = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
    if (fh != 0) {
        sceIoRead(fh, prCe->game_id, CEFIVE_GAMEID_LEN);
        sceIoClose(fh);
        prCe->rStart = CESS_WaitGameModule;
    }
    return CEFIVE_SUCCESS;
}

static int pollGameModule(CEFive *prCe) {
    SceModule* prGame = NULL;
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prGame = sceKernelFindModuleByAddress(0x08804000);
    if (prGame != NULL) {
        prCe->prGame = prGame;
        prCe->rStart = CESS_WaitCheatLoad;
    }
    return CEFIVE_SUCCESS;
}

static int pollKernelLibrary(CEFive *prCe) {
    SceModule* prKernel = NULL;
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prKernel = sceKernelFindModuleByName("sceKernelLibrary");
    if (prKernel != NULL) {
        prCe->prKernelLib = prKernel;
        prCe->rStart = CESS_WaitGameId;
    }
    return CEFIVE_SUCCESS;
}

static int pollVram(CEFive *prCe) {
    CEFiveUi* prUi = NULL;
    void* vram = NULL;
    int bufferwidth = 0;
    int pixelformat = 0;
    unsigned int dest = 0;
    int r;

    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prUi = prCe->prUi;
    if (prUi == NULL) {
        return CEFIVE_NULLPTR;
    }
    r = sceDisplayGetFrameBufferInternal(0, &dest, &bufferwidth, &pixelformat,
            PSP_DISPLAY_SETBUF_IMMEDIATE);
    if (dest != 0) {
        vram = (void*)(dest | 0xA0000000);
        prUi->vram = vram;
        prCe->rStart = CESS_Finished;
    }
    return CEFIVE_SUCCESS;
}

static int showUi(CEFive *prCe) {
    CEFiveUi* prUi = NULL;
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prUi = prCe->prUi;
    if (prUi == NULL) {
        return CEFIVE_NULLPTR;
    }
    if (prUi->vram == NULL) {
        return CEFIVE_NULLPTR;
    }
    int r = sceDisplaySetFrameBufferInternal(0, prUi->vram, 512, 0, 1);
    if (r != 0) {
        return CEFIVE_FAILURE;
    }
    //Stop the game from receiving input (USER mode input block)
    sceCtrlSetButtonMasks(0xFFFF, 1); // Mask lower 16bits
    sceCtrlSetButtonMasks(0x10000, 2); // Always return HOME key
    prUi->running = 1;
    prUi->drawn = 0;
    return CEFIVE_SUCCESS;
}

