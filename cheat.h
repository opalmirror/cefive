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

//Cheat flags
//If selected, will be disabled/enabled by music button
#define CHEAT_FLAG_SELECTED (1<<0)
//If 1, cheat is constantly on regardless of music button
#define CHEAT_FLAG_CONSTANT	(1<<1)
//Cheat was just recently enabled/disabled
#define CHEAT_FLAG_FRESH    (1<<2)

/* Indicates Success of a function. */
#define CHEAT_SUCCESS       (0)
/* Indicates Failure of a function. */
#define CHEAT_FAILURE       (-1)
/* Indicates a Memory allocation fault in a function. */
#define CHEAT_MEMORY        (-2)

/* The Cheat struct is used to hold a single Cheat in the Cheat Engine.  The
 * structure contains members for the block index, number of blocks, flags and
 * the name of the Cheat.  The structure maintains compatibility with the
 * previous NitePr struct by the same name.
 */
typedef struct _Cheat{
    unsigned short block;
    unsigned short len;
    unsigned char flags;
    unsigned char name[32];
}Cheat;

/* Clear all of the flags in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clear_flags(Cheat* prCheat);

/* Clear the CONSTANT cheat flag in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clearflag_constant(Cheat* prCheat);

/* Clear the FRESH cheat flag in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clearflag_fresh(Cheat* prCheat);

/* Clear the SELECTED cheat flag in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clearflag_selected(Cheat* prCheat);

/* Return the index of the first Block that belongs to the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to return the
 *              block for.
 * 
 * Returns:
 *   An unsigned integer containing the index within the Block array of the
 *   first Block belonging to the Cheat.
 */
unsigned int cheat_get_block(Cheat* prCheat);

/* Return the number of Block structs in the Block array that belong to the
 * specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to return the
 *              number of Blocks for.
 * 
 * Returns:
 *   An unsigned integer containing the number of Blocks in the Block array
 *   that belong to the Cheat.
 */
unsigned int cheat_get_length(Cheat *prCheat);

/* Return an integer indicating whether the CONSTANT flag of the specified
 * Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flag is set, 0 is returned if the flag is clear.
 */
int cheat_is_constant(Cheat* prCheat);

/* Return an integer indicating whether the FRESH flag of the specified
 * Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flag is set, 0 is returned if the flag is clear.
 */
int cheat_is_fresh(Cheat* prCheat);

/* Return an integer indicating whether the CONSTANT flag and the SELECTED flag
 * of the specified Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flags are set, 0 is returned if the flags are clear.
 */
int cheat_is_inactive(Cheat* prCheat);

/* Return an integer indicating whether the SELECTED flag of the specified
 * Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flag is set, 0 is returned if the flag is clear.
 */
int cheat_is_selected(Cheat* prCheat);

/* Assign the index of the first Block in the Block array that belongs to the
 * specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to assign the Block index for.
 *   block      unsigned int containing the index to assign.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the block index is assigned.
 */
int cheat_set_block(Cheat* prCheat, unsigned int block);

/* Sets the specified Cheat to a Constant Cheat.  A Constant Cheat is a cheat
 * that activates when the Cheat Engine is activated, but remains active when
 * the Cheat Engine is deactivated.
 * 
 * Parameters:
 *   prCheat    Pointer to the Cheat Struct to set the flag state for.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flag is set.
 */
int cheat_set_constant(Cheat* prCheat);

/* Sets the specified Cheat to an inactive Cheat.  An Inactive Cheat is a cheat
 * that is not copied to memory at any time.  If it has already been applied to
 * memory, an Inactive Cheat is removed from memory.
 * 
 * Parameters:
 *   prCheat    Pointer to the Cheat Struct to set the flag state for.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flag is set.
 */
int cheat_set_inactive(Cheat* prCheat);

/* Assign the number of Blocks in the Block array belong to the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to assign the Block index for.
 *   length     The number of Blocks in the Block array to assign.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the block length is assigned.
 */
int cheat_set_length(Cheat* prCheat, unsigned int length);

/* Sets the specified Cheat to a Selected Cheat.  A Selected Cheat is a cheat
 * that activates when the Cheat Engine is activated, and is removed from 
 * memory when the Cheat Engine is deactivated.
 * 
 * Parameters:
 *   prCheat    Pointer to the Cheat Struct to set the flag state for.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flag is set.
 */
int cheat_set_selected(Cheat* prCheat);

/* Set the CONSTANT flag for the specified Cheat.  The CONSTANT flag indicates
 * whether a particular Cheat is considered active at all times, or active
 * only when Cheats are toggled on in the Cheat Engine.
 * 
 * Parameters:
 *   prCheat    Pointer to the Cheat Struct to set the flag state for.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flag is set.
 */
int cheat_setflag_constant(Cheat* prCheat);

/* Set the FRESH flag for the specified Cheat.  The FRESH flag indicates whether
 * a Cheat has been applied to active memory.
 * 
 * Parameters:
 *   prCheat    Pointer to the Cheat Struct to set the flag state for.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flag is set.
 */
int cheat_setflag_fresh(Cheat* prCheat);

/* Set the SELECTED flag for the specified Cheat.  The SELECTED flag indicates
 * whether a particular Cheat is considered active when Cheats are toggled on 
 * in the Cheat Engine.
 * 
 * Parameters:
 *   prCheat    Pointer to the Cheat Struct to set the flag state for.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flag is set.
 */
int cheat_setflag_selected(Cheat* prCheat);

#ifdef	__cplusplus
}
#endif

#endif
