/* 
 * File:   gcheatengine.h
 * Author: george
 *
 * Created on May 14, 2011, 3:29 PM
 */

#ifndef GCHEATENGINE_H
#define	GCHEATENGINE_H

#include "cheat.h"
#include "block.h"

#define GCHEATENGINE_SUCCESS (0)
#define GCHEATENGINE_FAILURE (-1)
#define GCHEATENGINE_NULLPTR (-2)
#define GCHEATENGINE_MEMORY (-3)
#define GCHEATENGINE_IOERROR (-4)
#define GCHEATENGINE_INVIDX (-5)

#define GCHEATENGINE_ACTIVE_MAX (256)
#define GCHEATENGINE_BLOCK_MAX (16384)
#define GCHEATENGINE_CHEAT_MAX (1024)
#define GCHEATENGINE_INIT_PRIO (24)
#define GCHEATENGINE_STACK_SIZE (16384)

#define GCHEATENGINE_THREAD_NAME "GCheatEngineThread"
#define GCHEATENGINE_MUTEX_NAME "GCheatEngineMutex"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _EChRunState {
        ECHRS_Stopped = 0,
        ECHRS_Starting,
        ECHRS_Started,
        ECHRS_Running,
        ECHRS_Stopping
    } EChRunState;

    typedef struct _GCheatEngine {
        /** The current Run State of the Cheat Engine. */
        EChRunState rRunState;
        /** An Array of Cheat structs representing Cheats. */
        Cheat arCheat[GCHEATENGINE_CHEAT_MAX];
        /** An Array of Block structs representing Blocks. */
        Block arBlock[GCHEATENGINE_BLOCK_MAX];
        /** An array of int indexes representing active Cheats. */
        int aiActive[GCHEATENGINE_ACTIVE_MAX];
        /** The number of loaded Cheats. */
        int iCheats;
        /** The number of loaded Blocks. */
        int iBlocks;
    } GCheatEngine;

    /** Return a pointer to a Block struct representing the indicated Block.
     * 
     * @param index int specifying the Block requested.
     * @return A pointer to a Block struct or NULL is returned.
     */
    Block* gcheatengine_get_block(int index);
    
    /** Return a pointer to a Cheat struct representing the indicated Cheat.
     * 
     * @param index int specifying the Cheat requested.
     * @return A pointer to a Cheat struct or NULL is returned.
     */
    Cheat* gcheatengine_get_cheat(int index);
    
    /** Initialize the Cheat Engine.
     * 
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gcheatengine_init();
    
    /** Set the status of the indicated Cheat to constant.
     * 
     * @param index int containing the index of the Cheat to set.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gcheatengine_set_constant(int index);
    
    /** Clear the status of the indicated Cheat and remove any currently
     * applied patch from memory.
     * 
     * @param index int containing the index of the Cheat to clear.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gcheatengine_set_inactive(int index);
    
    /** Set the status of the indicated Cheat to triggered.
     * 
     * @param index int containing the index of the Cheat to set.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gcheatengine_set_triggered(int index);
    
    /** Starts the Cheat Engine, dispatching the Cheat Engine Thread.
     * 
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gcheatengine_start();
    
    /** Stops a running Cheat Engine.
     * 
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gcheatengine_stop();

#ifdef	__cplusplus
}
#endif

#endif	/* GCHEATENGINE_H */

