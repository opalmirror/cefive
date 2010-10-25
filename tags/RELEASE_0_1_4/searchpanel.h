/* 
 * File:   searchpanel.h
 * Author: Sir Gee of Five
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

typedef struct _DwordResultRow {
    AddressColumn rAddress;
    DwordColumn rValue;
}DwordResultRow;

typedef struct _SearchPanelConfig {
    ColorConfig color;
    ColorConfig cursor;
    ColorConfig status;
    CursorPos top;
    CursorPos tabletop;
    Dimension size;
    Dimension tablesize;
    int status_line;
}SearchPanelConfig;

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
}SearchPanel;

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

#endif	/* _SEARCHPANEL_H */

