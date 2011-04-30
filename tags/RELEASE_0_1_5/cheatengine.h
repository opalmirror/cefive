/* cheatengine.h
 *   Header file for the cefive Cheat Engine.
 * Author: Sir Gee of Five
 */

#ifndef _CHEATENGINE_H_
#define _CHEATENGINE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <pspkerneltypes.h>
#include "cheat.h"
#include "block.h"
#include "cefiveconfig.h"
#include "geelog.h"

#define CHEATENGINE_SUCCESS     (0)
#define CHEATENGINE_FAILURE     (-1)
#define CHEATENGINE_MEMORY      (-2)
#define CHEATENGINE_NULLPTR     (-3)
#define CHEATENGINE_FILEIO      (-4)

/* The maximum number of Cheat structs in the Cheat array. */
#define CHEATENGINE_CHEAT_MAX   512
/* The maximum number of Block structs in the Block array. */
#define CHEATENGINE_BLOCK_MAX   8192

typedef struct _CheatEngine {
    /* Pointer to a GeeLog struct representing the Logger. */
    GeeLog*     logger;
    /* Pointer to the first element of an array of Cheat structs. */
    Cheat*      cheatlist;
    /* Pointer to the first element of an array of Block structs. */
    Block*      blocklist;
    /* Pointer to a CEFiveConfig struct holding configuration settings. */
    CEFiveConfig* prConfig;
    /* The number of Cheats in the cheatlist array. */
    int         cheat_count;
    /* The number of Blocks in the blocklist array. */
    int         block_count;
    /* The base address of all Cheats and Blocks. */
    SceUInt32   base_address;
    /* Whether the Cheat Trigger is active (Music Key) */
    int         trigger_active;
} 
/* The CheatEngine struct represents a Cheat Engine.  The struct contains
 * members to encapsulate logging, Cheats and configuration.  Nearly all
 * Cheat Engine functions require a pointer to a CheatEngine struct.
 */
CheatEngine;

/* Activate all Cheats that are currently inactive.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Cheats have been activated.
 */
int cheatengineActivateCheats(CheatEngine* prEng);

/* Add and return a pointer to a new Block in a Cheat Engine by specifying the
 * values of the Block to add.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   flags      8-bit Block flags (see Block.h)
 *   addr       32-bit unsigned integer containing the address of the Block.
 *   value      32-bit unsigned integer containing the patch value of the Block.
 * 
 * Returns:
 *   A pointer to the newly created Block struct is returned.
 *   NULL is returned if the Block could not be created.
 */
Block* cheatengineAddBlock(CheatEngine *prEng, 
        unsigned char flags, unsigned int addr, unsigned int value);

/* Add a Cheat to the specified Cheat Engine returning a pointer to the newly
 * added Cheat struct.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   A pointer to a Cheat struct representing the newly added Cheat is returned.
 *   NULL is returned if a Cheat could not be added.
 */
Cheat* cheatEngineAddCheat(CheatEngine *prEng);

/* Apply the specified Block to memory.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int specifying the Block in the Block array to apply.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Block pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is applied to memory.
 */
int cheatengineApplyBlock(CheatEngine *prEng, int index);

/* Apply the specified Cheat to memory.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int specifying the Cheat in the Cheat array to apply.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL, 
 *      or if the Cheat could not be applied to memory.
 *   CHEATENGINE_SUCCESS is returned if the Cheat is applied to memory.
 */
int cheatengineApplyCheat(CheatEngine *prEng, int index);

/* Apply the specified Block to memory as a single byte.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct representing the Cheat Block.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
 *      or if the memory already contains the value that would have been
 *      applied.
 */
int cheatengineApplyUChar8Block(Block* prBlock);

/* Apply the specified Block to memory as a 32-bit value.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct representing the Cheat Block.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
 *      or if the memory already contains the value that would have been
 *      applied.
 */
int cheatengineApplyUInt32Block(Block* prBlock);

/* Apply the specified Block to memory as a 16-bit value.
 * 
 * Parameters:
 *   prBlock    Pointer to a Block struct representing the Cheat Block.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the value has been applied to memory, 
 *      or if the memory already contains the value that would have been
 *      applied.
 */
int cheatengineApplyUShort16Block(Block* prBlock);

/* Deactivate all of the currently activated Cheats that are selected.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Cheats have been deactivated.
 * 
 */
int cheatengineDeactivateCheats(CheatEngine* prEng);

/* Return a pointer to the specified Block from a Cheat Engine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Block to return.
 * 
 * Returns:
 *   NULL is returned if prEng is NULL, or if the specified index is out of
 *      bounds.
 *   A Pointer to the specified Block struct is returned otherwise.
 */
Block* cheatengineGetBlock(CheatEngine *, int index);

/* Return a pointer to the specified Cheat from a Cheat Engine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to return.
 * 
 * Returns:
 *   NULL is returned if prEng is NULL, or if the specified index is out of
 *      bounds.
 *   A Pointer to the specified Cheat struct is returned otherwise.
 */
