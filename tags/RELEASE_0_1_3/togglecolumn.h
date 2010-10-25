/* 
 * File:   togglecolumn.h
 * Author: Sir Gee of Five
 *
 * Created on September 26, 2010, 1:56 PM
 */

#ifndef _TOGGLECOLUMN_H
#define	_TOGGLECOLUMN_H

#include <pspkerneltypes.h>
#include "colorconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

    #define TOGGLECOLUMN_SUCCESS    (0)
    #define TOGGLECOLUMN_FAILURE    (-1)
    #define TOGGLECOLUMN_MEMORY     (-2)

    #define TOGGLECOLUMN_POSITIVE   "Yes"
    #define TOGGLECOLUMN_NEGATIVE   "No"
    #define TOGGLECOLUMN_COLOR_BG   ((u32)0xFF000000)
    #define TOGGLECOLUMN_COLOR_FG   ((u32)0xFFFFFFFF)
    #define TOGGLECOLUMN_CURSOR_BG  ((u32)0xFFFFFFFF)
    #define TOGGLECOLUMN_CURSOR_FG  ((u32)0xFF000000)
    #define TOGGLECOLUMN_MAX_LEN    80

    typedef enum _ToggleColumnJustify {
        ToCJ_Left, ToCJ_Right
    }ToggleColumnJustify;

    typedef struct _ToggleColumn {
        ColorConfig* prColor;
        ColorConfig* prCursor;
        int value;
        int is_cursor;
        int columns;
        char* positive_label;
        char* negative_label;
        ToggleColumnJustify justify;
    }ToggleColumn;

    int togglecolumn_format(ToggleColumn* prCol, char* fmt);
    int togglecolumn_init(ToggleColumn* prCol);
    int togglecolumn_redraw(ToggleColumn* prCol);
    int togglecolumn_toggle(ToggleColumn* prCol);

#ifdef	__cplusplus
}
#endif

#endif	/* _TOGGLECOLUMN_H */

