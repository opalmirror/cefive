#include "cheatengine.h"
#include "block.h"
#include "cheat.h"
#include "geelog.h"
#include <pspkernel.h>
#include <pspkerneltypes.h>
#include <stdio.h>
#include <pspiofilemgr.h>

static SceUInt32 parseDword(const char* sBuf);
static int parseName(const char* buffer, char* sName, size_t maxlen);

/* Activate all Cheats that are currently inactive.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Cheats have been activated.
 */
int cheatengineActivateCheats(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    int iCheat = 0;
    char sMsg[256];

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }

    cheatengineLog(prEng, LOG_DEBUG, 
            "cheatEngineActivateCheats: Activating Cheats.");
    for (iCheat = 0; iCheat < prEng->cheat_count; iCheat++) {
        prCheat = cheatengineGetCheat(prEng, iCheat);
        if (prCheat == NULL) {
            sprintf(sMsg, "%s: Cheat %d is NULL.", 
                    "cheatEngineActivateCheats", iCheat);
            cheatengineLog(prEng, LOG_WARN, sMsg);
            continue;
        }
        if (cheat_is_inactive(prCheat) == 0) {
            sprintf(sMsg, "%s: Applying Cheat %d.", 
                    "cheatEngineActivateCheat", iCheat);
            cheatengineLog(prEng, LOG_INFO, sMsg);
            cheatengineApplyCheat(prEng, iCheat);
        }
    }
    prEng->trigger_active = 1;
    cheatengineLog(prEng, LOG_DEBUG, "cheatengineActivate: Cheats Activated.");

    return CHEATENGINE_SUCCESS;
}

/* Add and return a pointer to a new Block in a Cheat Engine by specifying the
 * values of the Block to add.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   flags      8-bit Block flags (see Block.h)
 *   addr       32-bit unsigned integer containing the address of the Block.
 *   value      32-bit unsigned integer containing the patch value of the Block.
 * 
 * Returns:
 *   A pointer to the newly created Block struct is returned.
 *   NULL is returned if the Block could not be created.
 */
Block* cheatengineAddBlock(CheatEngine *prEng, unsigned char flags, 
        unsigned int addr, unsigned int value) {
    Block *prDest = NULL;
    if (prEng == NULL) {
        return prDest;
    }
    prDest = &(prEng->blocklist[prEng->block_count]);
    prDest->flags = flags;
    prDest->address = addr;
    prDest->hakVal = value;
    prEng->block_count++;
    cheatengineLog(prEng, LOG_DEBUG, "cheatengineAddBlock: Block added.");
    return prDest;
}

/* Add a Cheat to the specified Cheat Engine returning a pointer to the newly
 * added Cheat struct.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   A pointer to a Cheat struct representing the newly added Cheat is returned.
 *   NULL is returned if a Cheat could not be added.
 */
Cheat* cheatengineAddCheat(CheatEngine *prEng) {
    Cheat *prDest = NULL;
    int ct = 0;
    
    if (prEng == NULL) {
        return prDest;
    }
    ct = prEng->cheat_count;
    ct++;
    if (ct < CHEATENGINE_CHEAT_MAX) {
        prDest = &(prEng->cheatlist[prEng->cheat_count]);
        cheat_set_length(prDest, 0);
        cheat_set_block(prDest, prEng->block_count);
        cheat_setflag_fresh(prDest);
        prEng->cheat_count = ct;
        cheatengineLog(prEng, LOG_DEBUG, "cheatengineAddCheat: Cheat added.");
    } else {
        cheatengineLog(prEng, LOG_WARN,
                "cheatengineAddCheat: cheat_count at max, cannot add Cheat.");
    }
    return prDest;
}

/* Apply the specified Block to memory.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int specifying the Block in the Block array to apply.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Block pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is applied to memory.
 */
