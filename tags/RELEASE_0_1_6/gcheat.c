#include <stdlib.h>
#include <string.h>
#include "block.h"
#include "blocklist.h"
#include "gcheat.h"

int gcheat_add_block(GCheat* prCheat, Block* prBlock) {
    BlockList* prList = NULL;
    int r = 0;
    
    if (prCheat == NULL) {
        return GCHEAT_NULLPTR;
    }
    prList = &prCheat->rBlockList;
    r = blocklist_add(prList, prBlock);
    if (r != BLOCKLIST_SUCCESS) {
        return GCHEAT_FAILURE;
    }
    
    return GCHEAT_SUCCESS;
}

int gcheat_clear_blocklist(GCheat* prCheat) {
    BlockList* prList = NULL;
    int r = 0;
    
    if (prCheat == NULL) {
        return GCHEAT_NULLPTR;
    }
    prList = &prCheat->rBlockList;
    r = blocklist_clear(prList);
    if (r != BLOCKLIST_SUCCESS) {
        return GCHEAT_FAILURE;
    }
    
    return GCHEAT_SUCCESS;
}

Block* gcheat_get_block(GCheat* prCheat, int index) {
    Block* prBlock = NULL;
    BlockList* prList = NULL;
    if (prCheat == NULL) {
        return GCHEAT_NULLPTR;
    }
    prList = &prCheat->rBlockList;
    prBlock = blocklist_get(prList, index);
    return prBlock;
}

int gcheat_get_blockcount(GCheat* prCheat) {
    int count = -1;
    BlockList* prList = NULL;
    if (prCheat != NULL) {
        prList = &prCheat->rBlockList;
        count = prList->size;
    }
    return count;
}

BlockList* gcheat_get_blocklist(GCheat* prCheat) {
    BlockList* prList = NULL;
    if (prCheat != NULL) {
        prList = &prCheat->rBlockList;
    }
    return prList;
}

char* gcheat_get_name(GCheat* prCheat) {
    char* sName = NULL;
    if (prCheat != NULL) {
        sName = prCheat->sName;
    }
    return sName;
}

int gcheat_init(GCheat* prCheat) {
    BlockList* prList = NULL;
    int r = 0;
    
    if (prCheat == NULL) {
        return GCHEAT_NULLPTR;
    }
    prList = &prCheat->rBlockList;
    r = blocklist_init(prList);
    if (r != BLOCKLIST_SUCCESS) {
        return GCHEAT_FAILURE;
    }
    memset(prCheat->sName, 0, GCHEAT_NAME_LEN + 1);
    
    return GCHEAT_SUCCESS;
}

int gcheat_move_block_down(GCheat* prCheat, int index) {
    BlockList* prList = NULL;
    int r = 0;
    
    if (prCheat == NULL) {
        return GCHEAT_NULLPTR;
    }
    prList = &prCheat->rBlockList;
    r = blocklist_move_down(prList, index);
    if (r != BLOCKLIST_SUCCESS) {
        return GCHEAT_FAILURE;
    }
    return GCHEAT_SUCCESS;
}

int gcheat_move_block_up(GCheat* prCheat, int index) {
    BlockList* prList = NULL;
    int r = 0;
    
    if (prCheat == NULL) {
        return GCHEAT_NULLPTR;
    }
    prList = &prCheat->rBlockList;
    r = blocklist_move_up(prList, index);
    if (r != BLOCKLIST_SUCCESS) {
        return GCHEAT_FAILURE;
    }
    return GCHEAT_SUCCESS;
}

Block* gcheat_remove_block(GCheat* prCheat, int index) {
    Block* prBlock = NULL;
    BlockList* prList = NULL;

    if (prCheat != NULL) {
        prList = &prCheat->rBlockList;
        prBlock = blocklist_remove(prList, index);
    }
    return prBlock;
}

Block* gcheat_set_block(GCheat* prCheat, int index, Block* prBlock) {
    Block* prOld = NULL;
    BlockList* prList = NULL;

    if (prCheat != NULL) {
        prList = &prCheat->rBlockList;
        prOld = blocklist_set(prList, index, prBlock);
    }
    return prOld;
}