Cheat* cheatengineGetCheat(CheatEngine *, int index);

/* Initialize a CheatEngine struct by specifying initial pointers.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct to initialize.
 *   prCfg      Pointer to a CEFiveConfig struct containing configuration
 *              settings.
 *   arCheat    Pointer to the first element of an array of Cheat structs.
 *   arBlock    Pointer to the first element of an array of Block structs.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng or prCfg are NULL.
 *   CHEATENGINE_SUCCESS is returned if the struct is initialized.
 */
int cheatengineInit(CheatEngine* prEng, CEFiveConfig* prCfg, Cheat* arCheat,
        Block* arBlock);

/* Load initial Cheats and Blocks from the configured cheatfile_path of the
 * specified Cheat Engine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL or if the Configuration
 *      pointer within prEng is NULL.
 *   CHEATENGINE_FILEIO is returned if an I/O Error occurs.
 *   CHEATENGINE_SUCCESS is returned if the file is loaded.
 */
int cheatengineLoadCheats(CheatEngine* prEng);

/* Add a log message to the current Cheat Engine Logger if applicable.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing a Cheat Engine.
 *   rLevel     ELogLevel enum value indicating log level of message.
 *   sMsg       String containing message to log.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR if prEng is NULL.
 *   CHEATENGINE_FAILURE if the Cheat Engine has no logger.
 *   CHEATENGINE_SUCCESS if the message was sent to the logger.
 */
int cheatengineLog(CheatEngine* prEng, ELogLevel rLevel, const char* sMsg);

/* Refresh the specified Cheat Engine, Applying or Resetting cheats as 
 * necessary.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Cheat Engine is refreshed.
 */
int cheatengineRefresh(CheatEngine* prEng);

/* Reset a previously activated Block by specifying the index.  A Block that
 * is Reset will return memory to it's value when the Block was applied.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the Block pointer specified by index is
 *      NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Block has been Reset.
 */
int cheatengineResetBlock(CheatEngine *prEng, int index);

/* Reset a previously activated Cheat by specifying the index.  A Cheat that
 * is Reset will have each of the Blocks belonging to the Cheat Reset.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the Cheat pointer specified by index is
 *      NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat has been Reset.
 */
int cheatengineResetCheat(CheatEngine *prEng, int index);

/* Reset the specified Block by returning the memory to its original value.
 * The memory is replaced as an 8-bit value.
 * 
 * Parameters:
 *   prBlock        Pointer to a Block struct representing the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is reset.
 */
int cheatengineResetUChar8Block(Block* prBlock);

/* Reset the specified Block by returning the memory to its original value.
 * The memory is replaced as a 32-bit value.
 * 
 * Parameters:
 *   prBlock        Pointer to a Block struct representing the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is reset.
 */
int cheatengineResetUInt32Block(Block* prBlock);

/* Reset the specified Block by returning the memory to its original value.
 * The memory is replaced as a 16-bit value.
 * 
 * Parameters:
 *   prBlock        Pointer to a Block struct representing the Block to reset.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prBlock is NULL.
 *   CHEATENGINE_SUCCESS is returned if the Block is reset.
 */
int cheatengineResetUShort16Block(Block* prBlock);

/* Set a specified Cheat to constant.  A constant cheat is activated when a
 * Cheat is applied, but is not reset when the Cheat is reset.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to set constant.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat is set to constant.
 */
int cheatengineSetCheatConstant(CheatEngine* prEng, int index);

/* Set a specified Cheat to inactive.  An inactive cheat is not applied at any
 * time.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to set inactive.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat is set to inactive.
 */
int cheatengineSetCheatInactive(CheatEngine* prEng, int index);

/* Set a specified Cheat to selected.  A selected cheat is activated when a
 * Cheat is applied, and is reset when the Cheats are reset.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct representing the Cheat Engine.
 *   index      int index of the Cheat to set selected.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_FAILURE is returned if the specified Cheat pointer is NULL.
 *   CHEATENGINE_SUCCESS is returned if the specified Cheat is set to selected.
 */
int cheatengineSetCheatSelected(CheatEngine* prEng, int index);

/* Assign a GeeLog pointer to act as the logging facility for the specified
 * CheatEngine.
 * 
 * Parameters:
 *   prEng      Pointer to a CheatEngine struct that represents the Cheat
 *              Engine.
 *   prLog      Pointer to a GeeLog struct that represents the Logger.
 * 
 * Returns:
 *   CHEATENGINE_NULLPTR is returned if prEng is NULL.
 *   CHEATENGINE_SUCCESS is returned if the GeeLog logger is assigned.
 */
int cheatengineSetLogger(CheatEngine* prEng, GeeLog* prLog);

#ifdef	__cplusplus
}
#endif

#endif
