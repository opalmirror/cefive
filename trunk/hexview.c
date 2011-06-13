#include "hexview.h"

static int cursor_color(HexView* prView);
static int cursor_down(HexView* prView);
static int cursor_left(HexView* prView);
static int cursor_right(HexView* prView);
static int cursor_up(HexView* prView);
static PanelConfig* get_config(HexView* prView);
static ColorConfig* get_cursorcolor(HexView* prView);
static CursorPos* get_cursorpos(HexView* prView);
static HexPad* get_hexpad(HexView* prView);
static ColorConfig* get_panelcolor(HexView* prView);
static CursorPos* get_position(HexView* prView);
static Dimension* get_size(HexView* prView);
static int panel_color(HexView* prView);
static int render_address_col(HexView* prView, const int row);
static int render_row(HexView* prView, const int row);
static int render_value_col(HexView* prView, const int row, const int col);
static int render_value_row(HexView* prView, const int row);
static int render_value_row16(HexView* prView, const int row);
static int render_value_row4(HexView* prView, const int row);
static int render_value_row8(HexView* prView, const int row);
static SceUInt32 row_address(HexView* prView, const int row);
static int scroll_down(HexView* prView, const int rows);
static int scroll_up(HexView* prView, const int rows);

static int cursor_color(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    pspDebugScreenSetBackColor(prView->panelConfig.rCursor.background);
    pspDebugScreenSetTextColor(prView->panelConfig.rCursor.text);
    return HEXVIEW_SUCCESS;
}

static int cursor_down(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

static int cursor_left(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

static int cursor_right(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

static int cursor_up(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

static PanelConfig* get_config(HexView* prView) {
    if (prView == NULL) {
        return NULL;
    }
    return &prView->panelConfig;
}

static ColorConfig* get_cursorcolor(HexView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView == NULL) {
        return NULL;
    }
    prConfig = get_config(prView);
    return panelconfig_get_cursorcolor(prConfig);
}

static CursorPos* get_cursorpos(HexView* prView) {
    if (prView == NULL) {
        return NULL;
    }
    return &prView->cursorPos;
}

static HexPad* get_hexpad(HexView* prView) {
    if (prView == NULL) {
        return NULL;
    }
    return &prView->hexPad;
}

static ColorConfig* get_panelcolor(HexView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView == NULL) {
        return NULL;
    }
    prConfig = get_config(prView);
    return panelconfig_get_panelcolor(prConfig);
}

static CursorPos* get_position(HexView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView == NULL) {
        return NULL;
    }
    prConfig = get_config(prView);
    return panelconfig_get_position(prConfig);
}

static Dimension* get_size(HexView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView == NULL) {
        return NULL;
    }
    prConfig = get_config(prView);
    return panelconfig_get_size(prConfig);
}

