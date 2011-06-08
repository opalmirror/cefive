/* 
 * File:   asciicolumn.h
 * Author: Sir Gee of Five
 *
 * Created on September 27, 2010, 10:30 AM
 */

#ifndef _ASCIICOLUMN_H
#define	_ASCIICOLUMN_H

#include <string.h>
#include <pspdebug.h>
#include <pspkerneltypes.h>
#include "colorconfig.h"

#define ASCIICOLUMN_SUCCESS     (0)
#define ASCIICOLUMN_FAILURE     (-1)
#define ASCIICOLUMN_MEMORY      (-2)

#define ASCIICOLUMN_COLOR_BG    ((u32)0xFF000000)
#define ASCIICOLUMN_COLOR_FG    ((u32)0xFFFFFFFF)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _AsciiColumn {
        ColorConfig color;
        ColorConfig edit;
        char value;
        ColorConfig* prColor;
        int in_edit;
        int dirty;
    } AsciiColumn;

    char asciicolumn_commit(AsciiColumn* prCol);
    int asciicolumn_crossbutton(AsciiColumn* prCol);
    int asciicolumn_cursor_down(AsciiColumn* prCol);
    int asciicolumn_cursor_up(AsciiColumn* prCol);
    int asciicolumn_edit(AsciiColumn* prCol, char value);
    int asciicolumn_init(AsciiColumn* prCol);
    int asciicolumn_redraw(AsciiColumn* prCol);
    int asciicolumn_setvalue(AsciiColumn* prCol, char value);

#ifdef	__cplusplus
}
#endif

#endif	/* _ASCIICOLUMN_H */

