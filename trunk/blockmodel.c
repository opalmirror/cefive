#include "blockmodel.h"

static unsigned int kuiBlockId = 0;

Block* blockmodel_add(BlockModel* prModel) {
    int elemCt = 0;
    if (prModel != NULL) {
        elemCt = prModel->rowCount;
        if (elemCt > prModel->capacity) {
            if (blockmodel_expand(prModel) != BLOCKMODEL_SUCCESS) {
                return NULL;
            }
        }
        prModel->rowCount++;
        return blockmodel_get(prModel, elemCt);
    }
    return NULL;
}

int blockmodel_allocate(BlockModel* prModel, const int elements) {
    SceSize memSz = 0;
    char sBlock[BLOCKMODEL_BLKNAMLEN + 1];
    
    if (prModel == NULL) {
        return BLOCKMODEL_NULLPTR;
    }
    memSz = elements * sizeof(Block);
    sprintf(sBlock, "%s-%d", BLOCKMODEL_BLOCK_NAME, kuiBlockId);
    prModel->blockId = sceKernelAllocPartitionMemory(2, sBlock, PSP_SMEM_High,
            memSz, NULL);
    if (prModel->blockId < 0) {
        return BLOCKMODEL_MEMORY;
    }
    kuiBlockId++;
    prModel->blockList = (Block*)sceKernelGetBlockHeadAddr(prModel->blockId);
    if (prModel->blockList == NULL) {
        return BLOCKMODEL_FAILURE;
    }
    memset(prModel->blockList, 0, memSz);
    prModel->capacity = elements;
    prModel->rowCount = 0;
    return BLOCKMODEL_SUCCESS;
}

int blockmodel_delete(BlockModel* prModel) {
    if (prModel == NULL) {
        return BLOCKMODEL_NULLPTR;
    }
    if (prModel->blockId < 0) {
        return BLOCKMODEL_SUCCESS;
    }
    if (prModel->blockList == NULL) {
        return BLOCKMODEL_FAILURE;
    }
    if (sceKernelFreePartitionMemory(prModel->blockId) < 0) {
        return BLOCKMODEL_MEMORY;
    }
    prModel->blockId = -1;
    prModel->blockList = NULL;
    prModel->capacity = 0;
    prModel->expansion = 0;
    prModel->rowCount = 0;
    return BLOCKMODEL_SUCCESS;
}

int blockmodel_expand(BlockModel* prModel) {
    int elemCt = 0;
    if (prModel == NULL) {
        return BLOCKMODEL_NULLPTR;
    }
    if (prModel->blockId < 0) {
        return BLOCKMODEL_MEMORY;
    }
    if (prModel->blockList == NULL) {
        return BLOCKMODEL_FAILURE;
    }
    elemCt = prModel->capacity + prModel->expansion;
    if (blockmodel_reallocate(prModel, elemCt) != BLOCKMODEL_SUCCESS) {
        return BLOCKMODEL_MEMORY;
    }
    return BLOCKMODEL_SUCCESS;
}

Block* blockmodel_get(BlockModel* prModel, const int index) {
    if (prModel != NULL) {
        if (prModel->blockList != NULL) {
            if ((index >= 0) && (index < prModel->rowCount)) {
                return (Block*)(prModel->blockList + 
                        (index * sizeof(Block)));
            }
        }
    }
    return NULL;
}

int blockmodel_init(BlockModel* prModel, const int rowCount) {
    if (prModel == NULL) {
        return BLOCKMODEL_NULLPTR;
    }
    prModel->blockId = -1;
    if (blockmodel_allocate(prModel, rowCount) != BLOCKMODEL_SUCCESS) {
        return BLOCKMODEL_MEMORY;
    }
    prModel->expansion = rowCount;
    return BLOCKMODEL_SUCCESS;
}

int blockmodel_reallocate(BlockModel* prModel, const int elements) {
    SceUID blockId = -1;
    SceSize memSz = 0;
    Block* blockList = NULL;
    int elemCt = 0;
    
    if (prModel == NULL) {
        return BLOCKMODEL_NULLPTR;
    }
    if (prModel->blockId < 0) {
        return BLOCKMODEL_MEMORY;
    }
    if (prModel->blockList == NULL) {
        return BLOCKMODEL_FAILURE;
    }
    blockId = prModel->blockId;
    elemCt = prModel->rowCount;
    memSz = elemCt * sizeof(Block);
    blockList = prModel->blockList;
    
    if (blockmodel_allocate(prModel, elements) != BLOCKMODEL_SUCCESS) {
        return BLOCKMODEL_MEMORY;
    }
    
    memcpy(prModel->blockList, blockList, memSz);
    prModel->rowCount = elemCt;
    if (sceKernelFreePartitionMemory(blockId) < 0) {
        return BLOCKMODEL_MEMORY;
    }
    return BLOCKMODEL_SUCCESS;
}
