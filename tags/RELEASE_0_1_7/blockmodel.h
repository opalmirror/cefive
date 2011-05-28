/* 
 * File:   blockmodel.h
 * Author: Sir Gee of Five
 *
 * Created on May 21, 2011, 5:35 PM
 */

#ifndef BLOCKMODEL_H
#define	BLOCKMODEL_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspsysmem.h>
#include "block.h"

/** Indicates success. */
#define BLOCKMODEL_SUCCESS (0)

/** Indicates failure. */
#define BLOCKMODEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define BLOCKMODEL_NULLPTR (-2)

/** Indicates a memory error. */
#define BLOCKMODEL_MEMORY (-3)

/** Indicates an invalid index. */
#define BLOCKMODEL_INVIDX (-4)

/** The base Partition Block name. */
#define BLOCKMODEL_BLOCK_NAME "BlockModel"

/** The length of a Partition Block name. */
#define BLOCKMODEL_BLKNAMLEN (20)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _BlockModel {
        /** Memory Partition Block Id. */
        SceUID blockId;
        /** Pointer to base element of Block List. */
        Block* blockList;
        /** Number of elements in the List. */
        int rowCount;
        /** The capacity of the List in elements. */
        unsigned int capacity;
        /** The number of elements to expand the list by when expanding. */
        unsigned int expansion;
    }
    /** The BlockModel struct represents a Block Model. */
    BlockModel;

    /** Add a Block to a Block Model and return a pointer to the newly added
     * Block.
     * 
     * @param prModel Pointer to a BlockModel struct representing the Block
     * Model.
     * @return A pointer to a Block struct or NULL is returned.
     */
    Block* blockmodel_add(BlockModel* prModel);
    
    /** Allocate enough Kernel Partition Memory to hold the indicated number
     * of Block Elements in a Block Model.
     * 
     * @param prModel Pointer to a BlockModel struct representing the Block
     * Model.
     * @param elements int indicating the number of Block elements to allocate.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int blockmodel_allocate(BlockModel* prModel, const int elements);
    
    /** Free the allocated Kernel Partition Memory belonging to a Block Model.
     * 
     * @param prModel Pointer to a BlockModel struct representing the Block
     * Model.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int blockmodel_delete(BlockModel* prModel);
    
    /** Expand a Block Model by the configured amount.
     * 
     * @param prModel Pointer to a BlockModel struct representing the Block
     * Model.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int blockmodel_expand(BlockModel* prModel);
    
    /** Return a pointer to a Block struct representing the indicated Block.
     * 
     * @param prModel Pointer to a BlockModel struct representing the Block
     * Model.
     * @param index int indicating the index of the Block to return.
     * @return A pointer to a Block struct or NULL is returned.
     */
    Block* blockmodel_get(BlockModel* prModel, const int index);
    
    /** Initialize a Block Model.
     * 
     * @param prModel Pointer to the BlockModel struct to initialize.
     * @param rowCount int containing the initial number of elements.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int blockmodel_init(BlockModel* prModel, const int rowCount);

    /** Reallocate a Block Model to hold the specified number of elements.
     * Preserve the existing elements in the Block Model.
     * 
     * @param prModel Pointer to a BlockModel struct representing the Block
     * Model.
     * @param elements int indicating the number of Block elements to allocate.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int blockmodel_reallocate(BlockModel* prModel, const int elements);
    
#ifdef	__cplusplus
}
#endif

#endif	/* BLOCKMODEL_H */

