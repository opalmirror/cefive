/* 
 * File:   ceditor.h
 * Author: Sir Gee of Five
 *
 * Created on June 5, 2011, 1:38 PM
 */

#ifndef CEDITOR_H
#define	CEDITOR_H

#include <stdio.h>
#include <pspdebug.h>
#include "blockview.h"
#include "cheat.h"
#include "cheatengine.h"
#include "cursorpos.h"
#include "dimension.h"
#include "panelconfig.h"

#define CEDITOR_SUCCESS (0)
#define CEDITOR_FAILURE (-1)
#define CEDITOR_NULLPTR (-2)
#define CEDITOR_MEMORY (-3)
#define CEDITOR_INVIDX (-4)
#define CEDITOR_INVENG (-5)
#define CEDITOR_INVCHT (-6)

#define CEDITOR_POS_X (0)
#define CEDITOR_POS_Y (1)
#define CEDITOR_SIZE_W (69)
#define CEDITOR_SIZE_H (30)
#define CEDITOR_PANEL_BG ((u32)0xFFD0D0D0)
#define CEDITOR_PANEL_FG ((u32)0xFF000000)
#define CEDITOR_CURSOR_BG ((u32)0xFFD00000)
#define CEDITOR_CURSOR_FG ((u32)0xFFFFFFFF)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CEditor {
        PanelConfig panelConfig;
        BlockView blockView;
        CheatEngine* cheatEngine;
        int index;
        int dirty;
    }
    CEditor;

    int ceditor_button_circle(CEditor* prEd);
    
    int ceditor_button_cross(CEditor* prEd);
    
    int ceditor_button_ltrigger(CEditor* prEd);
    
    int ceditor_button_rtrigger(CEditor* prEd);
    
    int ceditor_button_square(CEditor* prEd);
    
    int ceditor_button_triangle(CEditor* prEd);
    
    int ceditor_dpad_down(CEditor* prEd);
    
    int ceditor_dpad_left(CEditor* prEd);
    
    int ceditor_dpad_right(CEditor* prEd);
    
    int ceditor_dpad_up(CEditor* prEd);
    
    int ceditor_init(CEditor* prEd, CheatEngine* prEngine);
    
    int ceditor_is_editing(CEditor* prEd);
    
    int ceditor_page_down(CEditor* prEd);
    
    int ceditor_page_up(CEditor* prEd);
    
    int ceditor_redraw(CEditor* prEd);
    
    int ceditor_set_cursorcolor(CEditor* prEd, const u32 background, 
            const u32 text);
    
    int ceditor_set_index(CEditor* prEd, const int index);
    
    int ceditor_set_panelcolor(CEditor* prEd, const u32 background, 
            const u32 text);

    int ceditor_set_position(CEditor* prEd, const int x, const int y);
    
    int ceditor_set_size(CEditor* prEd, const int width, const int height);

#ifdef	__cplusplus
}
#endif

#endif	/* CEDITOR_H */

