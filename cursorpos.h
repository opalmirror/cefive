/* cursorpos
 *  CursorPos, cursor positioning for Cheat Engine of Five.
 * Author: Sir Gee of Five
 */

#ifndef CURSORPOS_H
#define CURSORPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
    
/** Indicates success. */
#define CURSORPOS_SUCCESS       (0)

/** Indicates failure. */
#define CURSORPOS_FAILURE       (-1)

/** Indicates a memory error. */
#define CURSORPOS_MEMORY        (-2)

/** Indicates a NULL pointer. */
#define CURSORPOS_NULLPTR       (-3)

/** Indicates an I/O error. */
#define CURSORPOS_IOERROR       (-4)
    
#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CursorPos {
        /** The horizontal position or column. */
        int x;
        /** The vertical position or row. */
        int y;
    }
    /** The CursorPos struct is used to represent a Cursor Position. */
    CursorPos;

    /** Copy a CursorPos struct.
     * 
     * @param prDest pointer to the Destination CursorPos struct.
     * @param prSrc pointer to the Source CursorPos struct.
     * @return 0 on success, negative value on failure.
     */
    int cursorpos_copy(CursorPos* prDest, CursorPos* prSrc);
    
    /** Initialize a CursorPos struct.
     * 
     * @param prPos pointer to the CursorPos struct to initialize.
     * @return 0 on success, negative value on failure.
     */
    int cursorpos_init(CursorPos* prPos);
    
    /** Read a CursorPos struct from an open file descriptor.
     * 
     * @param prPos pointer to the CursorPos struct to read into.
     * @param fd SceUID open file descriptor
     * @return 0 on success, negative value on failure.
     */
    int cursorpos_read(CursorPos* prPos, SceUID fd);
    
    /** Assign values to a CursorPos struct.
     * 
     * @param prPos pointer to the CursorPos struct to assign.
     * @param x int value of the horizontal cursor position.
     * @param y int value of the vertical cursor position.
     * @return 0 on success, negative value on failure.
     */
    int cursorpos_set(CursorPos* prPos, int x, int y);
    
    /** Write a CursorPos to an open file descriptor.
     * 
     * @param prPos pointer to the CursorPos struct to write.
     * @param fd SceUID open file descriptor
     * @return 0 on success, negative value on failure.
     */
    int cursorpos_write(CursorPos* prPos, SceUID fd);
    
#ifdef	__cplusplus
}
#endif

#endif /* CURSORPOS_H */
