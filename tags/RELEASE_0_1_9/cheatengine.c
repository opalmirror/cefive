#include "cheatengine.h"

static Block karBlock[CHEATENGINE_BLOCK_MAX];
static Cheat karCheat[CHEATENGINE_CHEAT_MAX];

Block* cheatengine_add_block(CheatEngine* prEng) {
    int index = 0;
    if (prEng != NULL) {
        if (prEng->block_count < CHEATENGINE_BLOCK_MAX) {
            index = prEng->block_count;
            prEng->block_count++;
            return cheatengine_get_block(prEng, index);
        }
    }
    return NULL;
}

Cheat* cheatengine_add_cheat(CheatEngine* prEng) {
    int index = 0;
    if (prEng != NULL) {
        if (prEng->cheat_count < CHEATENGINE_CHEAT_MAX) {
            index = prEng->cheat_count;
            prEng->cheat_count++;
            return cheatengine_get_cheat(prEng, index);
        }
    }
    return NULL;
}

Block* cheatengine_get_block(CheatEngine* prEng, const int index) {
    if (prEng != NULL) {
        if ((index >= 0) && (index < prEng->block_count)) {
            return &prEng->blocklist[index];
        }
    }
    return NULL;
}

Cheat* cheatengine_get_cheat(CheatEngine* prEng, const int index) {
    if (prEng != NULL) {
        if ((index >= 0) && (index < prEng->cheat_count)) {
            return &prEng->cheatlist[index];
        }
    }
    return NULL;
}

int cheatengineActivateCheats(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    int iCheat = 0;

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }

    for (iCheat = 0; iCheat < prEng->cheat_count; iCheat++) {
        prCheat = cheatengineGetCheat(prEng, iCheat);
        if (prCheat == NULL) {
            continue;
        }
        if (cheat_is_inactive(prCheat) == 0) {
            cheatengineApplyCheat(prEng, iCheat);
        }
    }
    prEng->trigger_active = 1;

    return CHEATENGINE_SUCCESS;
}

int cheatengineApplyBlock(CheatEngine *prEng, int index) {
    int r = 0;
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    Block *prBlock = cheatengineGetBlock(prEng, index);
    if (prBlock == NULL) {
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
    return CHEATENGINE_SUCCESS;
}

int cheatengineApplyCheat(CheatEngine *prEng, int index) {
    int iBlock = 0;
    int iEnd = 0;
    int iStart = 0;
    int r = CHEATENGINE_SUCCESS;

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    Cheat* prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        return CHEATENGINE_FAILURE;
    }
    iStart = prCheat->block;
    iEnd = prCheat->len + iStart;
    for (iBlock = iStart; iBlock < iEnd; iBlock++) {
        r = cheatengineApplyBlock(prEng, iBlock);
        if (r != CHEATENGINE_SUCCESS) {
            break;
        }
    }
    cheat_setflag_fresh(prCheat);
    return r;
}

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

int cheatengineDeactivateCheats(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    int iCheat = 0;

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }

    for (iCheat = 0; iCheat < prEng->cheat_count; iCheat++) {
        prCheat = cheatengineGetCheat(prEng, iCheat);
        if (prCheat == NULL) {
            continue;
        }
        if (cheat_is_selected(prCheat) == 1) {
            cheatengineResetCheat(prEng, iCheat);
        }
    }
    prEng->trigger_active = 0;

    return CHEATENGINE_SUCCESS;
}

Block* cheatengineGetBlock(CheatEngine *prEng, int index) {
    return cheatengine_get_block(prEng, index);
}

Cheat* cheatengineGetCheat(CheatEngine *prEng, int index) {
    return cheatengine_get_cheat(prEng, index);
}

