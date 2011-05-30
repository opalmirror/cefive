/* 
 * File:   dimension.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 10:33 AM
 */

#ifndef DIMENSION_H
#define	DIMENSION_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
    
/** Indicates success. */
#define DIMENSION_SUCCESS       (0)

/** Indicates failure. */
#define DIMENSION_FAILURE       (-1)

/** Indicates a memory error. */
#define DIMENSION_MEMORY        (-2)

/** Indicates a NULL pointer. */
#define DIMENSION_NULLPTR       (-3)

/** Indicates an I/O error. */
#define DIMENSION_IOERROR       (-4)
    
#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Dimension {
        /** The width in columns. */
        int width;
        /** The height in rows. */
        int height;
    }
    /** The Dimension struct is used to represent a Screen Dimension. */
    Dimension;

    /** Copy a Dimension struct.
     * 
     * @param prDest Pointer to a Dimension struct representing the destination.
     * @param prSrc Pointer to a Dimension struct representing the source.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int dimension_copy(Dimension* prDest, Dimension* prSrc);
    
    /** Initialize a Dimension.
     * 
     * @param prDim Pointer to the Dimension struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int dimension_init(Dimension* prDim);

    /** Read a Dimension struct from the specified file descriptor.
     * 
     * @param prDim Pointer to a Dimension struct representing the Dimension.
     * @param fd SceUID representing the open file descriptor.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int dimension_read(Dimension* prDim, SceUID fd);

    /** Assign the specified values to a Dimension.
     * 
     * @param prDim Pointer to a Dimension struct representing the Dimension.
     * @param width int containing the width in columns.
     * @param height int containing the height in rows.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int dimension_set(Dimension* prDim, int width, int height);

    /** Write a Dimension struct to the specified file descriptor.
     * 
     * @param prDim Pointer to a Dimension struct representing the Dimension.
     * @param fd SceUID representing the open file descriptor.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int dimension_write(Dimension* prDim, SceUID fd);
    
#ifdef	__cplusplus
}
#endif

#endif	/* DIMENSION_H */
