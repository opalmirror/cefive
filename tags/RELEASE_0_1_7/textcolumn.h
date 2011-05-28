/* 
 * File:   textcolumn.h
 * Author: Sir Gee of Five
 *
 * Created on September 26, 2010, 11:45 AM
 */

#ifndef _TEXTCOLUMN_H
#define	_TEXTCOLUMN_H

#include <pspkerneltypes.h>
#include "colorconfig.h"

/** Indicates success */
#define TEXTCOLUMN_SUCCESS      (0)

/** Indicates failure. */
#define TEXTCOLUMN_FAILURE      (-1)

/** Indicates a NULL pointer. */
#define TEXTCOLUMN_NULLPTR      (-2)

/** Indicates a memory error. */
#define TEXTCOLUMN_MEMORY       (-3)

/** Maximum number of columns */
#define TEXTCOLUMN_MAX_LEN      80

/** Default Display Background Color */
#define TEXTCOLUMN_COLOR_BG     ((u32)0xFF000000)

/** Default Display Foreground Color */
#define TEXTCOLUMN_COLOR_FG     ((u32)0xFFFFFFFF)

/** Default Cursor Background Color */
#define TEXTCOLUMN_CURSOR_BG    ((u32)0xFFFFFFFF)

/** Default Cursor Foreground Color */
#define TEXTCOLUMN_CURSOR_FG    ((u32)0xFF000000)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _TextColumnJustification {
        TCJ_Left, TCJ_Right
    }
    TextColumnJustification;

    typedef struct _TextColumn {
        /** The text to display. */
        char text[TEXTCOLUMN_MAX_LEN + 1];
        /** Normal Color Configuration */
        ColorConfig rColor;
        ColorConfig* prColor;
        /** Cursor Color Configuration */
        ColorConfig rCursor;
        ColorConfig* prCursor;
        /** The number of visible columns. */
        int columns;
        /** The Justification of the column. */
        TextColumnJustification justification;
        /** Whether the column is a cursor column. */
        int is_cursor;
        /** Whether the column needs to be redrawn. */
        int dirty;
    }
    /** The TextColumn struct is used to represent a Text Column.
     * A Text Column is used to display a column of text on the screen.
     */
    TextColumn;

    /** Return a pointer to the ColorConfig struct representing the Panel
     * Color Configuration for a Text Column.
     * 
     * @param prCol Pointer to a TextColumn struct representing the Text Column.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* textcolumn_get_displaycolor(TextColumn* prCol);
    
    /** Return the printf format string for rendering the column.
     * 
     * @param prCol Pointer to a TextColumn struct representing the Text Column.
     * @param fmt Pointer to char memory to hold the format string.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int textcolumn_getformat(TextColumn* prCol, char* fmt);
    
    /** Initialize a TextColumn struct to represent a Text Column.
     * 
     * @param prCol Pointer to the TextColumn struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int textcolumn_init(TextColumn* prCol);
    
    /** Indicate that a Text Column needs to be redrawn.
     * 
     * @param prCol Pointer to a TextColumn struct representing the Text Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int textcolumn_invalidate(TextColumn* prCol);
    
    /** Redraw a Text Column on the screen at the current position.
     * 
     * @param prCol Pointer to a TextColumn struct representing the Text Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int textcolumn_redraw(TextColumn* prCol);
    
    /** Assign the number of visible columns in a Text Column.
     * 
     * @param prCol Pointer to a TextColumn struct representing the Text Column.
     * @param columns The number of columns to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int textcolumn_set_columns(TextColumn* prCol, int columns);
    
    /** Assign the text to display in a Text Column.
     * 
     * @param prCol Pointer to a TextColumn struct representing the Text Column.
     * @param text String containing the text to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int textcolumn_settext(TextColumn* prCol, const char* text);

#ifdef	__cplusplus
}
#endif

#endif	/* _TEXTCOLUMN_H */

