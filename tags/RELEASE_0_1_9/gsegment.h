/* 
 * File:   gsegment.h
 * Author: Sir Gee of Five
 *
 * Created on May 29, 2011, 10:23 AM
 */

#ifndef GSEGMENT_H
#define	GSEGMENT_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>

/** Indicates success. */
#define GSEGMENT_SUCCESS (0)
/** Indicates failure. */
#define GSEGMENT_FAILURE (-1)
/** Indicates a NULL pointer. */
#define GSEGMENT_NULLPTR (-2)

/** The maximum length of a Segment name. */
#define GSEGMENT_NAME_LEN (63)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ESegmentType {
        SET_None = 0,
        SET_Text,
        SET_Data,
        SET_RoData,
        SET_ModInfo
    }
    ESegmentType;

    typedef struct _Gsegment {
        /** The type of the segment. */
        ESegmentType segmentType;
        /** The first address in the segment. */
        SceUInt32 startaddr;
        /** The first address outside of the segment. */
        SceUInt32 endaddr;
        /** The name of the segment. */
        char name[GSEGMENT_NAME_LEN + 1];
    }
    /** The Gsegment struct is used to represent a memory Segment. */
    Gsegment;

    /** Copy a Gsegment.
     * 
     * @param pDest Pointer to a Gsegment struct representing the destination.
     * @param pSrc Pointer to a Gsegment struct representing the source.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int gsegment_copy(Gsegment* pDest, Gsegment* pSrc);

    /** Initialize a Gsegment.
     * 
     * @param pSeg Pointer to the Gsegment struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int gsegment_init(Gsegment* pSeg);

    /** Assign the values of a Gsegment.
     * 
     * @param pSeg Pointer to a Gsegment struct representing the segment.
     * @param startaddr SceUInt32 containing the first address in the segment.
     * @param endaddr SceUInt32 containing the first address outside of the
     * segment.
     * @param segType ESegmentType indicating the type of segment.
     * @param name The name of the segment.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int gsegment_set(Gsegment* pSeg, const SceUInt32 startaddr,
            const SceUInt32 endaddr, const char* name, ESegmentType segType);

#ifdef	__cplusplus
}
#endif

#endif	/* GSEGMENT_H */
