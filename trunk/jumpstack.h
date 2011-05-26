/* 
 * File:   jumpstack.h
 * Author: Sir Gee of Five
 *
 * Created on May 26, 2011, 10:06 AM
 */

#ifndef JUMPSTACK_H
#define	JUMPSTACK_H

#include <stdio.h>
#include <pspkerneltypes.h>

/** Indicates success. */
#define JUMPSTACK_SUCCESS (0)

/** Indicates failure. */
#define JUMPSTACK_FAILURE (-1)

/** Indicates a NULL pointer. */
#define JUMPSTACK_NULLPTR (-2)

/** Indicates a memory error. */
#define JUMPSTACK_MEMORY (-3)

/** The number of elements in a Jump Stack. */
#define JUMPSTACK_SIZE (16)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _JumpStack {
        /** Array of SceUInt32 addresses. */
        SceUInt32 element[JUMPSTACK_SIZE];
        /** The number of active elements. */
        int elements;
    }
    /** The JumpStack struct represents a Jump Stack. */
    JumpStack;

    /** Clear a Jump Stack.
     * 
     * @param prStack Pointer to a JumpStack struct representing the Jump Stack.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int jumpstack_clear(JumpStack* prStack);
    
    /** Initialize a Jump Stack.
     * 
     * @param prStack Pointer to the JumpStack struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int jumpstack_init(JumpStack* prStack);

    /** Return the last added address from a Jump Stack, removing the address.
     * 
     * @param prStack Pointer to a JumpStack struct representing the Jump Stack.
     * @return A SceUInt32 containing the last added address is returned.
     */
    SceUInt32 jumpstack_pop(JumpStack* prStack);
    
    /** Push an address onto the Jump Stack, rolling off the last element if
     * necessary.
     * 
     * @param prStack Pointer to a JumpStack struct representing the Jump Stack.
     * @param address SceUInt32 containing the address to push.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int jumpstack_push(JumpStack* prStack, SceUInt32 address);

#ifdef	__cplusplus
}
#endif

#endif	/* JUMPSTACK_H */
