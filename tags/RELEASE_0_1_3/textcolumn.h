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

#ifdef	__cplusplus
extern "C" {
#endif

    #define TEXTCOLUMN_SUCCESS      (0)
    #define TEXTCOLUMN_FAILURE      (-1)
    #define TEXTCOLUMN_MEMORY       (-2)

    #define TEXTCOLUMN_MAX_LEN      80
    #define TEXTCOLUMN_COLOR_BG     ((u32)0xFF000000)
    #define TEXTCOLUMN_COLOR_FG     ((u32)0xFFFFFFFF)
    #define TEXTCOLUMN_CURSOR_BG    ((u32)0xFFFFFFFF)
    #define TEXTCOLUMN_CURSOR_FG    ((u32)0xFF000000)

    typedef enum _TextColumnJustification {
        TCJ_Left, TCJ_Right
    }TextColumnJustification;

    typedef struct _TextColumn {
        char text[TEXTCOLUMN_MAX_LEN + 1];
        ColorConfig* prColor;
        ColorConfig* prCursor;
        int columns;
        TextColumnJustification justification;
        int is_cursor;
    }TextColumn;

    int textcolumn_getformat(TextColumn* prCol, char* fmt);
    int textcolumn_init(TextColumn* prCol);
    int textcolumn_redraw(TextColumn* prCol);
    int textcolumn_settext(TextColumn* prCol, const char* text);

#ifdef	__cplusplus
}
#endif

#endif	/* _TEXTCOLUMN_H */

