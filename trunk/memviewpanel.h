/* 
 * File:   memviewpanel.h
 * Author: Sir Gee of Five
 *
 * Created on May 24, 2011, 10:41 AM
 */

#ifndef MEMVIEWPANEL_H
#define	MEMVIEWPANEL_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "panelconfig.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "mips.h"

#define MEMVIEWPANEL_SUCCESS (0)
#define MEMVIEWPANEL_FAILURE (-1)
#define MEMVIEWPANEL_NULLPTR (-2)
#define MEMVIEWPANEL_MEMORY (-3)
#define MEMVIEWPANEL_INVIDX (-4)
#define MEMVIEWPANEL_BADADDR (-5)

#define MEMVIEWPANEL_MINOFF ((SceUInt32)0x08800000)
#define MEMVIEWPANEL_MAXOFF ((SceUInt32)0x0A000000)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _MemViewPanel {
        PanelConfig panelConfig;
        CursorPos cursorPos;
        SceUInt32 offset;
        SceUInt32 minOffset;
        SceUInt32 maxOffset;
        int dirty;
    }
    MemViewPanel;

    int memviewpanel_cursor_down(MemViewPanel* prPanel);
    
    int memviewpanel_cursor_left(MemViewPanel* prPanel);
    
    int memviewpanel_cursor_right(MemViewPanel* prPanel);
    
    int memviewpanel_cursor_up(MemViewPanel* prPanel);
    
    PanelConfig* memviewpanel_get_config(MemViewPanel* prPanel);
    
    ColorConfig* memviewpanel_get_cursorcolor(MemViewPanel* prPanel);
    
    CursorPos* memviewpanel_get_cursorpos(MemViewPanel* prPanel);
    
    ColorConfig* memviewpanel_get_panelcolor(MemViewPanel* prPanel);
    
    CursorPos* memviewpanel_get_position(MemViewPanel* prPanel);
    
    Dimension* memviewpanel_get_size(MemViewPanel* prPanel);
    
    int memviewpanel_init(MemViewPanel* prPanel);
    
    int memviewpanel_invalidate(MemViewPanel* prPanel);
    
    int memviewpanel_redraw(MemViewPanel* prPanel);

    int memviewpanel_scroll_down(MemViewPanel* prPanel, const int rows);
    
    int memviewpanel_scroll_up(MemViewPanel* prPanel, const int rows);
    
    int memviewpanel_seek(MemViewPanel* prPanel, SceUInt32 address);
    
    int memviewpanel_set_panelcolor(MemViewPanel* prPanel, 
            const u32 background, const u32 text);
    
    int memviewpanel_set_position(MemViewPanel* prPanel, const int x, 
            const int y);
    
    int memviewpanel_set_size(MemViewPanel* prPanel, const int width, 
        const int height);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MEMVIEWPANEL_H */

