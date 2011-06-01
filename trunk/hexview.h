/* 
 * File:   hexview.h
 * Author: Sir Gee of Five
 *
 * Created on June 1, 2011, 2:02 PM
 */

#ifndef HEXVIEW_H
#define	HEXVIEW_H

#include <stdio.h>
#include <string.h>
#include <psptypes.h>
#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "hexpad.h"
#include "panelconfig.h"

#define HEXVIEW_SUCCESS (0)
#define HEXVIEW_FAILURE (-1)
#define HEXVIEW_NULLPTR (-2)
#define HEXVIEW_MEMORY (-3)
#define HEXVIEW_INVIDX (-4)
#define HEXVIEW_BADADDR (-5)

#define HEXVIEW_MINOFFSET ((SceUInt32)0x08800000)
#define HEXVIEW_MAXOFFSET ((SceUInt32)0x0A000000)
#define HEXVIEW_COLUMNS (16)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _HexView {
        PanelConfig panelConfig;
        CursorPos cursorPos;
        HexPad hexPad;
        SceUInt32 offset;
        SceUInt32 minOffset;
        SceUInt32 maxOffset;
        int columns;
        int dirty;
        int editing;
    }
    HexView;

    int hexview_button_circle(HexView* prView);

    int hexview_button_cross(HexView* prView);
    
    int hexview_button_ltrigger(HexView* prView);
    
    int hexview_button_rtrigger(HexView* prView);
    
    int hexview_button_square(HexView* prView);
    
    int hexview_button_triangle(HexView* prView);
    
    int hexview_dpad_down(HexView* prView);
    
    int hexview_dpad_left(HexView* prView);
    
    int hexview_dpad_right(HexView* prView);
    
    int hexview_dpad_up(HexView* prView);
    
    int hexview_init(HexView* prView);

    int hexview_page_down(HexView* prView);
    
    int hexview_page_up(HexView* prView);
    
    int hexview_redraw(HexView* prView);
    
    int hexview_seek(HexView* prView, SceUInt32 address);
    
    int hexview_set_cursorcolor(HexView* prView, 
            const u32 background, const u32 text);
    
    int hexview_set_panelcolor(HexView* prView, 
            const u32 background, const u32 text);
    
    int hexview_set_cursorpos(HexView* prView, const int x, const int y);
    
    int hexview_set_position(HexView* prView, const int x, const int y);
    
    int hexview_set_size(HexView* prView, const int width, const int height);
    
#ifdef	__cplusplus
}
#endif

#endif	/* HEXVIEW_H */

