/* block.c
 *
 * Author:
 *  Sir Gee of Five
 */

#include <stdlib.h>
#include <pspkerneltypes.h>
#include "block.h"

/* Clear the BYTE flag of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the block to clear.
 * 
 * Results:
 *   BLOCK_MEMORY is returned if the specified pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block flag is cleared.
 */
int block_clear_byte(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_BYTE;
    return BLOCK_SUCCESS;
}

/* Clear the DMA flag of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the block to clear.
 * 
 * Results:
 *   BLOCK_MEMORY is returned if the specified pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block flag is cleared.
 */
int block_clear_dma(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_DMA;
    return BLOCK_SUCCESS;
}

/* Clear the DWORD flag of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the block to clear.
 * 
 * Results:
 *   BLOCK_MEMORY is returned if the specified pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block flag is cleared.
 */
int block_clear_dword(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_DWORD;
    return BLOCK_SUCCESS;
}

/* Clear the FREEZE flag of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the block to clear.
 * 
 * Results:
 *   BLOCK_MEMORY is returned if the specified pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block flag is cleared.
 */
int block_clear_freeze(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_FREEZE;
    return BLOCK_SUCCESS;
}

/* Clear the UWORD flag of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the block to clear.
 * 
 * Results:
 *   BLOCK_MEMORY is returned if the specified pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block flag is cleared.
 */
int block_clear_qword(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_UWORD;
    return BLOCK_SUCCESS;
}

/* Clear the WORD flag of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the block to clear.
 * 
 * Results:
 *   BLOCK_MEMORY is returned if the specified pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block flag is cleared.
 */
int block_clear_word(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags &= ~BLOCK_FLAG_WORD;
    return BLOCK_SUCCESS;
}

/* Return the destination address of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing a Cheat Block.
 * 
 * Returns:
 *   An unsigned integer containing the 32-bit address of a Block is returned.
 */
unsigned int block_get_address(Block* prBlock) {
    unsigned int addr = 0;
    if (prBlock != NULL) {
        addr = prBlock->address;
    }
    return addr;
}

/* Return the original value of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing a Cheat Block.
 * 
 * Returns:
 *   An unsigned integer containing the original Block value is returned.
 */
unsigned int block_get_original(Block* prBlock) {
    unsigned int orig = 0;
    if (prBlock != NULL) {
        orig = prBlock->stdVal;
    }
    return orig;
}

/* Return the patch value of the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing a Cheat Block.
 * 
 * Returns:
 *   An unsigned integer containing the patch value of the Block is returned.
 */
unsigned int block_get_patch(Block* prBlock) {
    unsigned int patch = 0;
    if (prBlock != NULL) {
        patch = prBlock->hakVal;
    }
    return patch;
}

/* Initialize the specified Block struct with the specified values.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct to initialize.
 *   address    Unsigned integer containing the 32-bit address of the Block.
 *   value      Unsigned integer containing the patch value of the Block.
 *   flags      Unsigned char containing the flags of the Block.
 * 
 * Returns:
 *   BLOCK_MEMORY is returned if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS is returned if the Block is initialized.
 */
int block_init(Block* prBlock, unsigned int address, unsigned int value,
        unsigned char flags) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->address = address;
    prBlock->stdVal = value;
    prBlock->flags = flags;

    return BLOCK_SUCCESS;
}

/* Return an integer indicating whether the BYTE flag is set in the specified
 * Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   1 is returned if the BYTE flag is set.
 *   0 is returned if the BYTE flag is clear.
 */
int block_is_byte(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_BYTE) ? 1 : 0;
    }
    return is;
}

/* Return an integer indicating whether the DMA flag is set in the specified
 * Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   1 is returned if the DMA flag is set.
 *   0 is returned if the DMA flag is clear.
 */
int block_is_dma(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_DMA) ? 1 : 0;
    }
    return is;
}