int cheatengine_init(CheatEngine* prEng, CEFiveConfig* prCfg, Cheat* arCheat,
        Block* arBlock) {
    const char* sFunc = "cheatengineInit";
    Block* prBlock = NULL;
    Cheat* prCheat = NULL;
    int i = 0;
    
    if (prEng == NULL) {
        geelog_flog(LOG_ERROR, sFunc, "Invalid CheatEngine pointer.");
        return CHEATENGINE_NULLPTR;
    }
    if (prCfg == NULL) {
        geelog_flog(LOG_ERROR, sFunc, "Invalid CEFiveConfig pointer.");
        return CHEATENGINE_NULLPTR;
    }
    prEng->prConfig = prCfg;
    /* I am planning on removing the ability to pass the Cheat and Block array
     * in to the function and instead use static Arrays from cheatengine itself.
     * In order to keep from breaking the existing mechanism, this allows NULL
     * to be passed for both the Cheat and Block arrays to use the static
     * ones.
     */
    if (arCheat == NULL) {
        prEng->cheatlist = karCheat;
        for (i = 0; i < CHEATENGINE_CHEAT_MAX; i++) {
            prCheat = &karCheat[i];
            if (cheat_init(prCheat) != CHEAT_SUCCESS) {
                geelog_flog(LOG_ERROR, sFunc, "Failed to initialize Cheat.");
                return CHEATENGINE_FAILURE;
            }
        }
    } else {
        prEng->cheatlist = arCheat;
    }
    if (arBlock == NULL) {
        prEng->blocklist = karBlock;
        for (i = 0; i < CHEATENGINE_BLOCK_MAX; i++) {
            prBlock = &karBlock[i];
            if (block_init(prBlock, 0, 0, (unsigned char) 0) != BLOCK_SUCCESS) {
                geelog_flog(LOG_ERROR, sFunc, "Failed to initialize Block.");
                return CHEATENGINE_FAILURE;
            }
        }
    } else {
        prEng->blocklist = arBlock;
    }
    prEng->cheat_count = 0;

    geelog_flog(LOG_INFO, sFunc, "Cheat Engine initialized.");
    return CHEATENGINE_SUCCESS;
}

int cheatengine_refresh(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    int iCheat = 0;

    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    for (iCheat = 0; iCheat < prEng->cheat_count; iCheat++) {
        prCheat = cheatengineGetCheat(prEng, iCheat);
        if (prCheat == NULL) {
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

int cheatengineResetBlock(CheatEngine *prEng, int index) {
    int r = 0;
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    Block *prBlock = cheatengineGetBlock(prEng, index);
    if (prBlock == NULL) {
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

    return CHEATENGINE_SUCCESS;
}

int cheatengineResetCheat(CheatEngine *prEng, int index) {
    int iBlock = 0;
    int iEnd = 0;
    int iStart = 0;
    int r = CHEATENGINE_SUCCESS;

    if (prEng == NULL) {
        return CHEATENGINE_FAILURE;
    }
    Cheat* prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        return CHEATENGINE_FAILURE;
    }
    iStart = prCheat->block;
    iEnd = prCheat->len + iStart;
    for (iBlock = iStart; iBlock < iEnd; iBlock++) {
        r = cheatengineResetBlock(prEng, iBlock);
        if (r != CHEATENGINE_SUCCESS) {
            break;
        }
    }
    cheat_setflag_fresh(prCheat);
    return r;
}

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

int cheatengineSetCheatConstant(CheatEngine* prEng, int index) {
    Cheat* prCheat = NULL;
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        return CHEATENGINE_FAILURE;
    }
    cheat_set_constant(prCheat);
    return CHEATENGINE_SUCCESS;
}

int cheatengineSetCheatInactive(CheatEngine* prEng, int index) {
    Cheat* prCheat = NULL;
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        return CHEATENGINE_FAILURE;
    }
    cheat_set_inactive(prCheat);
    return CHEATENGINE_SUCCESS;
}

int cheatengineSetCheatSelected(CheatEngine* prEng, int index) {
    Cheat *prCheat = NULL;
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    prCheat = cheatengineGetCheat(prEng, index);
    if (prCheat == NULL) {
        return CHEATENGINE_FAILURE;
    }
    cheat_set_selected(prCheat);
    return CHEATENGINE_SUCCESS;
}

