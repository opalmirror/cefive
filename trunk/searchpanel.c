#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "searchengine.h"
#include "searchpanel.h"
#include "dwordcolumn.h"
#include "addresscolumn.h"
#include "appletconfig.h"
#include "colorconfig.h"

static int draw_button_panel(SearchPanel* prPanel);
static int draw_dword_results(SearchPanel* prPanel);
static int draw_query_panel(SearchPanel* prPanel);
static int draw_results_panel(SearchPanel* prPanel);
static int draw_status_panel(SearchPanel* prPanel);
static int reset(SearchPanel* prPanel);
static int search(SearchPanel* prPanel);

static int draw_button_panel(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    char* sLabel = "Search";
    int y = 0;
    int x = 0;
    int row = 0;
    int col = 0;
    u32 bg = 0;
    u32 fg = 0;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prApCfg = prPanel->prApCfg;
    if (prEngine->rState == ESS_Searching) {
        sLabel = "Cancel";
    }
    if (prEngine->rState == ESS_Finished) {
        sLabel = "Continue";
    }
    x = prPanel->config.top.x + 2;
    y = prPanel->config.top.y + 7;
    row = prPanel->cursor.y;
    col = prPanel->cursor.x;
    if (prApCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prApCfg);
    } else {
        prColor = &prPanel->config.color;
    }
    if (row == 4) {
        if (col == 0) {
            if (prApCfg != NULL) {
                prColor = appletconfig_get_cursorcolor(prApCfg);
            } else {
                prColor = &prPanel->config.cursor;
            }
        }
    }
    pspDebugScreenSetXY(x, y);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("%-10s", sLabel);

    x += 10;
    if (prApCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prApCfg);
    } else {
        prColor = &prPanel->config.color;
    }
    if (row == 4) {
        if (col == 1) {
            if (prApCfg != NULL) {
                prColor = appletconfig_get_cursorcolor(prApCfg);
            } else {
                prColor = &prPanel->config.cursor;
            }
        }
    }
    pspDebugScreenSetXY(x, y);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("%-10s", "Reset");

    return SEARCHPANEL_SUCCESS;
}

static int draw_dword_results(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    SearchResult* prResult = NULL;
    DwordResultRow rRow;
    AddressColumn* prAddr = NULL;
    DwordColumn* prVal = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCursor = NULL;
    int iStart = 0;
    int iEnd = 0;
    int iResults = 0;
    int iRows = 0;
    int iRow = 0;
    int iTop = 0;
    int iX = 0;
    int iY = 0;
    int iSelRes = 0;
    unsigned int vaddr;

    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prApCfg = prPanel->prApCfg;
    iRows = prPanel->config.tablesize.height;
    iResults = prEngine->result_count;
    iStart = prPanel->pagepos;
    iTop = prPanel->config.tabletop.y;
    iX = prPanel->config.tabletop.x;
    if (iStart > iResults) {
        iStart = 0;
    }
    if (iResults < iRows) {
        iRows = iResults;
    }
    iEnd = iStart + iRows;
    prAddr = &rRow.rAddress;
    prVal = &rRow.rValue;
    addresscolumn_init(prAddr);
    dwordcolumn_init(prVal);
    prColor = appletconfig_get_panelcolor(prApCfg);
    prCursor = appletconfig_get_cursorcolor(prApCfg);
    iY = iTop;
    iSelRes = prPanel->cursor.y;
    for (iRow = iStart; iRow < iEnd; iRow++) {
        prResult = searchengine_get_result(prEngine, iRow);
        if (prResult == NULL) {
            continue;
        }
        pspDebugScreenSetXY(iX, iY);
        vaddr = prResult->address & ~0x40000000;
        addresscolumn_setvalue(prAddr, vaddr);
        colorconfig_setcolor(&prAddr->color, prColor->background, 
                prColor->text);
        if (iSelRes > 4) {
            if ((iY - iTop) == (iSelRes - 5)) {
                colorconfig_setcolor(&prAddr->color, prCursor->background, 
                        prCursor->text);
            }
        }
        addresscolumn_redraw(prAddr);

        pspDebugScreenSetXY(iX + 12, iY);
        dwordcolumn_setvalue(prVal, prResult->value);
        colorconfig_setcolor(&prVal->color, prColor->background, 
                prColor->text);
        if (iSelRes > 4) {
            if ((iY - iTop) == (iSelRes - 5)) {
                colorconfig_setcolor(&prVal->color, prCursor->background, 
                        prCursor->text);
            }
        }
        dwordcolumn_redraw(prVal);
        iY++;
    }

    return SEARCHPANEL_SUCCESS;
}

