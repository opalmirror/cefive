/* block.h
 *   The Block Struct and associated functions.
 * Originally adapted from crt0_prx.c
 * 
 * Author: Sir Gee of Five
 */
#ifndef _BLOCK_H
#define _BLOCK_H

#include <psptypes.h>

/** Indicates success */
#define BLOCK_SUCCESS           (0)
/** Indicates failure */
#define BLOCK_FAILURE           (-1)
/** Indicates a NULL pointer */
#define BLOCK_NULLPTR           (-2)
/** Indicates a memory error */
#define BLOCK_MEMORY            (-3)

/** Indicates that a flag is clear */
#define BLOCK_FALSE             (0)
/** Indicates that a flag is set */
#define BLOCK_TRUE              (1)

/** Indicates a Dynamic Block */
#define BLOCK_FLAG_DMA  	((SceUChar8) 0x10) /* 00010000 */
/** Indicates a Constant Block */
#define BLOCK_FLAG_FREEZE       ((SceUChar8) 0x20) /* 00100000 */
/** Indicates that Block is a 32-bit word. */
#define BLOCK_FLAG_DWORD        ((SceUChar8) 0x04) /* 00000100 */
/** Indicates that Block is a 64-bit double word. */
#define BLOCK_FLAG_UWORD        ((SceUChar8) 0x08) /* 00001000 */
/** Indicates that Block is a 16-bit half word. */
#define BLOCK_FLAG_WORD         ((SceUChar8) 0x02) /* 00000010 */
/** Indicates that Block is an 8-bit byte. */
#define BLOCK_FLAG_BYTE         ((SceUChar8) 0x01) /* 00000001 */

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Block {
        /** Block flags */
        SceUChar8 flags;
        /** Address of a Block */
        SceUInt32 address;
        /** Memory value before patch */
        SceUInt32 stdVal;
        /** Patch value */
        SceUInt32 hakVal;
    } 
    /** The Block struct is used to represent a single Block of a Cheat.
     * A Block is a value that is to be patched over running memory.
     */
    Block;

    /** Clear the BYTE flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int block_clear_byte(Block* prBlock);

    /** Clear the DMA flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int block_clear_dma(Block* prBlock);

    /* Clear the DWORD flag of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing the block to clear.
     * 
     * Results:
     *   BLOCK_MEMORY is returned if the specified pointer is NULL.
     *   BLOCK_SUCCESS is returned if the Block flag is cleared.
     */
    int block_clear_dword(Block* prBlock);

    /* Clear the FREEZE flag of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing the block to clear.
     * 
     * Results:
     *   BLOCK_MEMORY is returned if the specified pointer is NULL.
     *   BLOCK_SUCCESS is returned if the Block flag is cleared.
     */
    int block_clear_freeze(Block* prBlock);

    /* Clear the UWORD flag of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing the block to clear.
     * 
     * Results:
     *   BLOCK_MEMORY is returned if the specified pointer is NULL.
     *   BLOCK_SUCCESS is returned if the Block flag is cleared.
     */
    int block_clear_qword(Block* prBlock);

    /* Clear the WORD flag of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing the block to clear.
     * 
     * Results:
     *   BLOCK_MEMORY is returned if the specified pointer is NULL.
     *   BLOCK_SUCCESS is returned if the Block flag is cleared.
     */
    int block_clear_word(Block* prBlock);

    /* Return the destination address of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing a Cheat Block.
     * 
     * Returns:
     *   An unsigned integer containing the 32-bit address of a Block is returned.
     */
    unsigned int block_get_address(Block* prBlock);

    /* Return the original value of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing a Cheat Block.
     * 
     * Returns:
     *   An unsigned integer containing the original Block value is returned.
     */
    unsigned int block_get_original(Block* prBlock);

    /* Return the patch value of the specified Block.
     * 
     * Parameters:
     *   prBlock    Pointer to a Block struct containing a Cheat Block.
     * 
     * Returns:
     *   An unsigned integer containing the patch value of the Block is returned.
     */
    unsigned int block_get_patch(Block* prBlock);

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
            unsigned char flags);

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
    int block_is_byte(Block* prBlock);

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
    int block_is_dma(Block* prBlock);

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
    int block_is_dword(Block* prBlock);

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
    int block_is_freeze(Block* prBlock);

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
    int block_is_qword(Block* prBlock);

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
    int block_is_word(Block* prBlock);

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
    int block_set_address(Block* prBlock, unsigned int addr);

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
    int block_set_byte(Block* prBlock);

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
    int block_set_dma(Block* prBlock);

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
    int block_set_dword(Block* prBlock);

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
    int block_set_freeze(Block* prBlock);

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
    int block_set_original(Block* prBlock, unsigned int orig);

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
    int block_set_patch(Block* prBlock, unsigned int patch);

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
    int block_set_qword(Block* prBlock);

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
    int block_set_word(Block* prBlock);

#ifdef	__cplusplus
}
#endif

#endif /* _BLOCK_H */
