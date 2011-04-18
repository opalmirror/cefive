/* cheat.c
 *   Adapted from crt0_prx.c
 * Author:
 *   Sir Gee of Five
 */

#include <stdio.h>
#include "cheat.h"


/* Clear all of the flags in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clear_flags(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_constant(prCheat);
    cheat_clearflag_fresh(prCheat);
    cheat_clearflag_selected(prCheat);
    return CHEAT_SUCCESS;
}


/* Clear the CONSTANT cheat flag in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clearflag_constant(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags &= ~CHEAT_FLAG_CONSTANT;
    return CHEAT_SUCCESS;
}


/* Clear the FRESH cheat flag in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clearflag_fresh(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags &= ~CHEAT_FLAG_FRESH;
    return CHEAT_SUCCESS;
}


/* Clear the SELECTED cheat flag in the specified Cheat.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct containing the Cheat to clear.
 * 
 * Returns:
 *   CHEAT_MEMORY is returned if the specified Cheat pointer is NULL.
 *   CHEAT_SUCCESS is returned if the flags are cleared.
 */
int cheat_clearflag_selected(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags &= ~CHEAT_FLAG_SELECTED;
    return CHEAT_SUCCESS;
}


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
unsigned int cheat_get_block(Cheat* prCheat) {
    unsigned int block = 0;
    if (prCheat != NULL) {
        block = (unsigned int)prCheat->block;
    }
    return block;
}


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
unsigned int cheat_get_length(Cheat *prCheat) {
    unsigned int clen = 0;
    if (prCheat != NULL) {
        clen = (unsigned int)prCheat->len;
    }
    return clen;
}


/* Return an integer indicating whether the CONSTANT flag of the specified
 * Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flag is set, 0 is returned if the flag is clear.
 */
int cheat_is_constant(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        is = (prCheat->flags & CHEAT_FLAG_CONSTANT) ? 1 : 0;
    }
    return is;
}


/* Return an integer indicating whether the FRESH flag of the specified
 * Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flag is set, 0 is returned if the flag is clear.
 */
int cheat_is_fresh(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        is = (prCheat->flags & CHEAT_FLAG_FRESH) ? 1 : 0;
    }
    return is;
}


/* Return an integer indicating whether the CONSTANT flag and the SELECTED flag
 * of the specified Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flags are set, 0 is returned if the flags are clear.
 */
int cheat_is_inactive(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        if (cheat_is_constant(prCheat) == 0 &&
                cheat_is_selected(prCheat) == 0) {
            is = 1;
        }
    }
    return is;
}


/* Return an integer indicating whether the SELECTED flag of the specified
 * Cheat is set.
 * 
 * Parameters:
 *   prCheat    Pointer to a Cheat struct to return flag indications for.
 * 
 * Returns:
 *  1 is returned if the flag is set, 0 is returned if the flag is clear.
 */
int cheat_is_selected(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        is = (prCheat->flags & CHEAT_FLAG_SELECTED) ? 1 : 0;
    }
    return is;
}


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
int cheat_set_block(Cheat* prCheat, unsigned int block) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->block = (unsigned short)(block & 0xFFFF);
    return CHEAT_SUCCESS;
}


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
int cheat_set_length(Cheat* prCheat, unsigned int length) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->len = (unsigned short)(length & 0xFFFF);
    return CHEAT_SUCCESS;
}


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
int cheat_setflag_constant(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags |= CHEAT_FLAG_CONSTANT;
    return CHEAT_SUCCESS;
}


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
int cheat_setflag_fresh(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags |= CHEAT_FLAG_FRESH;
    return CHEAT_SUCCESS;
}


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
int cheat_setflag_selected(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags |= CHEAT_FLAG_SELECTED;
    return CHEAT_SUCCESS;
}


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
int cheat_set_constant(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_selected(prCheat);
    cheat_setflag_constant(prCheat);
    cheat_clearflag_fresh(prCheat);
    return CHEAT_SUCCESS;
}


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
int cheat_set_inactive(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_constant(prCheat);
    cheat_clearflag_selected(prCheat);
    cheat_clearflag_fresh(prCheat);
    return CHEAT_SUCCESS;
}


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
int cheat_set_selected(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_constant(prCheat);
    cheat_setflag_selected(prCheat);
    cheat_clearflag_fresh(prCheat);
    return CHEAT_SUCCESS;
}