static int draw_query_panel(SearchPanel* prPanel) {
    SearchPanelConfig* prPcfg = NULL;
    SearchEngine* prEngine = NULL;
    SearchQuery* prQuery = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCursor = NULL;
    int x = 0;
    int y = 0;
    char *sMode = "None";
    char *sSize = "None";
    char *fs = "0x%08X";
    int row = 0;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prPcfg = &prPanel->config;
    prEngine = prPanel->prEngine;
    prQuery = &prEngine->rQuery;
    prApCfg = prPanel->prApCfg;
    ESearchMode rMode = prQuery->searchMode;
    ESearchSize rSize = prQuery->searchSize;
    x = prPcfg->top.x;
    y = prPcfg->top.y + 2;
    row = prPanel->cursor.y;
    if (prApCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prApCfg);
        prCursor = appletconfig_get_cursorcolor(prApCfg);
    } else {
        prColor = &prPcfg->color;
        prCursor = &prPcfg->cursor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y);
    pspDebugScreenKprintf("%-10s", "Mode:");
    switch (rMode) {
        case ESM_Immediate:
            sMode = "Immediate Value";
            break;
        case ESM_Text:
            sMode = "Text";
            break;
        case ESM_Byte:
            sMode = "Exact Value";
            break;
    }
    if (row == 0) {
        pspDebugScreenSetBackColor(prCursor->background);
        pspDebugScreenSetTextColor(prCursor->text);
    }
    pspDebugScreenKprintf("%-50s", sMode);
    y++;

    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y);
    pspDebugScreenKprintf("%-10s", "Size:");
    switch (rSize) {
        case ESZ_Byte:
            sSize = "Byte (8-bits)";
            break;
        case ESZ_Dword:
            sSize = "Word (32-bits or 4 bytes)";
            break;
        case ESZ_Word:
            sSize = "Half-Word (16-bits or 2 bytes)";
            break;
    }
    if (row == 1) {
        pspDebugScreenSetBackColor(prCursor->background);
        pspDebugScreenSetTextColor(prCursor->text);
    }
    pspDebugScreenKprintf("%-50s", sSize);
    y++;

    /* Search Range Row */
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y);
    pspDebugScreenPuts("Range:    Start ");
    DwordColumn *prStart = &prPanel->rStartAddr;
    DwordColumn *prEnd = &prPanel->rEndAddr;
    colorconfig_setcolor(&prStart->color, prColor->background, prColor->text);
    colorconfig_setcolor(&prEnd->color, prColor->background, prColor->text);
    if (row == 2) {
        pspDebugScreenSetBackColor(prCursor->background);
        pspDebugScreenSetTextColor(prCursor->text);
        if (prPanel->cursor.x == 0) {
            colorconfig_setcolor(&prStart->color, prCursor->background,
                    prCursor->text);
        } else {
            colorconfig_setcolor(&prEnd->color, prCursor->background,
                    prCursor->text);
        }
    }
    if (searchpanel_is_editing(prPanel) == 0) {
        dwordcolumn_setvalue(prStart, prQuery->startAddr & 0xBFFFFFFF);
        dwordcolumn_setvalue(prEnd, prQuery->endAddr & 0xBFFFFFFF);
    }
    dwordcolumn_redraw(prStart);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenPuts(" End ");
    dwordcolumn_redraw(prEnd);
    y++;

    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y);
    pspDebugScreenKprintf("%-10s", "Query:");
    DwordColumn *prCol = &prPanel->rQueryCol;
    prCol->color.background = prColor->background;
    prCol->color.text = prColor->text;
    if (row == 3) {
        pspDebugScreenSetBackColor(prCursor->background);
        pspDebugScreenSetTextColor(prCursor->text);
        prCol->color.background = prCursor->background;
        prCol->color.text = prCursor->text;
    }
    if (rMode == ESM_Text) {
        pspDebugScreenKprintf("%-50s", prQuery->text);
    } else {
        if (searchpanel_is_editing(prPanel) == 1) {
            dwordcolumn_redraw(prCol);
        } else {
            switch (rSize) {
                case ESZ_Byte:
                    fs = "      0x%02X";
                    break;
                case ESZ_Dword:
                    fs = "0x%08X";
                    break;
                case ESZ_Word:
                    fs = "    0x%04X";
                    break;
            }
            pspDebugScreenKprintf(fs, prQuery->value);
        }
    }
    
    return SEARCHPANEL_SUCCESS;
}

