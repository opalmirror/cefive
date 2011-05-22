#include "cheatengine.h"

static Block karBlock[CHEATENGINE_BLOCK_MAX];
static Cheat karCheat[CHEATENGINE_CHEAT_MAX];

static SceUInt32 parseDword(const char* sBuf);
static int parseName(const char* buffer, char* sName, size_t maxlen);

Block* cheatengine_add_block(CheatEngine* prEng) {
    Block* prBlock = NULL;
    BlockModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = cheatengine_get_blockmodel(prEng);
        prBlock = blockmodel_add(prModel);
    }
    return prBlock;
}

Cheat* cheatengine_add_cheat(CheatEngine* prEng) {
    Cheat* prCheat = NULL;
    CheatModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = cheatengine_get_cheatmodel(prEng);
        prCheat = cheatmodel_add(prModel);
    }
    return prCheat;
}

Block* cheatengine_get_block(CheatEngine* prEng, const int index) {
    Block* prBlock = NULL;
    BlockModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = cheatengine_get_blockmodel(prEng);
        prBlock = blockmodel_get(prModel, index);
    }
    return prBlock;
}

int cheatengine_get_blockcount(CheatEngine* prEng) {
    int count = CHEATENGINE_FAILURE;
    BlockModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = cheatengine_get_blockmodel(prEng);
        count = prModel->rowCount;
    }
    return count;
}

BlockModel* cheatengine_get_blockmodel(CheatEngine* prEng) {
    BlockModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = &prEng->blockModel;
    }
    return prModel;
}

Cheat* cheatengine_get_cheat(CheatEngine* prEng, const int index) {
    Cheat *prCheat = NULL;
    CheatModel *prModel = NULL;
    if (prEng != NULL) {
        prModel = cheatengine_get_cheatmodel(prEng);
        prCheat = cheatmodel_get(prModel, index);
    }
    return prCheat;
}

int cheatengine_get_cheatcount(CheatEngine* prEng) {
    int count = CHEATENGINE_FAILURE;
    CheatModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = cheatengine_get_cheatmodel(prEng);
        count = prModel->rowCount;
    }
    return count;
}

CheatModel* cheatengine_get_cheatmodel(CheatEngine* prEng) {
    CheatModel* prModel = NULL;
    if (prEng != NULL) {
        prModel = &prEng->cheatModel;
    }
    return prModel;
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
    return prDest;
}

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
    }
    return prDest;
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
    Block *prDest = NULL;
    if (prEng == NULL || index < 0) {
        return prDest;
    }
    if (index >= prEng->block_count) {
        return prDest;
    }
    prDest = &(prEng->blocklist[index]);
    return prDest;
}

Cheat* cheatengineGetCheat(CheatEngine *prEng, int index) {
    Cheat *prDest = NULL;
    if (prEng == NULL || index < 0) {
        return prDest;
    }
    if (index >= prEng->cheat_count) {
        return prDest;
    }
    prDest = &(prEng->cheatlist[index]);
    return prDest;
}

int cheatengineInit(CheatEngine* prEng, CEFiveConfig* prCfg, Cheat* arCheat,
        Block* arBlock) {
    BlockModel* prBmodel = NULL;
    CheatModel* prCmodel = NULL;
    Block* prBlock = NULL;
    Cheat* prCheat = NULL;
    int i = 0;
    
    if (prEng == NULL) {
        return CHEATENGINE_NULLPTR;
    }
    if (prCfg == NULL) {
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
                return CHEATENGINE_FAILURE;
            }
        }
    } else {
        prEng->blocklist = arBlock;
    }
    prEng->cheat_count = 0;

    prBmodel = cheatengine_get_blockmodel(prEng);
    if (blockmodel_init(prBmodel, 16) != BLOCKMODEL_SUCCESS) {
        return CHEATENGINE_FAILURE;
    }
    prCmodel = cheatengine_get_cheatmodel(prEng);
    if (cheatmodel_init(prCmodel, 8) != CHEATMODEL_SUCCESS) {
        return CHEATENGINE_FAILURE;
    }
    return CHEATENGINE_SUCCESS;
}

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

int cheatengineRefresh(CheatEngine* prEng) {
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
