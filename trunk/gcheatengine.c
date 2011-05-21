#include "gcheatengine.h"
#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include "cheat.h"
#include "block.h"
#include "cheatengine.h"

static int clear_blocks();
static int clear_cheats();
static int gcheatengine_run(int argl, void* argv);

static GCheatEngine krEngine;
static SceUID krThreadId;
static SceUID krMutexId;

static int clear_blocks() {
    Block* prBlock = NULL;
    int i = 0;
    
    sceKernelWaitSema(krMutexId, 1, NULL);
    for (i = 0; i < GCHEATENGINE_BLOCK_MAX; i++) {
        prBlock = &krEngine.arBlock[i];
        if (prBlock != NULL) {
            if (block_init(prBlock, 0, 0, 0) != BLOCK_SUCCESS) {
                sceKernelSignalSema(krMutexId, 1);
                return GCHEATENGINE_FAILURE;
            }
        }
    }
    sceKernelSignalSema(krMutexId, 1);
    return GCHEATENGINE_SUCCESS;
}

static int clear_cheats() {
    Cheat* prCheat = NULL;
    int i = 0;
    sceKernelWaitSema(krMutexId, 1, NULL);
    for (i = 0; i < CHEATENGINE_CHEAT_MAX; i++) {
        prCheat = &krEngine.arCheat[i];
        if (prCheat != NULL) {
            if (cheat_init(prCheat) != CHEAT_SUCCESS) {
                sceKernelSignalSema(krMutexId, 1);
                return GCHEATENGINE_FAILURE;
            }
        }
    }
    sceKernelSignalSema(krMutexId, 1);
    return GCHEATENGINE_SUCCESS;
}

Block* gcheatengine_get_block(int index) {
    Block* prBlock = NULL;
    if ((index >= 0) && (index < GCHEATENGINE_BLOCK_MAX)) {
        sceKernelWaitSema(krMutexId, 1, NULL);
        prBlock = &krEngine.arBlock[index];
        sceKernelSignalSema(krMutexId, 1);
    }
    return prBlock;
}

Cheat* gcheatengine_get_cheat(int index) {
    Cheat* prCheat = NULL;
    if ((index >= 0) && (index < GCHEATENGINE_CHEAT_MAX)) {
        sceKernelWaitSema(krMutexId, 1, NULL);
        prCheat = &krEngine.arCheat[index];
        sceKernelSignalSema(krMutexId, 1);
    }
    return prCheat;
}

int gcheatengine_init() {
    krEngine.iBlocks = 0;
    krEngine.iCheats = 0;
    krThreadId = -1;
    krMutexId = -1;
}

static int gcheatengine_run(int argl, void* argv) {
    if (krEngine.rRunState != ECHRS_Started) {
        return GCHEATENGINE_FAILURE;
    }
    while (krEngine.rRunState == ECHRS_Running) {
        
    }
    /* Deactivate all activated cheats. */
    /* Delete the Mutex */
    if (sceKernelDeleteSema(krMutexId) != 0) {
        return GCHEATENGINE_FAILURE;
    }
    return GCHEATENGINE_SUCCESS;
}

int gcheatengine_start() {
    if (krEngine.rRunState != ECHRS_Stopped) {
        return GCHEATENGINE_FAILURE;
    }
    krEngine.rRunState = ECHRS_Starting;
    krMutexId = sceKernelCreateSema(GCHEATENGINE_MUTEX_NAME, 0, 1, 1, NULL);
    if (krMutexId < 0) {
        return GCHEATENGINE_FAILURE;
    }
    
    krEngine.rRunState = ECHRS_Started;
    krThreadId = sceKernelCreateThread(GCHEATENGINE_THREAD_NAME, 
            gcheatengine_run, GCHEATENGINE_INIT_PRIO, GCHEATENGINE_STACK_SIZE, 
            0, NULL);
    if (krThreadId < 0) {
        return GCHEATENGINE_FAILURE;
    }
    sceKernelStartThread(krThreadId, 0, NULL);
    return GCHEATENGINE_SUCCESS;
}
