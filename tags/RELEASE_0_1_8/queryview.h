/* 
 * File:   queryview.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 3:23 PM
 */

#ifndef QUERYVIEW_H
#define	QUERYVIEW_H

#include <stdio.h>
#include <pspdebug.h>
#include <psptypes.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "hexpad.h"
#include "panelconfig.h"
#include "searchquery.h"

#define QUERYVIEW_SUCCESS (0)
#define QUERYVIEW_FAILURE (-1)
#define QUERYVIEW_NULLPTR (-2)
#define QUERYVIEW_MEMORY (-3)
#define QUERYVIEW_INVIDX (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _QueryView {
        PanelConfig panelConfig;
        CursorPos cursorPos;
        SearchQuery searchQuery;
        HexPad hexPad;
    }
    QueryView;

    int queryview_button_circle(QueryView *prView);
    int queryview_button_cross(QueryView *prView);
    int queryview_button_ltrigger(QueryView *prView);
    int queryview_button_rtrigger(QueryView *prView);
    int queryview_button_square(QueryView *prView);
    int queryview_button_triangle(QueryView *prView);
    int queryview_dpad_down(QueryView *prView);
    int queryview_dpad_left(QueryView *prView);
    int queryview_dpad_right(QueryView *prView);
    int queryview_dpad_up(QueryView *prView);
    PanelConfig* queryview_get_config(QueryView* prView);
    ColorConfig* queryview_get_cursorcolor(QueryView* prView);
    CursorPos* queryview_get_cursorpos(QueryView* prView);
    HexPad* queryview_get_hexpad(QueryView* prView);
    ColorConfig* queryview_get_panelcolor(QueryView* prView);
    CursorPos* queryview_get_position(QueryView* prView);
    SearchQuery* queryview_get_query(QueryView* prView);
    Dimension* queryview_get_size(QueryView* prView);
    int queryview_init(QueryView* prView);
    int queryview_redraw(QueryView* prView);
    int queryview_set_cursorcolor(QueryView* prView, 
            const u32 background, const u32 text);
    int queryview_set_cursorpos(QueryView* prView, const int x, const int y);
    int queryview_set_panelcolor(QueryView* prView,
            const u32 background, const u32 text);
    int queryview_set_position(QueryView* prView, const int x, const int y);
    int queryview_set_size(QueryView* prView,
            const int width, const int height);

#ifdef	__cplusplus
}
#endif

#endif	/* QUERYVIEW_H */

