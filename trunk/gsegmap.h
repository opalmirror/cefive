/* 
 * File:   gsegmap.h
 * Author: Sir Gee of Five
 *
 * Created on May 29, 2011, 10:36 AM
 */

#ifndef GSEGMAP_H
#define	GSEGMAP_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include "gsegment.h"

/** Indicates success. */
#define GSEGMAP_SUCCESS (0)

/** Indicates failure. */
#define GSEGMAP_FAILURE (-1)

/** Indicates a NULL pointer. */
#define GSEGMAP_NULLPTR (-2)

/** Indicates a memory error. */
#define GSEGMAP_MEMORY (-3)

/** Indicates an invalid index. */
#define GSEGMAP_INVIDX (-4)

/** The number of Gsegment elements in a GsegMap. */
#define GSEGMAP_SIZE (128)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GsegMap {
        /** Array of Gsegment elements */
        Gsegment segment[GSEGMAP_SIZE];
        /** The number of Gsegment elements */
        int count;
    }
    /** The GsegMap struct is used to represent a Segment Map. */
    GsegMap;
    
    /** Add a Segment to a Segment Map, returning a pointer to the newly added
     * Gsegment struct.
     * 
     * @param prMap Pointer to a GsegMap struct representing the Segment Map.
     * @return A pointer to a Gsegment struct or NULL is returned.
     */
    Gsegment* gsegmap_add(GsegMap* prMap);

    /** Clear a Segment Map.
     * 
     * @param prMap Pointer to a GsegMap struct representing the Segment Map.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int gsegmap_clear(GsegMap* prMap);
    
    /** Return the Segment from the Segment Map that the specified address
     * appears in.  If the specified address does not appear in any segment,
     * return NULL.
     * 
     * @param prMap Pointer to a GsegMap struct representing the Segment Map.
     * @param address SceUInt32 specifying the address to return a segment for.
     * @return A pointer to a Gsegment struct or NULL is returned.
     */
    Gsegment* gsegmap_find(GsegMap* prMap, const SceUInt32 address);
    
    /** Return the indicated Segment from the Segment Map.
     * 
     * @param prMap Pointer to a GsegMap struct representing the Segment Map.
     * @param index int indicating the index of the Segment to return.
     * @return A pointer to a Gsegment struct or NULL is returned.
     */
    Gsegment* gsegmap_get(GsegMap* prMap, const int index);
    
    /** Initialize a Segment Map.
     * 
     * @param prMap Pointer to the GsegMap struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int gsegmap_init(GsegMap* prMap);

#ifdef	__cplusplus
}
#endif

#endif	/* GSEGMAP_H */
