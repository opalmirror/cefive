/* 
 * File:   cheatmodel.h
 * Author: Sir Gee of Five
 *
 * Created on May 21, 2011, 4:54 PM
 */

#ifndef CHEATMODEL_H
#define	CHEATMODEL_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspsysmem.h>
#include "cheat.h"

/** Indicates success. */
#define CHEATMODEL_SUCCESS (0)

/** Indicates failure. */
#define CHEATMODEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define CHEATMODEL_NULLPTR (-2)

/** Indicates a memory error. */
#define CHEATMODEL_MEMORY (-3)

/** Indicates an invalid index. */
#define CHEATMODEL_INVIDX (-4)

#define CHEATMODEL_BLOCK_NAME "CheatModel"

#define CHEATMODEL_BLKNMLEN (20)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CheatModel {
        /** Memory Partition Block Id. */
        SceUID blockId;
        /** Pointer to a Cheat struct representing the base element of a list
         * of Cheats.
         */
        Cheat* cheatList;
        /** The number of Cheats in the Model. */
        int rowCount;
        /** The capacity of the List in elements. */
        unsigned int capacity;
        /** The number of elements to expand the list by when expanding. */
        unsigned int expansion;
    }
    /** The CheatModel struct is used to represent a Cheat Model.
     * A Cheat Model represents all of the Cheats for a particular Game.
     */
    CheatModel;

    /** Add a new Cheat to a Cheat Model and return a pointer to the newly added
     * Cheat.
     * 
     * @param prModel Pointer to a CheatModel struct representing the Cheat
     * Model.
     * @return A pointer to a Cheat struct or NULL is returned.
     */
    Cheat* cheatmodel_add(CheatModel* prModel);
    
    /** Allocate enough Kernel Partition memory to hold the indicated number of
     * Cheat elements in a Cheat Model.
     * 
     * @param prModel Pointer to a CheatModel struct representing the Cheat
     * Model.
     * @param elements int indicating the number of Cheat elements to allocate.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatmodel_allocate(CheatModel* prModel, const int elements);
    
    /** Free the allocated Kernel Partition memory belonging to a Cheat Model.
     * 
     * @param prModel Pointer to a CheatModel struct representing the Cheat
     * Model.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatmodel_delete(CheatModel* prModel);
    
    /** Expand a Cheat Model by the configured amount.
     * 
     * @param prModel Pointer to a CheatModel struct representing the Cheat
     * Model.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatmodel_expand(CheatModel* prModel);
    
    /** Return a pointer to a Cheat struct representing the indicated Cheat.
     * 
     * @param prModel Pointer to a CheatModel struct representing the Cheat
     * Model.
     * @param index int indicating the index of the Cheat to return.
     * @return A pointer to a Cheat struct or NULL is returned.
     */
    Cheat* cheatmodel_get(CheatModel* prModel, int index);
    
    /** Copy the name of the indicated Cheat to the indicated string pointer.
     * 
     * @param sDest char pointer representing the destination string.
     * @param prModel Pointer to a CheatModel struct representing the Cheat 
     * Model.
     * @param index int indicating the index of the Cheat to return the name of.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatmodel_get_name(char* sDest, CheatModel* prModel, int index);
    
    /** Initialize a Cheat Model.
     * 
     * @param prModel Pointer to the CheatModel struct to initialize.
     * @param rowCount int indicating the number of Cheats in the Model.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatmodel_init(CheatModel* prModel, int rowCount);

    /** Reallocate a Cheat Model to hold the specified number of Cheat elements.
     * Preserve the existing elements in a Cheat Model.
     * 
     * @param prModel Pointer to a CheatModel struct representing the Cheat
     * Model.
     * @param elements int indicating the number of Cheat elements to allocate.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatmodel_reallocate(CheatModel* prModel, const int elements);
    
#ifdef	__cplusplus
}
#endif

#endif	/* CHEATMODEL_H */

