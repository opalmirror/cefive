/* 
 * File:   glabelmap.h
 * Author: Sir Gee of Five
 *
 * Created on May 30, 2011, 10:06 AM
 */

#ifndef GLABELMAP_H
#define	GLABELMAP_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include "glabel.h"

/** Indicates success. */
#define GLABELMAP_SUCCESS (0)

/** Indicates failure. */
#define GLABELMAP_FAILURE (-1)

/** Indicates a NULL pointer. */
#define GLABELMAP_NULLPTR (-2)

/** Indicates a memory error. */
#define GLABELMAP_MEMORY (-3)

/** Indicates an invalid index. */
#define GLABELMAP_INVIDX (-4)

/** The maximum number of Glabel elements in a GlabelMap. */
#define GLABELMAP_SIZE (512)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GlabelMap {
        /** Array of Glabel elements. */
        Glabel label[GLABELMAP_SIZE];
        /** The number of elements in the GlabelMap. */
        int count;
    }
    /** The GlabelMap struct is used to represent a Label Map. */
    GlabelMap;

    /** Add a Label to a Label Map returning a pointer to the newly added
     * Glabel struct.
     * 
     * @param prMap Pointer to a GlabelMap struct representing a Label Map.
     * @return A pointer to a Glabel struct or NULL is returned.
     */
    Glabel* glabelmap_add(GlabelMap* prMap);
    
    /** Clear a Label Map and reset the count to 0.
     * 
     * @param prMap Pointer to a GlabelMap struct representing a Label Map.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int glabelmap_clear(GlabelMap* prMap);
    
    /** Return a Label for the indicated address if applicable.
     * 
     * @param prMap Pointer to a GlabelMap struct representing a Label Map.
     * @param address SceUInt32 indicating the address to return a Label for.
     * @return A pointer to a Glabel struct or NULL is returned.
     */
    Glabel* glabelmap_find(GlabelMap* prMap, const SceUInt32 address);
    
    /** Return the indicated Label from a Label Map.
     * 
     * @param prMap Pointer to a GlabelMap struct representing a Label Map.
     * @param index int indicating the index of the Label to return.
     * @return A pointer to a Glabel struct or NULL is returned.
     */
    Glabel* glabelmap_get(GlabelMap* prMap, const int index);
    
    /** Initialize a Label Map.
     * 
     * @param prMap Pointer to the GlabelMap struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int glabelmap_init(GlabelMap* prMap);

#ifdef	__cplusplus
}
#endif

#endif	/* GLABELMAP_H */

