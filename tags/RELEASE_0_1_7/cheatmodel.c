#include "cheatmodel.h"

static unsigned int kuiBlockId = 0;

Cheat* cheatmodel_add(CheatModel* prModel) {
    int elemCt = 0;
    Cheat* prCheat = NULL;
    if (prModel != NULL) {
        elemCt = prModel->rowCount;
        if (elemCt > prModel->capacity) {
            if (cheatmodel_expand(prModel) != CHEATMODEL_SUCCESS) {
                return NULL;
            }
        }
        prModel->rowCount++;
        prCheat = cheatmodel_get(prModel, elemCt);
    }
    return prCheat;
}

int cheatmodel_allocate(CheatModel* prModel, const int elements) {
    SceSize memSz = 0;
    char sBlock[CHEATMODEL_BLKNMLEN + 1];
    
    if (prModel == NULL) {
        return CHEATMODEL_NULLPTR;
    }
    memSz = elements * sizeof(Cheat);
    sprintf(sBlock, "%s-%d", CHEATMODEL_BLOCK_NAME, kuiBlockId);
    prModel->blockId = sceKernelAllocPartitionMemory(2, sBlock, PSP_SMEM_High,
            memSz, NULL);
    if (prModel->blockId < 0) {
        return CHEATMODEL_MEMORY;
    }
    kuiBlockId++;
    prModel->cheatList = (Cheat*) sceKernelGetBlockHeadAddr(prModel->blockId);
    if (prModel->cheatList == NULL) {
        return CHEATMODEL_FAILURE;
    }
    memset(prModel->cheatList, 0, memSz);
    prModel->capacity = elements;
    prModel->rowCount = 0;
    return CHEATMODEL_SUCCESS;
}

int cheatmodel_delete(CheatModel* prModel) {
    if (prModel == NULL) {
        return CHEATMODEL_NULLPTR;
    }
    if (prModel->blockId < 0) {
        return CHEATMODEL_SUCCESS;
    }
    if (prModel->cheatList == NULL) {
        return CHEATMODEL_FAILURE;
    }
    if (sceKernelFreePartitionMemory(prModel->blockId) < 0) {
        return CHEATMODEL_MEMORY;
    }
    prModel->blockId = -1;
    prModel->cheatList = NULL;
    prModel->capacity = 0;
    prModel->expansion = 0;
    prModel->rowCount = 0;
    return CHEATMODEL_SUCCESS;
}

int cheatmodel_expand(CheatModel* prModel) {
    int elemCt = 0;
    if (prModel == NULL) {
        return CHEATMODEL_NULLPTR;
    }
    if (prModel->blockId < 0) {
        return CHEATMODEL_MEMORY;
    }
    if (prModel->cheatList == NULL) {
        return CHEATMODEL_FAILURE;
    }
    elemCt = prModel->capacity + prModel->expansion;
    if (cheatmodel_reallocate(prModel, elemCt) != CHEATMODEL_SUCCESS) {
        return CHEATMODEL_MEMORY;
    }
    return CHEATMODEL_SUCCESS;
}

Cheat* cheatmodel_get(CheatModel* prModel, int index) {
    Cheat* prCheat = NULL;
    if (prModel != NULL) {
        if ((index >= 0) && (index < prModel->rowCount)) {
            prCheat = (Cheat*)(prModel->cheatList + (index * sizeof(Cheat)));
        }
    }
    return prCheat;
}

int cheatmodel_get_name(char* sDest, CheatModel* prModel, int index) {
    Cheat* prCheat = NULL;
    if ((sDest == NULL) || (prModel == NULL)) {
        return CHEATMODEL_NULLPTR;
    }
    if ((index < 0) || (index >= prModel->rowCount)) {
        return CHEATMODEL_INVIDX;
    }
    prCheat = cheatmodel_get(prModel, index);
    if (prCheat == NULL) {
        return CHEATMODEL_FAILURE;
    }
    strcpy(sDest, prCheat->name);
    return CHEATMODEL_SUCCESS;
}

int cheatmodel_init(CheatModel* prModel, int rowCount) {
    if (prModel == NULL) {
        return CHEATMODEL_NULLPTR;
    }
    prModel->blockId = -1;
    if (cheatmodel_allocate(prModel, rowCount) != CHEATMODEL_SUCCESS) {
        return CHEATMODEL_MEMORY;
    }
    prModel->expansion = rowCount;
    return CHEATMODEL_SUCCESS;
}

int cheatmodel_reallocate(CheatModel* prModel, const int elements) {
    SceUID blockId = -1;
    SceSize memSz = 0;
    Cheat* cheatList = NULL;
    int elemCt = 0;
    
    if (prModel == NULL) {
        return CHEATMODEL_NULLPTR;
    }
    if (prModel->blockId < 0) {
        return CHEATMODEL_MEMORY;
    }
    if (prModel->cheatList == NULL) {
        return CHEATMODEL_FAILURE;
    }
    blockId = prModel->blockId;
    elemCt = prModel->rowCount;
    memSz = elements * sizeof(Cheat);
    cheatList = prModel->cheatList;
    
    if (cheatmodel_allocate(prModel, elements) != CHEATMODEL_SUCCESS) {
        return CHEATMODEL_MEMORY;
    }
    
    memcpy(prModel->cheatList, cheatList, memSz);
    prModel->rowCount = elemCt;
    if (sceKernelFreePartitionMemory(blockId) < 0) {
        return CHEATMODEL_MEMORY;
    }
    return CHEATMODEL_SUCCESS;
}
