/* block.h
 *   The Block Struct and associated functions.
 * Originally adapted from crt0_prx.c
 * 
 * Author: Sir Gee of Five
 */
#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <pspkerneltypes.h>

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
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_clear_byte(Block* prBlock);

    /** Clear the DMA flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_clear_dma(Block* prBlock);

    /** Clear the DWORD flag of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing the block to clear.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_clear_dword(Block* prBlock);

    /** Clear the FREEZE flag of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing the block to clear.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_clear_freeze(Block* prBlock);

    /** Clear the UWORD flag of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing the block to clear.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_clear_qword(Block* prBlock);

    /** Clear the WORD flag of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing the block to clear.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_clear_word(Block* prBlock);

    /** Return the destination address of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing a Cheat Block.
     * @return An unsigned integer containing the 32-bit address of a Block is 
     * returned.
     */
    SceUInt32 block_get_address(Block* prBlock);

    /** Return the original value of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing a Cheat Block.
     * @return An unsigned integer containing the original Block value is 
     * returned.
     */
    SceUInt32 block_get_original(Block* prBlock);

    /** Return the patch value of the specified Block.
     * 
     * @param prBlock Pointer to a Block struct containing a Cheat Block.
     * @return An unsigned integer containing the patch value of the Block is 
     * returned.
     */
    SceUInt32 block_get_patch(Block* prBlock);

    /** Initialize the specified Block struct with the specified values.
     * 
     * @param prBlock Pointer to a Block struct to initialize.
     * @param address Unsigned integer containing the 32-bit address of the 
     * Block.
     * @param value Unsigned integer containing the patch value of the Block.
     * @param flags Unsigned char containing the flags of the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_init(Block* prBlock, SceUInt32 address, SceUInt32 value,
            SceUChar8 flags);

    /** Indicate whether the BYTE flag is set in a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates the flag is not set, 1 indicates the flag is set.
     */
    int block_is_byte(Block* prBlock);

    /** Indicate whether the DMA flag is set in a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates the flag is not set, 1 indicates the flag is set.
     */
    int block_is_dma(Block* prBlock);

    /** Indicate whether the DWORD flag is set in a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates the flag is not set, 1 indicates the flag is set.
     */
    int block_is_dword(Block* prBlock);

    /** Indicate whether the FREEZE flag is set in a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates the flag is not set, 1 indicates the flag is set.
     */
    int block_is_freeze(Block* prBlock);

    /** Indicate whether the UWORD flag is set in a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates the flag is not set, 1 indicates the flag is set.
     */
    int block_is_qword(Block* prBlock);

    /** Indicate whether the WORD flag is set in a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates the flag is not set, 1 indicates the flag is set.
     */
    int block_is_word(Block* prBlock);

    /** Assign an address to a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @param addr the address to assign.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_address(Block* prBlock, SceUInt32 addr);

    /** Set the BYTE flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_byte(Block* prBlock);

    /** Set the DMA flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_dma(Block* prBlock);

    /** Set the DWORD flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_dword(Block* prBlock);

    /** Set the FREEZE flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_freeze(Block* prBlock);

    /** Assign the original memory value of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @param orig the original memory value to assign.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_original(Block* prBlock, SceUInt32 orig);

    /** Assign the patch memory value of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @param orig the patch memory value to assign.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_patch(Block* prBlock, SceUInt32 patch);

    /** Set the UWORD flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_qword(Block* prBlock);

    /** Set the WORD flag of a Block.
     * 
     * @param prBlock Pointer to a Block struct representing the Block.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int block_set_word(Block* prBlock);

#ifdef	__cplusplus
}
#endif

#endif /* BLOCK_H */