/* Return an integer indicating whether the DWORD flag is set in the specified
 * Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   1 is returned if the DWORD flag is set.
 *   0 is returned if the DWORD flag is clear.
 */
int block_is_dword(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_DWORD) ? 1 : 0;
    }
    return is;
}

/* Return an integer indicating whether the FREEZE flag is set in the specified
 * Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   1 is returned if the FREEZE flag is set.
 *   0 is returned if the FREEZE flag is clear.
 */
int block_is_freeze(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_FREEZE) ? 1 : 0;
    }
    return is;
}

/* Return an integer indicating whether the UWORD flag is set in the specified
 * Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   1 is returned if the UWORD flag is set.
 *   0 is returned if the UWORD flag is clear.
 */
int block_is_qword(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_UWORD) ? 1 : 0;
    }
    return is;
}

/* Return an integer indicating whether the WORD flag is set in the specified
 * Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   1 is returned if the WORD flag is set.
 *   0 is returned if the WORD flag is clear.
 */
int block_is_word(Block* prBlock) {
    int is = 0;
    if (prBlock != NULL) {
        is = (prBlock->flags & BLOCK_FLAG_WORD) ? 1 : 0;
    }
    return is;
}

/* Assign an address to the specified Block.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 *   address    Unsigned integer containing the 32-bit address to assign.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the address is assigned.
 */
int block_set_address(Block* prBlock, unsigned int addr) {
    if (prBlock != NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->address = addr;
    return BLOCK_SUCCESS;
}

/* Set the BYTE flag of the specified Block.  The BYTE flag indicates that the
 * patch value is masked to the Least Significant Byte value before being
 * applied to memory.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the flag is set.
 */
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

/* Set the DMA flag of the specified Block.  The DMA flag indicates that the
 * address of the Block contains an offset rather than a physical address and
 * should be applied as an offset.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the flag is set.
 */
int block_set_dma(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_DMA;
    return BLOCK_SUCCESS;
}

/* Set the DWORD flag of the specified Block.  The DWORD flag indicates that the
 * patch value is a 32-bit value and will be applied to memory as such.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the flag is set.
 */
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

/* Set the FREEZE flag of the specified Block.  The FREEZE flag was a partially
 * implemented flag in NitePr.  It's functionality was not documented, nor was
 * it supported sufficiently to determine the meaning.  The FREEZE flag now
 * indicates that the value needs to be applied to the memory in such a way as
 * to maintain the value in memory as the value in the Block.  In other words,
 * freeze the indicated address at the indicated value.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the flag is set.
 */
int block_set_freeze(Block* prBlock) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->flags |= BLOCK_FLAG_FREEZE;
    return BLOCK_SUCCESS;
}

/* Assign the original memory value of a Block.  The original memory value is
 * written back to memory when a Block is removed from memory.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 *   orig       Unsigned integer containing the original memory value.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the original value is assigned.
 */
int block_set_original(Block* prBlock, unsigned int orig) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->stdVal = orig;
    return BLOCK_SUCCESS;
}

/* Assign the patch value of a Block.  The patch value is copied over memory
 * when a Block is applied to memory.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 *   patch      Unsigned integer containing the patch value.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the patch value is assigned.
 */
int block_set_patch(Block* prBlock, unsigned int patch) {
    if (prBlock == NULL) {
        return BLOCK_MEMORY;
    }
    prBlock->hakVal = patch;
    return BLOCK_SUCCESS;
}

/* Set the UWORD flag of the specified Block.  The UWORD flag indicates that the
 * patch value is a 64-bit value and will be applied to memory as such.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the flag is set.
 */
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

/* Set the WORD flag of the specified Block.  The WORD flag indicates that the
 * patch value is a 16-bit value and will be applied to memory as such.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct containing the Cheat Block.
 * 
 * Returns:
 *   BLOCK_MEMORY if the destination Block pointer is NULL.
 *   BLOCK_SUCCESS if the flag is set.
 */
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
