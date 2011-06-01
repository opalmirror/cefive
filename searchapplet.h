/* 
 * File:   searchapplet.h
 * Author: Sir Gee of Five
 *
 * Created on May 31, 2011, 1:06 PM
 */

#ifndef SEARCHAPPLET_H
#define	SEARCHAPPLET_H

#include <stdio.h>
#include <psptypes.h>
#include <pspdebug.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "panelconfig.h"
#include "queryview.h"
#include "resultview.h"

#define SEARCHAPPLET_SUCCESS (0)
#define SEARCHAPPLET_FAILURE (-1)
#define SEARCHAPPLET_NULLPTR (-2)
#define SEARCHAPPLET_MEMORY (-3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _SearchApplet {
        PanelConfig panelConfig;
        QueryView queryView;
        ResultView resultView;
        /** The currently selected panel. */
        int panel;
    }
    SearchApplet;

    int searchapplet_button_circle(SearchApplet* prApp);
    
    int searchapplet_button_cross(SearchApplet* prApp);
    
    int searchapplet_button_ltrigger(SearchApplet* prApp);
    
    int searchapplet_button_rtrigger(SearchApplet* prApp);
    
    int searchapplet_button_square(SearchApplet* prApp);
    
    int searchapplet_button_triangle(SearchApplet* prApp);
    
    int searchapplet_dpad_down(SearchApplet* prApp);
    
    int searchapplet_dpad_left(SearchApplet* prApp);
    
    int searchapplet_dpad_right(SearchApplet* prApp);
    
    int searchapplet_dpad_up(SearchApplet* prApp);
    
    PanelConfig* searchapplet_get_config(SearchApplet* prApp);
    
    ColorConfig* searchapplet_get_cursorcolor(SearchApplet* prApp);
    
    ColorConfig* searchapplet_get_panelcolor(SearchApplet* prApp);
    
    CursorPos* searchapplet_get_position(SearchApplet* prApp);
    
    QueryView* searchapplet_get_queryview(SearchApplet* prApp);
    
    ResultView* searchapplet_get_resultview(SearchApplet* prApp);
    
    Dimension* searchapplet_get_size(SearchApplet* prApp);
    
    int searchapplet_init(SearchApplet* prApp);
    
    int searchapplet_redraw(SearchApplet* prApp);

    int searchapplet_set_cursorcolor(SearchApplet* prApp,
        const u32 background, const u32 text);
    
    int searchapplet_set_panelcolor(SearchApplet* prApp, 
            const u32 background, const u32 text);

    int searchapplet_set_position(SearchApplet* prApp, 
            const int x, const int y);

    int searchapplet_set_size(SearchApplet* prApp, 
        const int width, const int height);
    
#ifdef	__cplusplus
}
#endif

#endif	/* SEARCHAPPLET_H */

