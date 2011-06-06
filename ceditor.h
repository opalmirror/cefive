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
    
    int ceditor_page_down(CEditor* prEd);
    
    int ceditor_page_up(CEditor* prEd);
    
    int ceditor_redraw(CEditor* prEd);

#ifdef	__cplusplus
}
#endif

#endif	/* CEDITOR_H */

