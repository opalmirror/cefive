#include <stdlib.h>
#include "block.h"
#include "blocklist.h"

int blocklist_add(BlockList* prList, Block* prBlock) {
    int idx = 0;
    if (prList == NULL || prBlock == NULL) {
        return BLOCKLIST_NULLPTR;
    }
    if (prList->size + 1 > BLOCKLIST_MAX_LEN) {
        return BLOCKLIST_MEMORY;
    }
    idx = prList->size;
    prList->aprBlock[idx] = prBlock;
    prList->size++;
    return BLOCKLIST_SUCCESS;
}

int blocklist_clear(BlockList* prList) {
    int i = 0;
    
    if (prList == NULL) {
        return BLOCKLIST_NULLPTR;
    }
    for (i = 0; i < BLOCKLIST_MAX_LEN; i++) {
        prList->aprBlock[i] = NULL;
    }
    prList->size = 0;
    return BLOCKLIST_SUCCESS;
}

Block* blocklist_get(BlockList* prList, int index) {
    Block* block = NULL;
    if (prList != NULL) {
        if (index >= 0 && index < prList->size) {
            block = prList->aprBlock[index];
        }
    }
    return block;
}

int blocklist_init(BlockList* prList) {
    return blocklist_clear(prList);
}

int blocklist_move_down(BlockList* prList, int index) {
    Block* prBlock = NULL;
    if (prList == NULL) {
        return BLOCKLIST_NULLPTR;
    }
    if (index < 0 || index >= prList->size) {
        return BLOCKLIST_INVIDX;
    }
    if (index < prList->size - 1) {
        prBlock = prList->aprBlock[index];
        prList->aprBlock[index] = prList->aprBlock[index + 1];
        prList->aprBlock[index + 1] = prBlock;
    }
    
    return BLOCKLIST_SUCCESS;
}

int blocklist_move_up(BlockList* prList, int index) {
    Block* prBlock = NULL;
    if (prList == NULL) {
        return BLOCKLIST_NULLPTR;
    }
    if (index < 0 || index >= prList->size) {
        return BLOCKLIST_INVIDX;
    }
    if (index > 0) {
        prBlock = prList->aprBlock[index];
        prList->aprBlock[index] = prList->aprBlock[index - 1];
        prList->aprBlock[index - 1] = prBlock;
    }
    
    return BLOCKLIST_SUCCESS;
}

Block* blocklist_remove(BlockList* prList, int index) {
    Block* prOld = NULL;
    int i = 0;
    if (prList != NULL) {
        if (index >= 0 && index < prList->size) {
            prOld = prList->aprBlock[index];
            for (i = index; i < prList->size - 1; i++) {
                prList->aprBlock[i] = prList->aprBlock[i + 1];
            }
            prList->aprBlock[prList->size] = NULL;
            prList->size--;
        }
    }
    return prOld;
}

Block* blocklist_set(BlockList* prList, int index, Block* prBlock) {
    Block* prOld = NULL;
    if (prList == NULL || prBlock == NULL) {
        return prOld;
    }
    if (index < 0 || index >= prList->size) {
        return prOld;
    }
    prOld = blocklist_get(prList, index);
    if (prOld != NULL) {
        prList->aprBlock[index] = prBlock;
    }
    return prOld;
}