int cheatengineApplyBlock(CheatEngine *prEng, int index) {
    int r = 0;
    char sMsg[256];
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    Block *prBlock = cheatengineGetBlock(prEng, index);
    if (prBlock == NULL) {
        cheatengineLog(prEng, LOG_WARN,
               "cheatengineApplyBlock: Unable to get specified Block pointer.");
        return CHEATENGINE_FAILURE;
    }
    if (block_is_byte(prBlock)) {
        r = cheatengineApplyUChar8Block(prBlock);
    }
    if (block_is_word(prBlock)) {
        r = cheatengineApplyUShort16Block(prBlock);
    }
    if (block_is_dword(prBlock)) {
        r = cheatengineApplyUInt32Block(prBlock);
    }
    sprintf(sMsg, "%s: Applied Block %d.", "cheatengineApplyBlock", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    return CHEATENGINE_SUCCESS;
}

/* Apply the specified Cheat to memory.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int specifying the Cheat in the Cheat array to apply.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL, 
 *      or if the Cheat could not be applied to memory.
 *   CHEATENGINE_SUCCESS is returned if the Cheat is applied to memory.
 */
int cheatengineApplyCheat(CheatEngine *prEng, int index) {
    int iBlock = 0;
    int iEnd = 0;
    int iStart = 0;
    int r = CHEATENGINE_SUCCESS;
    char sMsg[256];

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    Cheat* prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        sprintf(sMsg, "%s: Cheat %d is NULL.", "cheatEngineApplyCheat", index);
        cheatengineLog(prEng, LOG_WARN, sMsg);
        return CHEATENGINE_FAILURE;
    }
    iStart = prCheat->block;
    sprintf(sMsg, "%s: Cheat starts at Block %d.", 
            "cheatengineApplyCheat", iStart);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    iEnd = prCheat->len + iStart;
    sprintf(sMsg, "%s: Cheat ends at Block %d.",
            "cheatengineApplyCheat", iEnd);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    for (iBlock = iStart; iBlock < iEnd; iBlock++) {
        r = cheatengineApplyBlock(prEng, iBlock);
        if (r != CHEATENGINE_SUCCESS) {
            sprintf(sMsg, "%s: Failed to apply Block %d.", 
                    "cheatengineApplyCheat", iBlock);
            cheatengineLog(prEng, LOG_WARN, sMsg);
            break;
        }
    }
    cheat_setflag_fresh(prCheat);
    sprintf(sMsg, "%s: Applied Cheat %d.", "cheatengineApplyCheat", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    return r;
}

/* Apply the specified Block to memory as a single byte.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct representing the Cheat Block.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
 *      or if the memory already contains the value that would have been
 *      applied.
 */
int cheatengineApplyUChar8Block(Block* prBlock) {
    if (prBlock == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    SceUChar8* pDest = (SceUChar8*)(prBlock->address | 0x40000000);
    SceUChar8 val = (SceUChar)(prBlock->hakVal & 0xFF);
    if (*pDest == val) {
        return CHEATENGINE_SUCCESS;
    }
    prBlock->stdVal = (unsigned int)*pDest;
    *pDest = val;
    sceKernelDcacheWritebackInvalidateRange(pDest, 1);
    sceKernelIcacheInvalidateRange(pDest, 1);

    return CHEATENGINE_SUCCESS;
}

/* Apply the specified Block to memory as a 32-bit value.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct representing the Cheat Block.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
 *      or if the memory already contains the value that would have been
 *      applied.
 */
int cheatengineApplyUInt32Block(Block* prBlock) {
    if (prBlock == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    SceUInt32* pDest = (SceUInt32*)(prBlock->address | 0x40000000);
    SceUInt32 val = (SceUInt32)(prBlock->hakVal & 0xFFFFFFFF);
    if (*pDest == val) {
        return CHEATENGINE_SUCCESS;
    }
    prBlock->stdVal = (unsigned int)*pDest;
    *pDest = val;
    sceKernelDcacheWritebackInvalidateRange(pDest, 4);
    sceKernelIcacheInvalidateRange(pDest, 4);

    return CHEATENGINE_SUCCESS;
}

/* Apply the specified Block to memory as a 16-bit value.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct representing the Cheat Block.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
 *      or if the memory already contains the value that would have been
 *      applied.
 */
int cheatengineApplyUShort16Block(Block* prBlock) {
    if (prBlock == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    SceUShort16* pDest = (SceUShort16*)(prBlock->address | 0x40000000);
    SceUShort16 val = (SceUShort16)(prBlock->hakVal & 0xFFFF);
    if (*pDest == val) {
        return CHEATENGINE_SUCCESS;
    }
    prBlock->stdVal = (unsigned int)*pDest;
    *pDest = val;
    sceKernelDcacheWritebackInvalidateRange(pDest, 2);
    sceKernelIcacheInvalidateRange(pDest, 2);

    return CHEATENGINE_SUCCESS;
}

/* Deactivate all of the currently activated Cheats that are selected.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Cheats have been deactivated.
 * 
 */
int cheatengineDeactivateCheats(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    int iCheat = 0;
    char sMsg[256];

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }

    cheatengineLog(prEng, LOG_DEBUG, 
            "cheatengineDeactivateCheats: Deactivating Cheats.");
    for (iCheat = 0; iCheat < prEng->cheat_count; iCheat++) {
        prCheat = cheatengineGetCheat(prEng, iCheat);
        if (prCheat == NULL) {
            sprintf(sMsg, "%s: Cheat %d is NULL.",
                    "cheatengineDeactivateCheats", iCheat);
            cheatengineLog(prEng, LOG_WARN, sMsg);
            continue;
        }
        if (cheat_is_selected(prCheat) == 1) {
            sprintf(sMsg, "%s: Deactivating Cheat %d.",
                    "cheatengineDeactivateCheats", iCheat);
            cheatengineLog(prEng, LOG_DEBUG, sMsg);
            cheatengineResetCheat(prEng, iCheat);
        }
    }
    prEng->trigger_active = 0;
    cheatengineLog(prEng, LOG_DEBUG, 
            "cheatengineDeactivateCheats: Cheats deactivated.");

    return CHEATENGINE_SUCCESS;
}

/* Return a pointer to the specified Block from a Cheat Engine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Block to return.
 * 
 * Returns:
 *   NULL is returned if prEng is NULL, or if the specified index is out of
 *      bounds.
 *   A Pointer to the specified Block struct is returned otherwise.
 */
Block* cheatengineGetBlock(CheatEngine *prEng, int index) {
    Block *prDest = NULL;
    if (prEng == NULL || index < 0) {
        return prDest;
    }
    if (index >= prEng->block_count) {
        cheatengineLog(prEng, LOG_WARN, 
                "cheatengineGetBlock: Index out of bounds.");
        return prDest;
    }
    prDest = &(prEng->blocklist[index]);
    return prDest;
}

/* Return a pointer to the specified Cheat from a Cheat Engine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to return.
 * 
 * Returns:
 *   NULL is returned if prEng is NULL, or if the specified index is out of
 *      bounds.
 *   A Pointer to the specified Cheat struct is returned otherwise.
 */
Cheat* cheatengineGetCheat(CheatEngine *prEng, int index) {
    Cheat *prDest = NULL;
    if (prEng == NULL || index < 0) {
        return prDest;
    }
    if (index >= prEng->cheat_count) {
        cheatengineLog(prEng, LOG_WARN, 
                "cheatengineGetCheat: index out of bounds.");
        return prDest;
    }
    prDest = &(prEng->cheatlist[index]);
    return prDest;
}

/* Initialize a CheatEngine struct by specifying initial pointers.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct to initialize.
 *   prCfg      Pointer to a CEFiveConfig struct containing configuration
 *              settings.
 *   arCheat    Pointer to the first element of an array of Cheat structs.
 *   arBlock    Pointer to the first element of an array of Block structs.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng or prCfg are NULL.
 *   CHEATENGINE_SUCCESS is returned if the struct is initialized.
 */
int cheatengineInit(CheatEngine* prEng, CEFiveConfig* prCfg, Cheat* arCheat,
        Block* arBlock) {
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    if (prCfg == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prEng->prConfig = prCfg;
    prEng->cheatlist = arCheat;
    prEng->blocklist = arBlock;
    prEng->cheat_count = 0;

    return CHEATENGINE_SUCCESS;
}

/* Load initial Cheats and Blocks from the configured cheatfile_path of the
 * specified Cheat Engine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL or if the Configuration
 *      pointer within prEng is NULL.
 *   CHEATENGINE_FILEIO is returned if an I/O Error occurs.
 *   CHEATENGINE_SUCCESS is returned if the file is loaded.
 */
int cheatengineLoadCheats(CheatEngine* prEng) {
    CEFiveConfig* prCfg = NULL;
    char* sPlugin = NULL;
    char* sDir = NULL;
    char* sGameId = NULL;
    SceUID fh = 0;
    SceOff rOffset = 0;

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCfg = prEng->prConfig;
    if (prCfg == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    sPlugin = prCfg->plugins_dir;
    sDir = prCfg->cefive_dir;
    sGameId = prCfg->game_id;
    sprintf(prCfg->cheatfile_path, "ms0:/%s/%s/%s.txt", sPlugin, sDir, sGameId);

    fh = sceIoOpen(prCfg->cheatfile_path, PSP_O_RDONLY, 0777);
    if (fh == 0) {
        return CHEATENGINE_FILEIO;
    }
    rOffset = sceIoLseek(fh, 0, SEEK_END);
    sceIoLseek(fh, 0, SEEK_SET);

    sceIoClose(fh);
    return CHEATENGINE_SUCCESS;
}

/* Add a log message to the current Cheat Engine Logger if applicable.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing a Cheat Engine.
 *   rLevel     ELogLevel enum value indicating log level of message.
 *   sMsg       String containing message to log.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR if prEng is NULL.
 *   CHEATENGINE_FAILURE if the Cheat Engine has no logger.
 *   CHEATENGINE_SUCCESS if the message was sent to the logger.
 */
int cheatengineLog(CheatEngine* prEng, ELogLevel rLevel, const char* sMsg) {
    GeeLog *prLog = NULL;
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    
    prLog = prEng->logger;
    if (prLog == NULL) {
        return CHEATENGINE_FAILURE;
    }
    
    geelog_log(prLog, rLevel, sMsg);
    
    return CHEATENGINE_SUCCESS;
}

int cheatengine_parsebuffer(CheatEngine* prEngine, const char* buffer,
        size_t len) {
    int bytes = 0;
    char c = (char)0;
    static int incomment = 0;
    static int inname = 0;
    static int incheat = 0;
    static int namelen = 0;
    static int curindex = 0;
    Cheat* prCheat = NULL;

    while (bytes < len) {
        c = *(buffer + bytes);
        switch (c) {
            case '\r':
            case '\n':
                if (incomment == 1) {
                    incomment = 0;
                }
                break;
            case '#':
                if (incomment == 0) {
                    if (inname == 0) {
                        if (incheat == 0) {
                            curindex = prEngine->cheat_count;
                            prCheat = cheatengineAddCheat(prEngine);
                            incheat = 1;
                            inname = 1;
                            namelen = 0;
                        }
                    }
                }
                break;
            case '!':
                if (incomment == 0) {
                    if (incheat == 1) {
                        if (inname == 0) {
                            if (prCheat != NULL) {
                                if (cheat_is_selected(prCheat) == 1) {
                                    cheat_set_constant(prCheat);
                                } else {
                                    cheat_set_selected(prCheat);
                                }
                            }
                        } else {
                            if (prCheat != NULL) {
                                prCheat->name[namelen] = c;
                                namelen++;
                            }
                        }
                    }
                }
                break;
            case ';':
                if (incomment == 0) {
                    if (inname == 0) {
                        incomment = 1;
                    }
                }
                break;
            default:
                if (incomment == 0) {
                    if (incheat == 1) {
                        if (inname == 1) {
                            if (prCheat != NULL) {
                                prCheat->name[namelen] = c;
                                namelen++;
                            }
                        }
                    }
                }
                break;
        }
        bytes++;
    }
    return bytes;
}

int cheatengineReadline(SceUID rFh, char* buffer) {
    int len = 0;
    int ci = 0;
    char c = (char)0;
    int end = 0;
    int r = 0;

    if (buffer == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    while (end == 0) {
        r = sceIoRead(rFh, buffer + ci, 1);
        len += r;
        if (r == 0) {
            end = 1;
            continue;
        }
        c = *(buffer + ci);
        if (c == '\n' || c == '\r') {
            end = 1;
        }
        ci++;
    }
    return len;
}

/* Refresh the specified Cheat Engine, Applying or Resetting cheats as 
 * necessary.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Cheat Engine is refreshed.
 */
int cheatengineRefresh(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    int iCheat = 0;
    char sMsg[256];

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    for (iCheat = 0; iCheat < prEng->cheat_count; iCheat++) {
        prCheat = cheatengineGetCheat(prEng, iCheat);
        if (prCheat == NULL) {
            sprintf(sMsg, "%s: Cheat %d pointer is NULL.", 
                    "cheatengineRefresh", iCheat);
            cheatengineLog(prEng, LOG_WARN, sMsg);
            continue;
        }
        // If the Cheats are Active
        if (prEng->trigger_active == 1) {
            if (cheat_is_fresh(prCheat) == 0) {
                if (cheat_is_inactive(prCheat) == 1) {
                    cheatengineResetCheat(prEng, iCheat);
                } else {
                    cheatengineApplyCheat(prEng, iCheat);
                }
            }
        }
    }

    return CHEATENGINE_SUCCESS;
}

/* Reset a previously activated Block by specifying the index.  A Block that
 * is Reset will return memory to it's value when the Block was applied.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the Block pointer specified by index is
 *      NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Block has been Reset.
 */
int cheatengineResetBlock(CheatEngine *prEng, int index) {
    int r = 0;
    char sMsg[256];
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    Block *prBlock = cheatengineGetBlock(prEng, index);
    if (prBlock == NULL) {
        sprintf(sMsg, "%s: Block %d pointer is NULL.", 
                "cheatengineResetBlock", index);
        cheatengineLog(prEng, LOG_WARN, sMsg);
        return CHEATENGINE_FAILURE;
    }
    if (block_is_byte(prBlock)) {
        cheatengineResetUChar8Block(prBlock);
    }
    if (block_is_word(prBlock)) {
        cheatengineResetUShort16Block(prBlock);
    }
    if (block_is_dword(prBlock)) {
        cheatengineResetUInt32Block(prBlock);
    }

    sprintf(sMsg, "%s: Reset Block %d.", "cheatengineResetBlock", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    return CHEATENGINE_SUCCESS;
}

/* Reset a previously activated Cheat by specifying the index.  A Cheat that
 * is Reset will have each of the Blocks belonging to the Cheat Reset.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the Cheat pointer specified by index is
 *      NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat has been Reset.
 */
int cheatengineResetCheat(CheatEngine *prEng, int index) {
    int iBlock = 0;
    int iEnd = 0;
    int iStart = 0;
    int r = CHEATENGINE_SUCCESS;
    char sMsg[256];

    if (prEng == NULL) {
        return CHEATENGINE_FAILURE;
    }
    Cheat* prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        sprintf(sMsg, "%s: Cheat %d pointer is NULL.", 
                "cheatengineResetCheat", index);
        cheatengineLog(prEng, LOG_WARN, sMsg);
        return CHEATENGINE_FAILURE;
    }
    iStart = prCheat->block;
    sprintf(sMsg, "%s: Cheat %d Starting Block is %d.",
            "cheatengineResetCheat", index, iStart);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    iEnd = prCheat->len + iStart;
    sprintf(sMsg, "%s: Cheat %d Ending Block is %d.",
            "cheatengineResetCheat", index, iEnd);
    for (iBlock = iStart; iBlock < iEnd; iBlock++) {
        r = cheatengineResetBlock(prEng, iBlock);
        if (r != CHEATENGINE_SUCCESS) {
            sprintf(sMsg, "%s: Failed to reset Block %d.", 
                    "cheatengineResetCheat", iBlock);
            cheatengineLog(prEng, LOG_WARN, sMsg);
            break;
        }
    }
    cheat_setflag_fresh(prCheat);
    sprintf(sMsg, "%s: Cheat %d Reset.", "cheatengineResetCheat", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    return r;
}

/* Reset the specified Block by returning the memory to its original value.
 * The memory is replaced as an 8-bit value.
 * 
 * Parameters:
 *   prBlock        Pointer to a Block struct representing the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is reset.
 */
int cheatengineResetUChar8Block(Block* prBlock) {
    if (prBlock == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    SceUChar8* pDest = (SceUChar8*)prBlock->address;
    SceUChar8 val = (SceUChar)(prBlock->stdVal & 0xFF);
    if (*pDest == val) {
        return CHEATENGINE_SUCCESS;
    }
    *pDest = val;
    sceKernelDcacheWritebackInvalidateRange(pDest, 1);
    sceKernelIcacheInvalidateRange(pDest, 1);

    return CHEATENGINE_SUCCESS;
}

/* Reset the specified Block by returning the memory to its original value.
 * The memory is replaced as a 32-bit value.
 * 
 * Parameters:
 *   prBlock        Pointer to a Block struct representing the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is reset.
 */
int cheatengineResetUInt32Block(Block* prBlock) {
    if (prBlock == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    SceUInt32* pDest = (SceUInt32*)prBlock->address;
    SceUInt32 val = (SceUInt32)(prBlock->stdVal & 0xFFFFFFFF);
    if (*pDest == val) {
        return CHEATENGINE_SUCCESS;
    }
    *pDest = val;
    sceKernelDcacheWritebackInvalidateRange(pDest, 4);
    sceKernelIcacheInvalidateRange(pDest, 4);

    return CHEATENGINE_SUCCESS;
}

/* Reset the specified Block by returning the memory to its original value.
 * The memory is replaced as a 16-bit value.
 * 
 * Parameters:
 *   prBlock        Pointer to a Block struct representing the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is reset.
 */
int cheatengineResetUShort16Block(Block* prBlock) {
    if (prBlock == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    SceUShort16* pDest = (SceUShort16*)prBlock->address;
    SceUShort16 val = (SceUShort16)(prBlock->stdVal & 0xFFFF);
    if (*pDest == val) {
        return CHEATENGINE_SUCCESS;
    }
    *pDest = val;
    sceKernelDcacheWritebackInvalidateRange(pDest, 2);
    sceKernelIcacheInvalidateRange(pDest, 2);

    return CHEATENGINE_SUCCESS;
}

/* Set a specified Cheat to constant.  A constant cheat is activated when a
 * Cheat is applied, but is not reset when the Cheat is reset.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to set constant.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat is set to constant.
 */
int cheatengineSetCheatConstant(CheatEngine* prEng, int index) {
    Cheat* prCheat = NULL;
    char sMsg[256];
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        sprintf(sMsg, "%s: Cheat %d pointer is NULL.", 
                "cheatengineSetCheatConstant", index);
        cheatengineLog(prEng, LOG_WARN, sMsg);
        return CHEATENGINE_FAILURE;
    }
    sprintf(sMsg, "%s: Setting Cheat %d to constant.",
            "cheatengineSetCheatConstant", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    cheat_set_constant(prCheat);
    return CHEATENGINE_SUCCESS;
}

/* Set a specified Cheat to inactive.  An inactive cheat is not applied at any
 * time.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to set inactive.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat is set to inactive.
 */
int cheatengineSetCheatInactive(CheatEngine* prEng, int index) {
    Cheat* prCheat = NULL;
    char sMsg[256];
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        sprintf(sMsg, "%s: Cheat %d pointer is NULL.", 
                "cheatengineSetCheatInactive", index);
        cheatengineLog(prEng, LOG_WARN, sMsg);
        return CHEATENGINE_FAILURE;
    }
    sprintf(sMsg, "%s: Setting Cheat %d to inactive.",
            "cheatengineSetCheatInactive", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    cheat_set_inactive(prCheat);
    return CHEATENGINE_SUCCESS;
}

/* Set a specified Cheat to selected.  A selected cheat is activated when a
 * Cheat is applied, and is reset when the Cheats are reset.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to set selected.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat is set to selected.
 */
int cheatengineSetCheatSelected(CheatEngine* prEng, int index) {
    Cheat *prCheat = NULL;
    char sMsg[256];
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        sprintf(sMsg, "%s: Cheat %d pointer is NULL.", 
                "cheatengineSetCheatSelected", index);
        cheatengineLog(prEng, LOG_WARN, sMsg);
        return CHEATENGINE_FAILURE;
    }
    sprintf(sMsg, "%s: Setting Cheat %d to selected.",
            "cheatengineSetCheatSelected", index);
    cheatengineLog(prEng, LOG_DEBUG, sMsg);
    cheat_set_selected(prCheat);
    return CHEATENGINE_SUCCESS;
}

/* Assign a GeeLog pointer to act as the logging facility for the specified
 * CheatEngine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct that represents the Cheat
 *              Engine.
 *   prLog      Pointer to a GeeLog struct that represents the Logger.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the GeeLog logger is assigned.
 */
int cheatengineSetLogger(CheatEngine* prEng, GeeLog* prLog) {
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prEng->logger = prLog;
    return CHEATENGINE_SUCCESS;
}

static SceUInt32 parseDword(const char* sBuf) {
    SceUInt32 value = 0;
    char c = (char)0;
    int i = 0;

    for (i = 0; i < 8; i++) {
        c = sBuf[i];
        value <<= 4;
        switch (c) {
            case '_':
            case '0':
                break;
            case '1':
                value |= 1;
                break;
            case '2':
                value |= 2;
                break;
            case '3':
                value |= 3;
                break;
            case '4':
                value |= 4;
                break;
            case '5':
                value |= 5;
                break;
            case '6':
                value |= 6;
                break;
            case '7':
                value |= 7;
                break;
            case '8':
                value |= 8;
                break;
            case '9':
                value |= 9;
                break;
            case 'A':
            case 'a':
                value |= 10;
                break;
            case 'B':
            case 'b':
                value |= 11;
                break;
            case 'C':
            case 'c':
                value |= 12;
                break;
            case 'D':
            case 'd':
                value |= 13;
                break;
            case 'E':
            case 'e':
                value |= 14;
                break;
            case 'F':
            case 'f':
                value |= 15;
                break;
        }
    }
    
    return value;
}

static int parseName(const char* buffer, char* sName, size_t maxlen) {
    int len = 0;
    int started = 0;
    char c = (char)0;
    while (len < maxlen) {
        c = *(buffer + len);
        if (c == '\r' || c == '\n') {
            sName[len] = (char)0;
            break;
        }
        if (c == ' ' && started == 0) {
            continue;
        }
        sName[len] = c;
        started = 1;
        len++;
    }
    if (len == maxlen) {
        sName[maxlen] = (char)0;
    }
    return len;
}
