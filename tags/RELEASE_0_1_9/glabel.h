/* 
 * File:   glabel.h
 * Author: Sir Gee of Five
 *
 * Created on May 29, 2011, 6:29 PM
 */

#ifndef GLABEL_H
#define	GLABEL_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>

/** Indicates success. */
#define GLABEL_SUCCESS (0)

/** Indicates failure. */
#define GLABEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define GLABEL_NULLPTR (-2)

/** Indicates a memory error. */
#define GLABEL_MEMORY (-3)

/** The maximum length of text in a Glabel. */
#define GLABEL_TEXT_LEN (63)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Glabel {
        /** The address of the Label. */
        SceUInt32 address;
        /** The text of the Label. */
        char text[GLABEL_TEXT_LEN + 1];
    }
    /** A Glabel struct represents a memory label. */
    Glabel;

    /** Copy a Glabel.
     * 
     * @param prDest Pointer to a Glabel struct representing the destination.
     * @param prSrc Pointer to a Glabel struct representing the source.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int glabel_copy(Glabel* prDest, Glabel* prSrc);
    
    /** Initialize a Glabel.
     * 
     * @param prLabel Pointer to a Glabel struct representing the Label.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int glabel_init(Glabel* prLabel);

    /** Assign the values of a Label.
     * 
     * @param prLabel Pointer to a Glabel struct representing the Label.
     * @param address SceUInt32 containing the address of the Label.
     * @param text string containing the text of the Label.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int glabel_set(Glabel* prLabel, const SceUInt32 address, const char* text);

#ifdef	__cplusplus
}
#endif

#endif	/* GLABEL_H */

