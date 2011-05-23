/* cheatengine.h
 *   Header file for the cefive Cheat Engine.
 * Author: Sir Gee of Five
 */

#ifndef _CHEATENGINE_H_
#define _CHEATENGINE_H_

#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "cheat.h"
#include "block.h"
#include "cefiveconfig.h"
#include "cheatmodel.h"
#include "blockmodel.h"
#include "geelog.h"

/** Indicates success. */
#define CHEATENGINE_SUCCESS     (0)

/** Indicates failure. */
#define CHEATENGINE_FAILURE     (-1)

/** Indicates a memory error. */
#define CHEATENGINE_MEMORY      (-2)

/** Indicates a NULL pointer. */
#define CHEATENGINE_NULLPTR     (-3)

/** Indicates an I/O Error. */
#define CHEATENGINE_FILEIO      (-4)

/** The maximum number of Cheat structs in the Cheat array. */
#define CHEATENGINE_CHEAT_MAX   512

/** The maximum number of Block structs in the Block array. */
#define CHEATENGINE_BLOCK_MAX   8192

#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct _CheatEngine {
        /** CheatModel struct representing the Cheat Model. */
        CheatModel cheatModel;
        /** BlockModel struct representing the Block Model. */
        BlockModel blockModel;
        /** Pointer to the first element of an array of Cheat structs. */
        Cheat* cheatlist;
        /** Pointer to the first element of an array of Block structs. */
        Block* blocklist;
        /** Pointer to a CEFiveConfig struct holding configuration settings. */
        CEFiveConfig* prConfig;
        /** The number of Cheats in the cheatlist array. */
        int cheat_count;
        /** The number of Blocks in the blocklist array. */
        int block_count;
        /** The base address of all Cheats and Blocks. */
        SceUInt32 base_address;
        /** Whether the Cheat Trigger is active (Music Key) */
        int trigger_active;
    }
    /** The CheatEngine struct represents a Cheat Engine.  The struct contains
     * members to encapsulate logging, Cheats and configuration.  Nearly all
     * Cheat Engine functions require a pointer to a CheatEngine struct.
     */
    CheatEngine;

    /** Add a new Block to a Cheat Engine, returning a pointer to the newly
     * added Block.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return A pointer to a Block struct or NULL is returned. 
     */
    Block* cheatengine_add_block(CheatEngine* prEng);

    /** Add a new Cheat to a Cheat Engine, returning a pointer to the newly
     * added Cheat.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return A pointer to a Cheat struct or NULL is returned. 
     */
    Cheat* cheatengine_add_cheat(CheatEngine* prEng);
    
    /** Return a pointer to a Block struct representing the indicated Block.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @param index int indicating the Block to return.
     * @return A pointer to a Block struct or NULL is returned.
     */
    Block* cheatengine_get_block(CheatEngine* prEng, const int index);
    
    /** Return the number of Block elements in a Cheat Engine.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return The number of Block elements or -1 is returned. 
     */
    int cheatengine_get_blockcount(CheatEngine* prEng);
    
    /** Return a pointer to a BlockModel struct representing the Block Model.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return A pointer to a BlockModel struct or NULL is returned. 
     */
    BlockModel* cheatengine_get_blockmodel(CheatEngine* prEng);
    
    /** Return a pointer to a Cheat struct representing the indicated Cheat.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @param index int indicating the Cheat to return.
     * @return A pointer to a Cheat struct or NULL is returned.
     */
    Cheat* cheatengine_get_cheat(CheatEngine* prEng, const int index);
    
    /** Return the number of Cheat elements in a Cheat Engine.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return The number of Cheat elements or -1 is returned. 
     */
    int cheatengine_get_cheatcount(CheatEngine* prEng);
    
    /** Return a pointer to a CheatModel struct representing the Cheat Model.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return A pointer to a CheatModel struct or NULL is returned.
     */
    CheatModel* cheatengine_get_cheatmodel(CheatEngine* prEng);
    
    /** Activate all Cheats that are currently inactive.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @return CHEATENGINE_NULLPOTR is returned if the parameter prEng is NULL.
     * CHEATENGINE_SUCCESS is returned if the Cheats are activated.
     */
    int cheatengineActivateCheats(CheatEngine* prEng);

    /** Add and return a pointer to a new Block in a Cheat Engine by specifying the
     * values of the Block to add.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param flags 8-bit Block flags (see Block.h)
     * @param addr 32-bit unsigned integer containing the address of the Block.
     * @param value 32-bit unsigned integer containing the patch value of the Block.
     * @return A pointer to the newly created Block struct is returned.
     * NULL is returned if the Block could not be created.
     */
    Block* cheatengineAddBlock(CheatEngine *prEng,
            unsigned char flags, unsigned int addr, unsigned int value);

    /** Add a Cheat to the specified Cheat Engine returning a pointer to the newly
     * added Cheat struct.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @return A pointer to a Cheat struct representing the newly added Cheat is 
     * returned. NULL is returned if a Cheat could not be added.
     */
    Cheat* cheatEngineAddCheat(CheatEngine *prEng);

    /** Apply the specified Block to memory.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int specifying the Block in the Block array to apply.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL. CHEATENGINE_FAILURE
     * is returned if the specified Block pointer is NULL. CHEATENGINE_SUCCESS is 
     * returned if the Block is applied to memory.
     */
    int cheatengineApplyBlock(CheatEngine *prEng, int index);

    /** Apply the specified Cheat to memory.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int specifying the Cheat in the Cheat array to apply.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.
     * CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL, 
     * or if the Cheat could not be applied to memory.
     * CHEATENGINE_SUCCESS is returned if the Cheat is applied to memory.
     */
    int cheatengineApplyCheat(CheatEngine *prEng, int index);

    /** Apply the specified Block to memory as a single byte.
     * 
     * @param prBlock Pointer to a Block struct representing the Cheat Block.
     * @return CHEATENGINE_NULLPTR is returned if prBlock is NULL.
     * CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
     * or if the memory already contains the value that would have been applied.
     */
    int cheatengineApplyUChar8Block(Block* prBlock);

    /** Apply the specified Block to memory as a 32-bit value.
     * 
     * @param prBlock Pointer to a Block struct representing the Cheat Block.
     * @return CHEATENGINE_NULLPTR is returned if prBlock is NULL.
     * CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
     * or if the memory already contains the value that would have been applied.
     */
    int cheatengineApplyUInt32Block(Block* prBlock);

    /** Apply the specified Block to memory as a 16-bit value.
     * 
     * @param prBlock Pointer to a Block struct representing the Cheat Block.
     * @return CHEATENGINE_NULLPTR is returned if prBlock is NULL.
     * CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
     * or if the memory already contains the value that would have been applied.
     */
    int cheatengineApplyUShort16Block(Block* prBlock);

    /** Deactivate all of the currently activated Cheats that are selected.
     * 
     * @paranm prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.
     * CHEATENGINE_SUCCESS is returned if the Cheats have been deactivated.
     */
    int cheatengineDeactivateCheats(CheatEngine* prEng);

    /** Return a pointer to the specified Block from a Cheat Engine.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Block to return.
     * @return NULL is returned if prEng is NULL, or if the specified index is out 
     * of bounds.  A Pointer to the specified Block struct is returned otherwise.
     */
    Block* cheatengineGetBlock(CheatEngine *, int index);

    /** Return a pointer to the specified Cheat from a Cheat Engine.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Cheat to return.
     * @return NULL is returned if prEng is NULL, or if the specified index is out 
     * of bounds.  A Pointer to the specified Cheat struct is returned otherwise.
     */
    Cheat* cheatengineGetCheat(CheatEngine *, int index);

    /** Initialize a CheatEngine struct by specifying initial pointers.
     * 
     * @param prEng Pointer to a CheatEngine struct to initialize.
     * @param prCfg Pointer to a CEFiveConfig struct containing configuration
     * settings.
     * @param arCheat Pointer to the first element of an array of Cheat structs.
     * @param arBlock Pointer to the first element of an array of Block structs.
     * @return CHEATENGINE_NULLPTR is returned if prEng or prCfg are NULL.
     * CHEATENGINE_SUCCESS is returned if the struct is initialized.
     */
    int cheatengineInit(CheatEngine* prEng, CEFiveConfig* prCfg, Cheat* arCheat,
            Block* arBlock);

    /** Load initial Cheats and Blocks from the configured cheatfile_path of the
     * specified Cheat Engine.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL or if the 
     * Configuration pointer within prEng is NULL.  CHEATENGINE_FILEIO is returned 
     * if an I/O Error occurs.  CHEATENGINE_SUCCESS is returned if the file is 
     * loaded.
     */
    int cheatengineLoadCheats(CheatEngine* prEng);

    /** Refresh the specified Cheat Engine, Applying or Resetting cheats as 
     * necessary.
     * 
     * @param prEng Pointer to a CheatEngine struct representing a Cheat Engine.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.
     * CHEATENGINE_SUCCESS is returned if the Cheat Engine is refreshed.
     */
    int cheatengineRefresh(CheatEngine* prEng);

    /** Reset a previously activated Block by specifying the index.  A Block that
     * is Reset will return memory to it's value when the Block was applied.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Block to reset.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.  
     * CHEATENGINE_FAILURE is returned if the Block pointer specified by index is
     * NULL.  CHEATENGINE_SUCCESS is returned if the specified Block has been Reset.
     */
    int cheatengineResetBlock(CheatEngine *prEng, int index);

    /** Reset a previously activated Cheat by specifying the index.  A Cheat that
     * is Reset will have each of the Blocks belonging to the Cheat Reset.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Cheat to reset.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL. 
     * CHEATENGINE_FAILURE is returned if the Cheat pointer specified by index is
     * NULL.  CHEATENGINE_SUCCESS is returned if the specified Cheat has been Reset.
     */
    int cheatengineResetCheat(CheatEngine *prEng, int index);

    /** Reset the specified Block by returning the memory to its original value.
     * The memory is replaced as an 8-bit value.
     * 
     * @param prBlock Pointer to a Block struct representing the Block to reset.
     * @return CHEATENGINE_NULLPTR is returned if prBlock is NULL.  
     * CHEATENGINE_SUCCESS is returned if the Block is reset.
     */
    int cheatengineResetUChar8Block(Block* prBlock);

    /** Reset the specified Block by returning the memory to its original value.
     * The memory is replaced as a 32-bit value.
     * 
     * @param prBlock Pointer to a Block struct representing the Block to reset.
     * @return CHEATENGINE_NULLPTR is returned if prBlock is NULL.  
     * CHEATENGINE_SUCCESS is returned if the Block is reset.
     */
    int cheatengineResetUInt32Block(Block* prBlock);

    /** Reset the specified Block by returning the memory to its original value.
     * The memory is replaced as a 16-bit value.
     * 
     * @param prBlock Pointer to a Block struct representing the Block to reset.
     * @return CHEATENGINE_NULLPTR is returned if prBlock is NULL.  
     * CHEATENGINE_SUCCESS is returned if the Block is reset.
     */
    int cheatengineResetUShort16Block(Block* prBlock);

    /** Set a specified Cheat to constant.  A constant cheat is activated when a
     * Cheat is applied, but is not reset when the Cheat is reset.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Cheat to set constant.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.  
     * CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
     * CHEATENGINE_SUCCESS is returned if the specified Cheat is set to constant.
     */
    int cheatengineSetCheatConstant(CheatEngine* prEng, int index);

    /** Set a specified Cheat to inactive.  An inactive cheat is not applied at any
     * time.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Cheat to set inactive.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.  
     * CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
     * CHEATENGINE_SUCCESS is returned if the specified Cheat is set to inactive.
     */
    int cheatengineSetCheatInactive(CheatEngine* prEng, int index);

    /** Set a specified Cheat to selected.  A selected cheat is activated when a
     * Cheat is applied, and is reset when the Cheats are reset.
     * 
     * @param prEng Pointer to a CheatEngine struct representing the Cheat Engine.
     * @param index int index of the Cheat to set selected.
     * @return CHEATENGINE_NULLPTR is returned if prEng is NULL.  
     * CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
     * CHEATENGINE_SUCCESS is returned if the specified Cheat is set to selected.
     */
    int cheatengineSetCheatSelected(CheatEngine* prEng, int index);

#ifdef	__cplusplus
}
#endif

#endif
