/* 
 * File:   blockview.h
 * Author: Sir Gee of Five
 *
 * Created on June 3, 2011, 10:16 AM
 */

#ifndef BLOCKVIEW_H
#define	BLOCKVIEW_H

#include <stdio.h>
#include <pspdebug.h>
#include <psptypes.h>
#include "block.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "hexpad.h"
#include "mips.h"
#include "panelconfig.h"

#define BLOCKVIEW_SUCCESS (0)
#define BLOCKVIEW_FAILURE (-1)
#define BLOCKVIEW_NULLPTR (-2)
#define BLOCKVIEW_MEMORY (-3)
#define BLOCKVIEW_INVIDX (-4)

#define BLOCKVIEW_POS_X (0)
#define BLOCKVIEW_POS_Y (3)
#define BLOCKVIEW_SIZE_W (60)
#define BLOCKVIEW_SIZE_H (20)
#define BLOCKVIEW_PANELBG ((u32)0xFFC0C0C0)
#define BLOCKVIEW_PANELFG ((u32)0xFF000000)
#define BLOCKVIEW_CURSORBG ((u32)0xFFE00000)
#define BLOCKVIEW_CURSORFG ((u32)0xFFFFFFFF)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _BlockView {
        PanelConfig panelConfig;
        CursorPos cursorPos;
        HexPad hexPad;
        int blockIndex;
        int blockCount;
        int pageIndex;
        int editing;
        int dirty;
        Block* blockList;
    }
    BlockView;

    int blockview_button_circle(BlockView* prView);
    
    int blockview_button_cross(BlockView* prView);

    int blockview_button_ltrigger(BlockView* prView);

    int blockview_button_rtrigger(BlockView* prView);

    int blockview_button_square(BlockView* prView);

    int blockview_button_triangle(BlockView* prView);
    
    int blockview_dpad_down(BlockView* prView);

    int blockview_dpad_left(BlockView* prView);

    int blockview_dpad_right(BlockView* prView);

    int blockview_dpad_up(BlockView* prView);
    
    int blockview_init(BlockView* prView);
    
    int blockview_page_down(BlockView* prView);
    
    int blockview_page_up(BlockView* prView);
    
    int blockview_redraw(BlockView* prView);
    
    int blockview_set_cursorcolor(BlockView* prView, const u32 background,
            const u32 text);
    
    int blockview_set_cursorpos(BlockView* prView, const int x, const int y);
    
    int blockview_set_panelcolor(BlockView* prView, const u32 background,
            const u32 text);
    
    int blockview_set_position(BlockView* prView, const int x, const int y);
    
    int blockview_set_size(BlockView* prView, const int width, 
            const int height);

#ifdef	__cplusplus
}
#endif

#endif	/* BLOCKVIEW_H */