static int draw_results_panel(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    SearchResult* prResult = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    int row = 0;
    int col = 0;
    int x = 0;
    int y = 0;
    int rct = 0;
    int i = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prApCfg = prPanel->prApCfg;
    rct = prEngine->result_count;

    x = prPanel->config.top.x;
    y = prPanel->config.top.y + 9;

    pspDebugScreenSetXY(x, y);
    if (prApCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prApCfg);
    } else {
        prColor = &prPanel->config.color;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("Search Results: %d", rct);
    prPanel->config.tabletop.y = 11;
    prPanel->config.tablesize.height = 20;
    draw_dword_results(prPanel);
    return SEARCHPANEL_SUCCESS;
}

static int draw_status_panel(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    char* sStatus = "Unknown Status";
    char sSmsg[60];
    int x = 0;
    int y = 0;
    ESearchState rEss;
    SceUInt32 pos = 0;
    SceUInt32 max = 0;
    SceUInt32 min = 0;
    SceUInt32 vpos = 0;
    int tdist = 0;
    int edist = 0;
    int cp = 0;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prApCfg = prPanel->prApCfg;
    x = prPanel->config.top.x;
    y = prPanel->config.status_line;
    rEss = prEngine->rState;
    switch (rEss) {
        case ESS_Fault:
            sStatus = "SearchEngine encountered fault.";
            break;
        case ESS_Stopped:
            sStatus = "SearchEngine not running.";
            break;
        case ESS_Idle:
            sStatus = "Idle.";
            break;
        case ESS_Searching:
            pos = searchengine_tell(prEngine);
            max = prEngine->rConfig.max_position;
            min = prEngine->rConfig.min_position;
            tdist = max - min;
            edist = pos - min;
            cp = ((edist / tdist) * 100);
            vpos = pos & ~0x40000000;
            sprintf(sSmsg, "Searching...0x%08X %d%%", vpos, cp);
            sStatus = sSmsg;
            break;
        case ESS_Finished:
            sStatus = "Search Finished.";
            break;
    }
    pspDebugScreenSetXY(x, y);
    if (prApCfg != NULL) {
        prColor = appletconfig_get_statuscolor(prApCfg);
    } else {
        prColor = &prPanel->config.status;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("Status: %-59s", sStatus);

    return SEARCHPANEL_SUCCESS;
}

static int reset(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    if (prEngine->rState != ESS_Finished) {
        return SEARCHPANEL_FAILURE;
    }
    searchengine_reset(prEngine);
    return searchpanel_reset(prPanel);
}
/* reposition of the cursor when pressing Circle Button while scrolling through search results */
int searchpanel_circle_button(SearchPanel* prPanel) {
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    if (prPanel->cursor.y > 4) {
        prPanel->cursor.x = 0;
        prPanel->cursor.y = 3;
        return SEARCHPANEL_SUCCESS;
    }
    
    return SEARCHPANEL_FAILURE;
}

/* Handles the X Button Input in Search Panel */
int searchpanel_cross_button(SearchPanel* prPanel) {
    int x = 0;
    int y = 0;
    unsigned int value = 0;
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    SearchEngine* prEngine = prPanel->prEngine;
    SearchQuery* prQuery = &prEngine->rQuery;
    x = prPanel->cursor.x;
    y = prPanel->cursor.y;

    /* Switch the "Mode" */
    if (y == 0) {
        searchpanel_cycle_searchmode(prPanel);
        return SEARCHPANEL_SUCCESS;
    }
    /* Switch the "Size" */
    if (y == 1) {
        searchpanel_cycle_searchsize(prPanel);
        return SEARCHPANEL_SUCCESS;
    }
    /* Switch the "Range" */
    if (y == 2) {
        if (searchpanel_is_editing(prPanel) == 0) {
            if (x == 0) {
                dwordcolumn_edit(&prPanel->rStartAddr, 
                        prQuery->startAddr & 0xBFFFFFFF);
            } else {
                dwordcolumn_edit(&prPanel->rEndAddr, 
                        prQuery->endAddr & 0xBFFFFFFF);
            }
            searchpanel_set_editing(prPanel, 1);
        } else {
            if (x == 0) {
                DwordColumn* prStart = &prPanel->rStartAddr;
                value = dwordcolumn_commit(prStart);
                prQuery->startAddr = value | 0x40000000;
            } else {
                DwordColumn* prEnd = &prPanel->rEndAddr;
                value = dwordcolumn_commit(prEnd);
                prQuery->endAddr = value | 0x40000000;
            }
            searchpanel_set_editing(prPanel, 0);
        }
    }
    /* Change the "Query" */
    if (y == 3) {
        if (searchpanel_is_editing(prPanel) == 0) {
            dwordcolumn_edit(&prPanel->rQueryCol, prPanel->rQueryCol.value);
            searchpanel_set_editing(prPanel, 1);
        } else {
            DwordColumn* prCol = &prPanel->rQueryCol;
            value = dwordcolumn_commit(prCol);
            prQuery->value = value;
            searchpanel_set_editing(prPanel, 0);
        }
    }
    /* start the searching */
    if (y == 4) {
        if (x == 0) {
            search(prPanel);
        }
        /* reset previous search results */
        if (x == 1) {
            reset(prPanel);
        }
    }

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_cursor_down(SearchPanel* prPanel) {
    SearchEngine *prEngine = NULL;
    int row = 0;
    int results = 0;
    int pageheight = 0;
    int vrow = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    results = searchengine_get_result_count(prEngine);
    pageheight = prPanel->config.tablesize.height;
    row = prPanel->cursor.y;
    row++;
    if (row > 4) {
        if (results <= 0) {
            row = 4;
        } else {
            vrow = row - 5;
            if (vrow > pageheight - 1) {
                searchpanel_scroll_down(prPanel);
                vrow = pageheight - 1;
                row = vrow + 5;
            }
        }
    }
    prPanel->cursor.y = row;

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_cursor_left(SearchPanel* prPanel) {
    int col = 0;
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    col = prPanel->cursor.x;
    col--;
    if (col < 0) {
        col = 0;
    }
    prPanel->cursor.x = col;

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_cursor_right(SearchPanel* prPanel) {
    int col = 0;
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    col = prPanel->cursor.x;
    col++;
    if (col > 1) {
        col = 1;
    }
    prPanel->cursor.x = col;

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_cursor_up(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    int row = 0;
    int pagepos = 0;
    int vrow = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    row = prPanel->cursor.y;
    if (row >= 5) {
        
    }
    row--;
    if (row < 0) {
        row = 0;
    }
    if (row > 3) {
        vrow = row - 5;
        
        pagepos = prPanel->pagepos;
        if (pagepos > 0 && vrow < 0) {
            searchpanel_scroll_up(prPanel);
            row = 5;
        }
    }
    prPanel->cursor.y = row;

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_cycle_searchmode(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    searchengine_cycle_mode(prEngine);

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_cycle_searchsize(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    searchengine_cycle_size(prEngine);

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_dpad_down(SearchPanel* prPanel) {
    DwordColumn* prCol = NULL;
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    if (searchpanel_is_editing(prPanel) == 0) {
        searchpanel_cursor_down(prPanel);
    } else {
        if (prPanel->cursor.y == 2) {
            if (prPanel->cursor.x == 0) {
                prCol = &prPanel->rStartAddr;
            } else {
                prCol = &prPanel->rEndAddr;
            }
        }
        if (prPanel->cursor.y == 3) {
            prCol = &prPanel->rQueryCol;
        }
        if (prCol != NULL) {
            dwordcolumn_decrement(prCol);
        }
    }

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_dpad_left(SearchPanel* prPanel) {
    DwordColumn* prCol = NULL;
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    if (searchpanel_is_editing(prPanel) == 0) {
        searchpanel_cursor_left(prPanel);
    } else {
        if (prPanel->cursor.y == 2) {
            if (prPanel->cursor.x == 0) {
                prCol = &prPanel->rStartAddr;
            } else {
                prCol = &prPanel->rEndAddr;
            }
        }
        if (prPanel->cursor.y == 3) {
            prCol = &prPanel->rQueryCol;
        }
        if (prCol != NULL) {
            dwordcolumn_prevdigit(prCol);
        }
    }

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_dpad_right(SearchPanel* prPanel) {
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    if (searchpanel_is_editing(prPanel) == 0) {
        searchpanel_cursor_right(prPanel);
    } else {
        if (prPanel->cursor.y == 2) {
            if (prPanel->cursor.x == 0) {
                dwordcolumn_nextdigit(&prPanel->rStartAddr);
            } else {
                dwordcolumn_nextdigit(&prPanel->rEndAddr);
            }
        }
        if (prPanel->cursor.y == 3) {
            dwordcolumn_nextdigit(&prPanel->rQueryCol);
        }
    }

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_dpad_up(SearchPanel* prPanel) {
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    if (searchpanel_is_editing(prPanel) == 0) {
        searchpanel_cursor_up(prPanel);
    } else {
        if (prPanel->cursor.y == 2) {
            if (prPanel->cursor.x == 0) {
                dwordcolumn_increment(&prPanel->rStartAddr);
            } else {
                dwordcolumn_increment(&prPanel->rEndAddr);
            }
        }
        if (prPanel->cursor.y == 3) {
            dwordcolumn_increment(&prPanel->rQueryCol);
        }
    }

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_init(SearchPanel* prPanel, SearchEngine* prEngine) {
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prPanel->prEngine = prEngine;
    prPanel->config.color.background = SEARCHPANEL_COLOR_BG;
    prPanel->config.color.text = SEARCHPANEL_COLOR_FG;
    prPanel->config.cursor.background = SEARCHPANEL_CURSOR_BG;
    prPanel->config.cursor.text = SEARCHPANEL_CURSOR_FG;
    prPanel->config.status.background = SEARCHPANEL_STATUS_BG;
    prPanel->config.status.text = SEARCHPANEL_STATUS_FG;
    prPanel->config.size.height = 20;
    prPanel->config.size.width = 67;
    prPanel->config.top.x = 0;
    prPanel->config.top.y = 1;
    prPanel->cursor.x = 0;
    prPanel->cursor.y = 0;
    prPanel->config.status_line = 33;
    dwordcolumn_init(&prPanel->rQueryCol);
    dwordcolumn_init(&prPanel->rStartAddr);
    dwordcolumn_init(&prPanel->rEndAddr);

    return SEARCHPANEL_SUCCESS;
}

int searchpanel_is_editing(SearchPanel* prPanel) {
    int is = 0;
    if (prPanel != NULL) {
        if (prPanel->editing != 0) {
            is = 1;
        }
    }
    return is;
}

int searchpanel_page_down(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    int pgpos = 0;
    int tbht = 0;
    int resct = 0;
    int newpos = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    pgpos = prPanel->pagepos;
    tbht = prPanel->config.tablesize.height;
    resct = searchengine_get_result_count(prEngine);
    newpos = pgpos + tbht;
    if (newpos > resct) {
        // Don't allow it to page down too far.
        newpos = resct - tbht;
    }
    prPanel->pagepos = newpos;
    return SEARCHPANEL_SUCCESS;
}

int searchpanel_page_up(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    int pgpos = 0;
    int tbht = 0;
    int newpos = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    pgpos = prPanel->pagepos;
    tbht = prPanel->config.tablesize.height;
    newpos = pgpos - tbht;
    if (newpos < 0) {
        newpos = 0;
    }
    prPanel->pagepos = newpos;
    return SEARCHPANEL_SUCCESS;
}

int searchpanel_redraw(SearchPanel* prPanel) {
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prApCfg = prPanel->prApCfg;

    pspDebugScreenSetXY(prPanel->config.top.x, prPanel->config.top.y);
    if (prApCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prApCfg);
    } else {
        prColor = &prPanel->config.color;
    }

    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenPuts("Search Panel");
    draw_query_panel(prPanel);
    draw_button_panel(prPanel);
    draw_results_panel(prPanel);
    draw_status_panel(prPanel);
    
    return SEARCHPANEL_SUCCESS;
}

int searchpanel_reset(SearchPanel* prPanel) {
    AppletConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    int x = 0;
    int y = 0;
    int rows = 0;
    int row = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prCfg = prPanel->prApCfg;
    prColor = appletconfig_get_panelcolor(prCfg);
    x = prPanel->config.tabletop.x;
    y = prPanel->config.tabletop.y;
    rows = prPanel->config.tablesize.height;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y - 1);
    pspDebugScreenKprintf("%-67s", "Search Results: 0");
    for (row = 0; row < rows; row++) {
        pspDebugScreenSetXY(x, y + row);
        pspDebugScreenKprintf("%-67s", "");
    }
    return SEARCHPANEL_SUCCESS;
}

int searchpanel_scroll_down(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    int py = 0;
    int th = 0;
    int sr = 0;
    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    py = prPanel->pagepos;
    th = prPanel->config.tablesize.height;
    sr = searchengine_get_result_count(prEngine);
    
    py++;
    if (py + th > sr) {
        // Scrolling down would go beyond the end of the table.
        py = sr - th;
    }
    prPanel->pagepos = py;
    
    return SEARCHPANEL_SUCCESS;
}

int searchpanel_scroll_up(SearchPanel* prPanel) {
    int py = 0;
    
    if (prPanel == NULL) {
        return SEARCHPANEL_NULLPTR;
    }
    py = prPanel->pagepos;
    py--;
    if (py < 0) {
        py = 0;
    }
    prPanel->pagepos = py;
    return SEARCHPANEL_SUCCESS;
}

int searchpanel_set_editing(SearchPanel* prPanel, int editing) {
    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prPanel->editing = (editing == 0) ? 0 : 1;
    return SEARCHPANEL_SUCCESS;
}

static int search(SearchPanel* prPanel) {
    SearchEngine* prEngine = NULL;
    ESearchState rState = ESS_Fault;

    if (prPanel == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return SEARCHPANEL_MEMORY;
    }
    rState = prEngine->rState;
    if (rState == ESS_Idle) {
        // Start Search
        searchengine_search(prEngine);
        return SEARCHPANEL_SUCCESS;
    }
    if (rState == ESS_Searching) {
        // Cancel Search
        searchengine_cancel(prEngine);
        return SEARCHPANEL_SUCCESS;
    }
    if (rState == ESS_Finished) {
        // Continue Search
    }
    return SEARCHPANEL_SUCCESS;
}
