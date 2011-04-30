/* cheat.h
 *      The Cheat struct and related functions.
 * Originally adapted from crt0_prx.c
 * 
 * Author: Sir Gee of Five
 */

#ifndef _CHEAT_H
#define _CHEAT_H

#ifdef	__cplusplus
extern "C" {
#endif

    /** Indicates Cheat is selected. */
#define CHEAT_FLAG_SELECTED (1<<0)

    /** Indicates Cheat is constant. */
#define CHEAT_FLAG_CONSTANT	(1<<1)

    /** Indicates Cheat was recently activated or reset. */
#define CHEAT_FLAG_FRESH    (1<<2)

    /** Indicates Success of a function. */
#define CHEAT_SUCCESS       (0)

    /** Indicates Failure of a function. */
#define CHEAT_FAILURE       (-1)

    /** Indicates a Memory allocation fault in a function. */
#define CHEAT_MEMORY        (-2)

    typedef struct _Cheat {
        /** The first Block index of a Cheat. */
        unsigned short block;
        /** The number of Blocks belonging to the Cheat. */
        unsigned short len;
        /** Cheat Flags */
        unsigned char flags;
        /** Display Name of the Cheat */
        unsigned char name[32];
    }
    /** The Cheat struct is used to hold a single Cheat in the Cheat Engine.  
     * The structure contains members for the block index, number of blocks, 
     * flags and the name of the Cheat.  The structure maintains compatibility 
     * with the previous NitePr struct by the same name.
     */
    Cheat;

    /** Clear all of the flags in the specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct to clear.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flags are cleared.
     */
    int cheat_clear_flags(Cheat* prCheat);

    /** Clear the CONSTANT cheat flag in the specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct containing the Cheat to clear.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flags are cleared.
     */
    int cheat_clearflag_constant(Cheat* prCheat);

    /** Clear the FRESH cheat flag in the specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct containing the Cheat to clear.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flags are cleared.
     */
    int cheat_clearflag_fresh(Cheat* prCheat);

    /** Clear the SELECTED cheat flag in the specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct containing the Cheat to clear.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flags are cleared.
     */
    int cheat_clearflag_selected(Cheat* prCheat);

    /** Return the index of the first Block that belongs to the specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct representing the Cheat
     * @return An unsigned integer containing the index within the Block array 
     * of the first Block belonging to the Cheat.
     */
    unsigned int cheat_get_block(Cheat* prCheat);

    /** Return the number of Block structs in the Block array that belong to the
     * specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct representing the Cheat
     * @return An unsigned integer containing the number of Blocks in the Block 
     * array that belong to the Cheat.
     */
    unsigned int cheat_get_length(Cheat *prCheat);

    /** Return an integer indicating whether the CONSTANT flag of the specified
     * Cheat is set.
     * 
     * @param prCheat Pointer to a Cheat struct to return flag indications for.
     * @return 1 is returned if the flag is set, 0 is returned if the flag is 
     * clear.
     */
    int cheat_is_constant(Cheat* prCheat);

    /** Return an integer indicating whether the FRESH flag of the specified
     * Cheat is set.
     * 
     * @param prCheat Pointer to a Cheat struct to return flag indications for.
     * @return 1 is returned if the flag is set, 0 is returned if the flag is 
     * clear.
     */
    int cheat_is_fresh(Cheat* prCheat);

    /** Return an integer indicating whether the CONSTANT flag and the SELECTED
     * flag of the specified Cheat is set.
     * 
     * @param prCheat Pointer to a Cheat struct to return flag indications for.
     * @return 1 is returned if the flag is set, 0 is returned if the flag is 
     * clear.
     */
    int cheat_is_inactive(Cheat* prCheat);

    /** Return an integer indicating whether the SELECTED flag of the specified
     * Cheat is set.
     * 
     * @param prCheat Pointer to a Cheat struct to return flag indications for.
     * @return 1 is returned if the flag is set, 0 is returned if the flag is 
     * clear.
     */
    int cheat_is_selected(Cheat* prCheat);

    /** Assign the index of the first Block in the Block array that belongs to 
     * the specified Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct to assign the Block index for.
     * @param block unsigned int containing the index to assign.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the block index is assigned.
     */
    int cheat_set_block(Cheat* prCheat, unsigned int block);

    /** Sets the specified Cheat to a Constant Cheat.  A Constant Cheat is a 
     * cheat that activates when the Cheat Engine is activated, but remains 
     * active when the Cheat Engine is deactivated.
     * 
     * @param prCheat Pointer to the Cheat Struct to set the flag state for.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flag is set.
     */
    int cheat_set_constant(Cheat* prCheat);

    /** Sets the specified Cheat to an inactive Cheat.  An Inactive Cheat is a 
     * cheat that is not copied to memory at any time.  If it has already been 
     * applied to memory, an Inactive Cheat is removed from memory.
     * 
     * @param prCheat Pointer to the Cheat Struct to set the flag state for.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flag is set.
     */
    int cheat_set_inactive(Cheat* prCheat);

    /** Assign the number of Blocks in the Block array belong to the specified 
     * Cheat.
     * 
     * @param prCheat Pointer to a Cheat struct to assign the Block index for.
     * @param length The number of Blocks in the Block array to assign.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the block length is assigned.
     */
    int cheat_set_length(Cheat* prCheat, unsigned int length);

    /** Sets the specified Cheat to a Selected Cheat.  A Selected Cheat is a 
     * cheat that activates when the Cheat Engine is activated, and is removed
     * from memory when the Cheat Engine is deactivated.
     * 
     * @param prCheat Pointer to the Cheat Struct to set the flag state for.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flag is set.
     */
    int cheat_set_selected(Cheat* prCheat);

    /** Set the CONSTANT flag for the specified Cheat.  The CONSTANT flag 
     * indicates whether a particular Cheat is considered active at all times, 
     * or active only when Cheats are toggled on in the Cheat Engine.
     * 
     * @param prCheat Pointer to the Cheat Struct to set the flag state for.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flag is set.
     */
    int cheat_setflag_constant(Cheat* prCheat);

    /** Set the FRESH flag for the specified Cheat.  The FRESH flag indicates 
     * whether a Cheat has been applied to active memory.
     * 
     * @param prCheat Pointer to the Cheat Struct to set the flag state for.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flag is set.
     */
    int cheat_setflag_fresh(Cheat* prCheat);

    /** Set the SELECTED flag for the specified Cheat.  The SELECTED flag 
     * indicates whether a particular Cheat is considered active when Cheats 
     * are toggled on in the Cheat Engine.
     * 
     * @param prCheat Pointer to the Cheat Struct to set the flag state for.
     * @return CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
     * CHEAT_SUCCESS is returned if the flag is set.
     */
    int cheat_setflag_selected(Cheat* prCheat);

#ifdef	__cplusplus
}
#endif

#endif
