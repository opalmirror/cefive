/* 
 * File:   searchpanel.h
 * Authors: Sir Gee of Five, PSP_Lord
 *
 * Created on September 22, 2010, 11:04 AM
 */

#ifndef _SEARCHPANEL_H
#define	_SEARCHPANEL_H

#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "searchengine.h"
#include "dwordcolumn.h"
#include "addresscolumn.h"
#include "appletconfig.h"

#define SEARCHPANEL_SUCCESS     (0)
#define SEARCHPANEL_FAILURE     (-1)
#define SEARCHPANEL_MEMORY      (-2)
#define SEARCHPANEL_NULLPTR     (-3)

#define SEARCHPANEL_COLOR_BG    ((u32)0xFF000000)
#define SEARCHPANEL_COLOR_FG    ((u32)0xFFFFFFFF)
#define SEARCHPANEL_CURSOR_BG   ((u32)0xFFFFFFFF)
#define SEARCHPANEL_CURSOR_FG   ((u32)0xFF000000)
#define SEARCHPANEL_STATUS_BG   ((u32)0xFFFFFFFF)
#define SEARCHPANEL_STATUS_FG   ((u32)0xFF000000)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DwordResultRow {
        AddressColumn rAddress;
        DwordColumn rValue;
    } DwordResultRow;

    typedef struct _SearchPanelConfig {
        ColorConfig color;
        ColorConfig cursor;
        ColorConfig status;
        CursorPos top;
        CursorPos tabletop;
        Dimension size;
        Dimension tablesize;
        int status_line;
    } SearchPanelConfig;

    typedef struct _SearchPanel {
        SearchPanelConfig config;
        AppletConfig* prApCfg;
        SearchEngine* prEngine;
        CursorPos cursor;
        DwordColumn rQueryCol;
        DwordColumn rStartAddr;
        DwordColumn rEndAddr;
        int editing;
        int pagepos;
    } SearchPanel;

    /** set cursor back to "Query" by pressing Circle button 
     * when scrolling through search results
     * 
     * @param prPanel Pointer to a SearchPanel struct representing 
     * the result list
     * @return SEARCHPANEL_MEMORY if the SearchPanel pointer is NULL;
     *         SEARCHPANEL_SUCCESS if cursor has been set back;
     *         SEARCHPANEL_FAILURE if cursor has not been set 
     */
    int searchpanel_circle_button(SearchPanel* prPanel);

    int searchpanel_cross_button(SearchPanel* prPanel);
    int searchpanel_cursor_down(SearchPanel* prPanel);
    int searchpanel_cursor_left(SearchPanel* prPanel);
    int searchpanel_cursor_right(SearchPanel* prPanel);
    int searchpanel_cursor_up(SearchPanel* prPanel);
    int searchpanel_cycle_searchmode(SearchPanel* prPanel);
    int searchpanel_cycle_searchsize(SearchPanel* prPanel);
    int searchpanel_dpad_down(SearchPanel* prPanel);
    int searchpanel_dpad_left(SearchPanel* prPanel);
    int searchpanel_dpad_right(SearchPanel* prPanel);
    int searchpanel_dpad_down(SearchPanel* prPanel);
    int searchpanel_init(SearchPanel* prPanel, SearchEngine* prEngine);
    int searchpanel_is_editing(SearchPanel* prPanel);
    int searchpanel_page_down(SearchPanel* prPanel);
    int searchpanel_page_up(SearchPanel* prPanel);
    int searchpanel_redraw(SearchPanel* prPanel);
    int searchpanel_reset(SearchPanel* prPanel);
    int searchpanel_scroll_down(SearchPanel* prPanel);
    int searchpanel_scroll_up(SearchPanel* prPanel);
    int searchpanel_set_editing(SearchPanel* prPanel, int editing);

#ifdef	__cplusplus
}
#endif

#endif	/* _SEARCHPANEL_H */

