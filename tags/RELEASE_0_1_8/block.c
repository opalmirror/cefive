#include "block.h"

int block_clear_byte(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_BYTE;
    return BLOCK_SUCCESS;
}

int block_clear_dma(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_DMA;
    return BLOCK_SUCCESS;
}

int block_clear_dword(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_DWORD;
    return BLOCK_SUCCESS;
}

int block_clear_freeze(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_FREEZE;
    return BLOCK_SUCCESS;
}

int block_clear_qword(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_UWORD;
    return BLOCK_SUCCESS;
}

int block_clear_word(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_WORD;
    return BLOCK_SUCCESS;
}

SceUInt32 block_get_address(Block* prBlock) {
    unsigned int addr = 0;
    if (prBlock != NULL) {
        addr = prBlock->address;
    }
    return addr;
}

SceUInt32 block_get_original(Block* prBlock) {
    unsigned int orig = 0;
    if (prBlock != NULL) {
        orig = prBlock->stdVal;
    }
    return orig;
}

SceUInt32 block_get_patch(Block* prBlock) {
    unsigned int patch = 0;
    if (prBlock != NULL) {
        patch = prBlock->hakVal;
    }
    return patch;
}

int block_init(Block* prBlock, SceUInt32 address, SceUInt32 value,
        SceUChar8 flags) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->address = address;
    prBlock->stdVal = value;
    prBlock->flags = flags;

    return BLOCK_SUCCESS;
}

int block_is_byte(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_BYTE) ? 1 : 0;
    }
    return is;
}

int block_is_dma(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_DMA) ? 1 : 0;
    }
    return is;
}

int block_is_dword(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_DWORD) ? 1 : 0;
    }
    return is;
}

int block_is_freeze(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_FREEZE) ? 1 : 0;
    }
    return is;
}

int block_is_qword(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_UWORD) ? 1 : 0;
    }
    return is;
}

int block_is_word(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_WORD) ? 1 : 0;
    }
    return is;
}

int block_set_address(Block* prBlock, SceUInt32 addr) {
    if (prBlock != NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->address = addr;
    return BLOCK_SUCCESS;
}

int block_set_byte(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_BYTE;
    block_clear_dword(prBlock);
    block_clear_qword(prBlock);
    block_clear_word(prBlock);
    return BLOCK_SUCCESS;
}

int block_set_dma(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_DMA;
    return BLOCK_SUCCESS;
}

int block_set_dword(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_DWORD;
    block_clear_byte(prBlock);
    block_clear_qword(prBlock);
    block_clear_word(prBlock);
    return BLOCK_SUCCESS;
}

int block_set_freeze(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_FREEZE;
    return BLOCK_SUCCESS;
}

int block_set_original(Block* prBlock, SceUInt32 orig) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->stdVal = orig;
    return BLOCK_SUCCESS;
}

int block_set_patch(Block* prBlock, SceUInt32 patch) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->hakVal = patch;
    return BLOCK_SUCCESS;
}

int block_set_qword(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_UWORD;
    block_clear_byte(prBlock);
    block_clear_dword(prBlock);
    block_clear_word(prBlock);
    return BLOCK_SUCCESS;
}

int block_set_word(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_WORD;
    block_clear_byte(prBlock);
    block_clear_dword(prBlock);
    block_clear_qword(prBlock);
    return BLOCK_SUCCESS;
}
