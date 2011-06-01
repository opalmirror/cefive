#include "queryview.h"

static int commit_edit(QueryView* prView);
static int cycle_search_size(QueryView* prView);
static int cycle_search_type(QueryView* prView);
static int edit_end_address(QueryView* prView);
static int edit_start_address(QueryView* prView);
static int edit_value(QueryView* prView);
static int render_button_row(QueryView* prView);
static int render_range_row(QueryView* prView);
static int render_type_row(QueryView* prView);
static int render_value_row(QueryView* prView);
static int reset_query(QueryView* prView);

static int commit_edit(QueryView* prView) {
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    SearchQuery* prQuery = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->cancelled == 1) {
        if (hexpad_reset(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        prView->editing = 0;
        return QUERYVIEW_SUCCESS;
    }
    prQuery = queryview_get_query(prView);
    prCursor = queryview_get_cursorpos(prView);
    switch (prCursor->y) {
        case 0: /* Value row */
            break;
        case 1: /* Type / Size row */
            break;
        case 2: /* Range row */
            switch (prCursor->x) {
                case 0: /* Start Address Column */
                    prQuery->startAddress = hexpad_get_value(prPad);
                    break;
                case 1: /* End Address Column */
                    prQuery->endAddress = hexpad_get_value(prPad);
                    break;
            }
            break;
    }
    if (hexpad_reset(prPad) < 0) {
        return QUERYVIEW_FAILURE;
    }
    prView->editing = 0;
    return QUERYVIEW_SUCCESS;
}

static int cycle_search_size(QueryView* prView) {
    SearchQuery* prQuery = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prQuery = queryview_get_query(prView);
    switch (prQuery->searchSize) {
        case SSZ_Byte:
            prQuery->searchSize = SSZ_Half;
            break;
        case SSZ_Half:
            prQuery->searchSize = SSZ_Word;
            break;
        case SSZ_Word:
            prQuery->searchSize = SSZ_Byte;
            break;
        default:
            prQuery->searchSize = SSZ_Word;
    }

    return QUERYVIEW_SUCCESS;
}

static int cycle_search_type(QueryView* prView) {
    SearchQuery* prQuery = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prQuery = queryview_get_query(prView);
    switch (prQuery->searchMode) {
        case SMD_Immediate:
            prQuery->searchMode = SMD_Text;
            break;
        case SMD_Text:
            prQuery->searchMode = SMD_Byte;
            break;
        case SMD_Byte:
            prQuery->searchMode = SMD_Immediate;
            break;
        default:
            prQuery->searchMode = SMD_Immediate;
    }

    return QUERYVIEW_SUCCESS;
}

static int edit_end_address(QueryView* prView) {
    SearchQuery* prQuery = NULL;
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prQuery = queryview_get_query(prView);
    prPad = queryview_get_hexpad(prView);
    hexpad_set_value(prPad, prQuery->endAddress);
    prPad->digit = 0;
    prPad->visible = 1;
    prPad->cancelled = 0;
    prView->editing = 1;
    
    return QUERYVIEW_SUCCESS;
}

static int edit_start_address(QueryView* prView) {
    SearchQuery* prQuery = NULL;
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    prQuery = queryview_get_query(prView);
    prPad = queryview_get_hexpad(prView);
    hexpad_set_value(prPad, prQuery->startAddress);
    prPad->digit = 0;
    prPad->visible = 1;
    prPad->cancelled = 0;
    prView->editing = 1;

    return QUERYVIEW_SUCCESS;
}

static int edit_value(QueryView* prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_circle(QueryView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_button_circle(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_button_cross(QueryView* prView) {
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_button_cross(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    prCursor = queryview_get_cursorpos(prView);
    switch (prCursor->y) {
        case 0: /* Value Row */
            if (edit_value(prView) < 0) {
                return QUERYVIEW_FAILURE;
            }
            break;
        case 1: /* Type / Size Row */
            switch (prCursor->x) {
                case 0: /* Search Type Column */
                    if (cycle_search_type(prView) < 0) {
                        return QUERYVIEW_FAILURE;
                    }
                    break;
                case 1: /* Search Size Column */
                    if (cycle_search_size(prView) < 0) {
                        return QUERYVIEW_FAILURE;
                    }
                    break;
            }
            break;
        case 2: /* Range Row */
            switch (prCursor->x) {
                case 0: /* Start Address Column */
                    if (edit_start_address(prView) < 0) {
                        return QUERYVIEW_FAILURE;
                    }
                    break;
                case 1: /* End Address Column */
                    if (edit_end_address(prView) < 0) {
                        return QUERYVIEW_FAILURE;
                    }
                    break;
            }
            break;
        case 3: /* Button Row */
            switch (prCursor->x) {
                case 0: /* Search / Cancel / Continue Button */
                    break;
                case 1: /* Reset Button */
                    if (reset_query(prView) < 0) {
                        return QUERYVIEW_FAILURE;
                    }
                    break;
            }
            break;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_button_ltrigger(QueryView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_prev_digit(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_rtrigger(QueryView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_next_digit(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_square(QueryView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_button_square(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_triangle(QueryView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_button_triangle(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_down(QueryView* prView) {
    HexPad* prPad = NULL;
    CursorPos* prCursor = NULL;
    int y = 0;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_down(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    prCursor = queryview_get_cursorpos(prView);
    y = prCursor->y + 1;
    if (y > 3) {
        y = 0;
    }
    if (prCursor->y != y) {
        prCursor->x = 0;
    }
    prCursor->y = y;
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_left(QueryView* prView) {
    HexPad* prPad = NULL;
    CursorPos* prCursor = NULL;
    int x = 0;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_left(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    prCursor = queryview_get_cursorpos(prView);
    if ((prCursor->y >= 1) && (prCursor->y < 4)) {
        x = prCursor->x - 1;
        if (x < 0) {
            x = 1;
        }
        prCursor->x = x;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_right(QueryView* prView) {
    HexPad* prPad = NULL;
    CursorPos* prCursor = NULL;
    int x = 0;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_right(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    prCursor = queryview_get_cursorpos(prView);
    if ((prCursor->y >= 1) && (prCursor->y < 4)) {
        x = prCursor->x + 1;
        if (x > 1) {
            x = 0;
        }
        prCursor->x = x;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_up(QueryView* prView) {
    HexPad* prPad = NULL;
    CursorPos* prCursor = NULL;
    int y = 0;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPad = queryview_get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_up(prPad) < 0) {
            return QUERYVIEW_FAILURE;
        }
        return QUERYVIEW_SUCCESS;
    }
    prCursor = queryview_get_cursorpos(prView);
    y = prCursor->y - 1;
    if (y < 0) {
        y = 3;
    }
    if (prCursor->y != y) {
        prCursor->x = 0;
    }
    prCursor->y = y;
    return QUERYVIEW_SUCCESS;
}

PanelConfig* queryview_get_config(QueryView* prView) {
    if (prView != NULL) {
        return &prView->panelConfig;
    }
    return NULL;
}

ColorConfig* queryview_get_cursorcolor(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_cursorcolor(prConfig);
    }
    return NULL;
}

CursorPos* queryview_get_cursorpos(QueryView* prView) {
    if (prView != NULL) {
        return &prView->cursorPos;
    }
    return NULL;
}

HexPad* queryview_get_hexpad(QueryView* prView) {
    if (prView != NULL) {
        return &prView->hexPad;
    }
    return NULL;
}

ColorConfig* queryview_get_panelcolor(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_panelcolor(prConfig);
    }
    return NULL;
}

CursorPos* queryview_get_position(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_position(prConfig);
    }
    return NULL;
}

SearchQuery* queryview_get_query(QueryView* prView) {
    if (prView != NULL) {
        return &prView->searchQuery;
    }
    return NULL;
}

Dimension* queryview_get_size(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_size(prConfig);
    }
    return NULL;
}

int queryview_init(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prConfig = queryview_get_config(prView);
    if (panelconfig_init(prConfig) < 0) {
        return QUERYVIEW_FAILURE;
    }
    prCursor = queryview_get_cursorpos(prView);
    if (cursorpos_init(prCursor) < 0) {
        return QUERYVIEW_FAILURE;
    }
    prPad = queryview_get_hexpad(prView);
    if (hexpad_init(prPad) < 0) {
        return QUERYVIEW_FAILURE;
    }
    prView->editing = 0;
    return QUERYVIEW_SUCCESS;
}

int queryview_redraw(QueryView* prView) {
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    /* If the Query View is editing, the Hex Pad is active. */
    if (prView->editing == 1) {
        prPad = queryview_get_hexpad(prView);
        /* If the Hex Pad is visible, redraw it. */
        if (prPad->visible == 1) {
            if (hexpad_redraw(prPad) < 0) {
                return QUERYVIEW_FAILURE;
            }
            return QUERYVIEW_SUCCESS;
        }
        /* If the Hex Pad is no longer visible, commit the edit. */
        if (commit_edit(prView) < 0) {
            return QUERYVIEW_FAILURE;
        }
    }
    
    if (render_value_row(prView) < 0) {
        return QUERYVIEW_FAILURE;
    }
    if (render_type_row(prView) < 0) {
        return QUERYVIEW_FAILURE;
    }
    if (render_range_row(prView) < 0) {
        return QUERYVIEW_FAILURE;
    }
    if (render_button_row(prView) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_set_cursorcolor(QueryView* prView,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prColor = queryview_get_cursorcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return QUERYVIEW_FAILURE;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_set_cursorpos(QueryView* prView, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prCursor = queryview_get_cursorpos(prView);
    if (cursorpos_set(prCursor, x, y) < 0) {
        return QUERYVIEW_FAILURE;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_set_panelcolor(QueryView* prView,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prColor = queryview_get_panelcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_set_position(QueryView* prView, const int x, const int y) {
    CursorPos* prPos = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPos = queryview_get_position(prView);
    if (cursorpos_set(prPos, x, y) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_set_size(QueryView* prView,
        const int width, const int height) {
    Dimension* prSize = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prSize = queryview_get_size(prView);
    if (dimension_set(prSize, width, height) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

static int render_button_row(QueryView* prView) {
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCColor = NULL;
    CursorPos* prCursor = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prSize = queryview_get_size(prView);
    prPos = queryview_get_position(prView);
    prColor = queryview_get_panelcolor(prView);
    prCColor = queryview_get_cursorcolor(prView);
    prCursor = queryview_get_cursorpos(prView);

    return QUERYVIEW_SUCCESS;
}

static int render_range_row(QueryView* prView) {
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCColor = NULL;
    CursorPos* prCursor = NULL;
    SearchQuery* prQuery = NULL;
    char sFmt[10];
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prQuery = queryview_get_query(prView);
    prSize = queryview_get_size(prView);
    prPos = queryview_get_position(prView);
    prColor = queryview_get_panelcolor(prView);
    prCColor = queryview_get_cursorcolor(prView);
    prCursor = queryview_get_cursorpos(prView);
    
    pspDebugScreenSetXY(prCursor->x, prCursor->y + 3);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenPuts("Range: Start ");
    if ((prCursor->y == 2) && (prCursor->x == 0)) {
        pspDebugScreenSetBackColor(prCColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenKprintf("0x%08X", prQuery->startAddress);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenPuts(" End ");
    if ((prCursor->y == 2) && (prCursor->x == 1)) {
        pspDebugScreenSetBackColor(prCColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenKprintf("0x%08X", prQuery->endAddress);
    sprintf(sFmt, "%%-%ds", prSize->width - 38);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf(sFmt, "");
    return QUERYVIEW_SUCCESS;
}

static int render_type_row(QueryView* prView) {
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCColor = NULL;
    CursorPos* prCursor = NULL;
    SearchQuery* prQuery = NULL;
    char* sType = NULL;
    char* sSize = NULL;
    char sFmt[10];
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prQuery = queryview_get_query(prView);
    prSize = queryview_get_size(prView);
    prPos = queryview_get_position(prView);
    prColor = queryview_get_panelcolor(prView);
    prCColor = queryview_get_cursorcolor(prView);
    prCursor = queryview_get_cursorpos(prView);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 2);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenPuts("Type: ");
    if ((prCursor->y == 1) && (prCursor->x == 0)) {
        pspDebugScreenSetBackColor(prCColor->background);
        pspDebugScreenSetTextColor(prCColor->text);
    }
    switch (prQuery->searchMode) {
        case SMD_Byte:
            sType = "Exact Value";
            break;
        case SMD_Immediate:
            sType = "Immediate Value";
            break;
        case SMD_Text:
            sType = "Text";
            break;
        default:
            sType = "None";
    }
    pspDebugScreenKprintf("%-16s", sType);

    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenPuts("Size: ");
    if ((prCursor->y == 1) && (prCursor->x == 1)) {
        pspDebugScreenSetBackColor(prCColor->background);
        pspDebugScreenSetTextColor(prCColor->text);
    }
    switch (prQuery->searchSize) {
        case SSZ_Byte:
            sSize = "Byte (8-bits)";
            break;
        case SSZ_Half:
            sSize = "Halfword (16-bits)";
            break;
        case SSZ_Word:
            sSize = "Word (32-bits)";
            break;
        default:
            sSize = "None";
    }
    pspDebugScreenKprintf("%-18s", sSize);
    sprintf(sFmt, "%%-%ds", prSize->width - 42);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf(sFmt, "");
    return QUERYVIEW_SUCCESS;
}

static int render_value_row(QueryView* prView) {
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    CursorPos* prCursor = NULL;
    SearchQuery* prQuery = NULL;
    char sFmt[10];
    char sLine[70];
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prQuery = queryview_get_query(prView);
    prSize = queryview_get_size(prView);
    prPos = queryview_get_position(prView);
    prColor = queryview_get_panelcolor(prView);
    prCursor = queryview_get_cursorpos(prView);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 1);
    if (prCursor->y == 0) {
        prColor = queryview_get_cursorcolor(prColor);
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    sprintf(sFmt, "%%-%ds", prSize->width);
    switch (prQuery->searchMode) {
        case SMD_Byte:
            switch(prQuery->searchSize) {
                case SSZ_Byte:
                    sprintf(sLine, "Value: 0x%02X", (SceUChar8)prQuery->value);
                    break;
                case SSZ_Half:
                    sprintf(sLine, "Value: 0x%04X", 
                            (SceUShort16)prQuery->value);
                    break;
                case SSZ_Word:
                    sprintf(sLine, "Value: 0x%08X", prQuery->value);
                    break;
                default:
                    sprintf(sLine, "Value: 0x%08X", prQuery->value);
            }
            break;
        case SMD_Immediate:
            sprintf(sLine, "Value: 0x%08X", prQuery->value);
            break;
        case SMD_Text:
            if (strlen(prQuery->text) > 0) {
                sprintf(sLine, "Value: %s", prQuery->text);
            } else {
                sprintf(sLine, "Value: <none>");
            }
            break;
        default:
            sprintf(sLine, "Value: <unknown>");
    }
    pspDebugScreenKprintf(sFmt, sLine);
    return QUERYVIEW_SUCCESS;
}

static int reset_query(QueryView* prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}
