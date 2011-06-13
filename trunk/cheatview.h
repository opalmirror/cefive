/* 
 * File:   cheatview.h
 * Author: Sir Gee of Five
 *
 * Created on June 9, 2011, 10:34 AM
 */

#ifndef CHEATVIEW_H
#define	CHEATVIEW_H

#include <stdio.h>
#include <string.h>
#include <pspdebug.h>
#include <psptypes.h>
#include "cheat.h"
#include "cheatengine.h"
#include "cursorpos.h"
#include "dimension.h"
#include "panelconfig.h"

#define CHEATVIEW_SUCCESS (0)
#define CHEATVIEW_FAILURE (-1)
#define CHEATVIEW_NULLPTR (-2)
#define CHEATVIEW_MEMORY (-3)
#define CHEATVIEW_INVIDX (-4)
#define CHEATVIEW_INVENG (-5)

#define CHEATVIEW_POS_X (0)
#define CHEATVIEW_POS_Y (1)
#define CHEATVIEW_SIZE_W (69)
#define CHEATVIEW_SIZE_H (30)
#define CHEATVIEW_PANEL_BG ((u32)0xFFD0D0D0)
#define CHEATVIEW_PANEL_FG ((u32)0xFF000000)
#define CHEATVIEW_CURSOR_BG ((u32)0xFFD00000)
#define CHEATVIEW_CURSOR_FG ((u32)0xFFFFFFFF)
#define CHEATVIEW_ACTIVE ((u32)0xFF20D020)
#define CHEATVIEW_CONSTANT ((u32)0xFF2020D0)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CheatView {
        PanelConfig panelConfig;
        CursorPos cursorPos;
        CheatEngine* cheatEngine;
        u32 activeColor;
        u32 constantColor;
        int pageIndex;
        int dirty;
    }
    CheatView;

    int cheatview_button_circle(CheatView* prView);
    
    int cheatview_button_cross(CheatView* prView);
    
    int cheatview_button_ltrigger(CheatView* prView);
    
    int cheatview_button_rtrigger(CheatView* prView);
    
    int cheatview_button_square(CheatView* prView);
    
    int cheatview_button_triangle(CheatView* prView);
    
    int cheatview_dpad_down(CheatView* prView);

    int cheatview_dpad_left(CheatView* prView);

    int cheatview_dpad_right(CheatView* prView);
    
    int cheatview_dpad_up(CheatView* prView);
    
    int cheatview_init(CheatView* prView, CheatEngine* prEngine);
    
    int cheatview_page_down(CheatView* prView);
    
    int cheatview_page_up(CheatView* prView);
    
    int cheatview_redraw(CheatView* prView);

    int cheatview_set_activecolor(CheatView* prView, const u32 color);
    
    int cheatview_set_constantcolor(CheatView* prView, const u32 color);
    
    int cheatview_set_cursorcolor(CheatView* prView, const u32 background,
        const u32 text);

    int cheatview_set_panelcolor(CheatView* prView, const u32 background,
        const u32 text);
    
    int cheatview_set_position(CheatView* prView, const int x, const int y);
    
    int cheatview_set_size(CheatView* prView, const int width, 
            const int height);
    
#ifdef	__cplusplus
}
#endif

#endif	/* CHEATVIEW_H */