int hexview_button_circle(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_button_cross(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_button_ltrigger(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_button_rtrigger(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_button_square(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_button_triangle(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_dpad_down(HexView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prPad = get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_down(prPad) < 0) {
            return HEXVIEW_FAILURE;
        }
        return HEXVIEW_SUCCESS;
    }
    if (cursor_down(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_dpad_left(HexView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prPad = get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_left(prPad) < 0) {
            return HEXVIEW_FAILURE;
        }
        return HEXVIEW_SUCCESS;
    }
    if (cursor_left(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_dpad_right(HexView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prPad = get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_right(prPad) < 0) {
            return HEXVIEW_FAILURE;
        }
        return HEXVIEW_SUCCESS;
    }
    if (cursor_right(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_dpad_up(HexView* prView) {
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prPad = get_hexpad(prView);
    if (prPad->visible == 1) {
        if (hexpad_cursor_up(prPad) < 0) {
            return HEXVIEW_FAILURE;
        }
        return HEXVIEW_SUCCESS;
    }
    if (cursor_up(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_init(HexView* prView) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prConfig = get_config(prView);
    if (panelconfig_init(prConfig) < 0) {
        return HEXVIEW_FAILURE;
    }
    prCursor = get_cursorpos(prView);
    if (cursorpos_init(prCursor) < 0) {
        return HEXVIEW_FAILURE;
    }
    prPad = get_hexpad(prView);
    if (hexpad_init(prPad) < 0) {
        return HEXVIEW_FAILURE;
    }
    
    prView->minOffset = HEXVIEW_MINOFFSET;
    prView->maxOffset = HEXVIEW_MAXOFFSET;
    prView->offset = prView->minOffset;
    prView->columns = HEXVIEW_COLUMNS;
    prView->editing = 0;
    prView->dirty = 1;
    
    return HEXVIEW_SUCCESS;    
}

int hexview_page_down(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_page_up(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_redraw(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_seek(HexView* prView, SceUInt32 address) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    if ((address < prView->minOffset) || (address >= prView->maxOffset)) {
        return HEXVIEW_BADADDR;
    }
    prView->offset = address;
    prView->dirty = 1;
    if (hexview_set_cursorpos(prView, 0, 0) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_set_cursorcolor(HexView* prView,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prColor = get_cursorcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return HEXVIEW_FAILURE;
    }
    prPad = get_hexpad(prView);
    if (hexpad_set_cursorcolor(prPad, background, text) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_set_panelcolor(HexView* prView,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    HexPad* prPad = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prColor = get_panelcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return HEXVIEW_FAILURE;
    }
    prPad = get_hexpad(prView);
    if (hexpad_set_panelcolor(prPad, background, text) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_set_cursorpos(HexView* prView, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prCursor = get_cursorpos(prView);
    if (cursorpos_set(prCursor, x, y) < 0) {
        return HEXVIEW_FAILURE;
    }
    prView->dirty = 1;
    return HEXVIEW_SUCCESS;
}

int hexview_set_position(HexView* prView, const int x, const int y) {
    CursorPos* prPos = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prPos = get_position(prView);
    if (cursorpos_set(prPos, x, y) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

int hexview_set_size(HexView* prView, const int width, const int height) {
    Dimension* prSize = NULL;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    prSize = get_size(prView);
    if (dimension_set(prSize, width, height) < 0) {
        return HEXVIEW_FAILURE;
    }
    return HEXVIEW_SUCCESS;
}

static int panel_color(HexView* prView) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    pspDebugScreenSetBackColor(prView->panelConfig.rColor.background);
    pspDebugScreenSetTextColor(prView->panelConfig.rColor.text);
    return HEXVIEW_SUCCESS;
}

static int render_address_col(HexView* prView, const int row) {
    SceUInt32 address = 0;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    if (prView->cursorPos.y == row) {
        if (cursor_color(prView) < 0) {
            return HEXVIEW_FAILURE;
        }
    } else {
        if (panel_color(prView) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    address = row_address(prView, row);
    pspDebugScreenKprintf("0x%08X", address);
    return HEXVIEW_SUCCESS;
}

static int render_ascii_col(HexView* prView, const int row) {
    SceChar8* pVal = NULL;
    SceUInt32 address = 0;
    char sLine[17];
    
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    address = row_address(prView, row);
    
    return HEXVIEW_SUCCESS;
}

static int render_row(HexView* prView, const int row) {
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    CursorPos* prCursor = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    SceUInt32 address = 0;
    
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    
    prPcolor = get_panelcolor(prView);
    prCcolor = get_cursorcolor(prView);
    prCursor = get_cursorpos(prView);
    prPos = get_position(prView);
    prSize = get_size(prView);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + row);
    if (row == prCursor->y) {
        if (cursor_color(prView) < 0) {
            return HEXVIEW_FAILURE;
        }
    } else {
        if (panel_color(prView) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    address = row_address(prView, row);
    pspDebugScreenKprintf("0x%08X", address);

    pspDebugScreenSetBackColor(prPcolor->background);
    pspDebugScreenSetTextColor(prPcolor->text);

    return HEXVIEW_SUCCESS;
}

static int render_value_col(HexView* prView, const int row, const int col) {
    SceUInt32 address = 0;
    SceUChar8* prValue = NULL;
    
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    address = row_address(prView, row);
    address += col;
    prValue = (SceUChar8*)address;
    if (panel_color(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    if (prView->cursorPos.y == row) {
        if (prView->cursorPos.x == col) {
            if (cursor_color(prView) < 0) {
                return HEXVIEW_FAILURE;
            }
        }
    }
    pspDebugScreenKprintf("%02X", *prValue);
    return HEXVIEW_SUCCESS;
}

static int render_value_row(HexView* prView, const int row) {
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    switch (prView->columns) {
        case 16:
            if (render_value_row16(prView, row) < 0) {
                return HEXVIEW_FAILURE;
            }
            break;
        case 8:
            if (render_value_row8(prView, row) < 0) {
                return HEXVIEW_FAILURE;
            }
            break;
        case 4:
            if (render_value_row4(prView, row) < 0) {
                return HEXVIEW_FAILURE;
            }
            break;
    }
    return HEXVIEW_SUCCESS;
}

static int render_value_row16(HexView* prView, const int row) {
    int col = 0;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    for (col = 0; col < 4; col++) {
        if (render_value_col(prView, row, col) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    if (panel_color(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    pspDebugScreenPuts(" ");
    for (col = 4; col < 8; col++) {
        if (render_value_col(prView, row, col) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    if (panel_color(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    pspDebugScreenPuts(" ");
    for (col = 8; col < 12; col++) {
        if (render_value_col(prView, row, col) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    if (panel_color(prView) < 0) {
        return HEXVIEW_FAILURE;
    }
    pspDebugScreenPuts(" ");
    for (col = 12; col < 16; col++) {
        if (render_value_col(prView, row, col) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    return HEXVIEW_SUCCESS;
}

static int render_value_row4(HexView* prView, const int row) {
    int col = 0;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    for (col = 0; col < 4; col++) {
        if (render_value_col(prView, row, col) < 0) {
            return HEXVIEW_FAILURE;
        }
        if (col < 3) {
            if (panel_color(prView) < 0) {
                return HEXVIEW_FAILURE;
            }
            pspDebugScreenPuts("        ");
        }
    }
    return HEXVIEW_SUCCESS;
}

static int render_value_row8(HexView* prView, const int row) {
    int col = 0;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    for (col = 0; col < 8; col++) {
        if (render_value_col(prView, row, col) < 0) {
            return HEXVIEW_FAILURE;
        }
        if (col < 7) {
            if (panel_color(prView) < 0) {
                return HEXVIEW_FAILURE;
            }
            pspDebugScreenPuts("   ");
        }
    }
    return HEXVIEW_SUCCESS;
}

static SceUInt32 row_address(HexView* prView, const int row) {
    SceUInt32 address = 0;
    if (prView != NULL) {
        address = prView->offset + (row * prView->columns);
    }
    return address;
}

static int scroll_down(HexView* prView, const int rows) {
    int memsz = 0;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    memsz = rows * prView->columns;
    return HEXVIEW_SUCCESS;
}

static int scroll_up(HexView* prView, const int rows) {
    int memsz = 0;
    SceUInt32 address = 0;
    if (prView == NULL) {
        return HEXVIEW_NULLPTR;
    }
    memsz = rows * prView->columns;
    address = prView->offset - memsz;
    if (address >= prView->minOffset) {
        if (hexview_seek(prView, address) < 0) {
            return HEXVIEW_FAILURE;
        }
    }
    return HEXVIEW_SUCCESS;
}
