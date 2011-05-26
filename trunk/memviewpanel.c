#include "memviewpanel.h"

static int attempt_jump(MemViewPanel* prPanel);
static int attempt_return(MemViewPanel* prPanel);
static int render_address_col(MemViewPanel* prPanel, const int row);
static int render_comment_col(MemViewPanel* prPanel, const int row);
static int render_row(MemViewPanel* prPanel, const int row);
static int render_value_col(MemViewPanel* prPanel, const int row);
static SceUInt32 row_address(MemViewPanel* prPanel, const int row);
static ColorConfig* row_color(MemViewPanel* prPanel, const int row);
static SceUInt32 row_destination(MemViewPanel* prPanel, const int row);
static EValueType row_type(MemViewPanel* prPanel, const int row);
static SceUInt32 row_value(MemViewPanel* prPanel, const int row);

static int attempt_jump(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    JumpStack* prStack = NULL;
    EValueType rType = VT_None;
    SceUInt32 value = 0;
    SceUInt32 address = 0;
    int row = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prStack = memviewpanel_get_jumpstack(prPanel);
    prCursor = memviewpanel_get_cursorpos(prPanel);
    row = prCursor->y;
    rType = row_type(prPanel, row);
    address = row_address(prPanel, row);
    if (rType == VT_Pointer) {
        value = row_value(prPanel, row);
        if (jumpstack_push(prStack, address) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
        if (memviewpanel_seek(prPanel, value) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        value = row_destination(prPanel, row);
        if (value != 0) {
            if (jumpstack_push(prStack, address) < 0) {
                return MEMVIEWPANEL_FAILURE;
            }
            if (memviewpanel_seek(prPanel, value) < 0) {
                return MEMVIEWPANEL_FAILURE;
            }
        }
    }
    
    return MEMVIEWPANEL_SUCCESS;
}

static int attempt_return(MemViewPanel* prPanel) {
    JumpStack *prStack = NULL;
    SceUInt32 address = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prStack = memviewpanel_get_jumpstack(prPanel);
    if (prStack->elements > 0) {
        address = jumpstack_pop(prStack);
        if (address != 0) {
            if (memviewpanel_seek(prPanel, address) < 0) {
                return MEMVIEWPANEL_FAILURE;
            }
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_cursor_down(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    Dimension* prSize = NULL;
    int y = 0;
    int oldy = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prCursor = memviewpanel_get_cursorpos(prPanel);
    prSize = memviewpanel_get_size(prPanel);
    y = prCursor->y;
    y++;
    if (y >= prSize->height) {
        if (memviewpanel_scroll_down(prPanel, 1) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
        y = prSize->height - 1;
    }
    oldy = prCursor->y;
    prCursor->y = y;
    if (oldy != y) {
        if (render_row(prPanel, oldy) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
        if (render_row(prPanel, prCursor->y) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_cursor_left(MemViewPanel* prPanel) {
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    if (attempt_return(prPanel) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_cursor_right(MemViewPanel* prPanel) {
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    if (attempt_jump(prPanel) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_cursor_up(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    int y = 0;
    int oldy = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prCursor = memviewpanel_get_cursorpos(prPanel);
    y = prCursor->y;
    y--;
    if (y < 0) {
        if (memviewpanel_scroll_up(prPanel, 1) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
        y = 0;
    }
    oldy = prCursor->y;
    prCursor->y = y;
    if (oldy != y) {
        if (render_row(prPanel, oldy) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
        if (render_row(prPanel, prCursor->y) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

PanelConfig* memviewpanel_get_config(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = &prPanel->panelConfig;
    }
    return prConfig;
}

ColorConfig* memviewpanel_get_cursorcolor(MemViewPanel* prPanel) {
    ColorConfig* prColor = NULL;
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        prColor = panelconfig_get_cursorcolor(prConfig);
    }
    
    return prColor;
}

CursorPos* memviewpanel_get_cursorpos(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    if (prPanel != NULL) {
        prCursor = &prPanel->cursorPos;
    }
    return prCursor;
}

JumpStack* memviewpanel_get_jumpstack(MemViewPanel* prPanel) {
    JumpStack* prStack = NULL;
    if (prPanel != NULL) {
        prStack = &prPanel->jumpStack;
    }
    return prStack;
}

ColorConfig* memviewpanel_get_panelcolor(MemViewPanel* prPanel) {
    ColorConfig* prColor = NULL;
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        prColor = panelconfig_get_panelcolor(prConfig);
    }
    return prColor;
}

CursorPos* memviewpanel_get_position(MemViewPanel* prPanel) {
    CursorPos* prPos = NULL;
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        prPos = panelconfig_get_position(prConfig);
    }
    return prPos;
}

Dimension* memviewpanel_get_size(MemViewPanel* prPanel) {
    Dimension* prSize = NULL;
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        prSize = panelconfig_get_size(prConfig);
    }
    return prSize;
}

int memviewpanel_init(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    JumpStack* prStack = NULL;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    
    prConfig = memviewpanel_get_config(prPanel);
    if (panelconfig_init(prConfig) != PANELCONFIG_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    prCursor = memviewpanel_get_cursorpos(prPanel);
    if (cursorpos_init(prCursor) != CURSORPOS_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    prStack = memviewpanel_get_jumpstack(prPanel);
    if (jumpstack_init(prStack) != JUMPSTACK_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    prPanel->minOffset = MEMVIEWPANEL_MINOFF;
    prPanel->maxOffset = MEMVIEWPANEL_MAXOFF;
    prPanel->offset = prPanel->minOffset;
    prPanel->pointerColor = MEMVIEWPANEL_PTRCOLOR;
    
    if (memviewpanel_invalidate(prPanel) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_invalidate(MemViewPanel* prPanel) {
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPanel->dirty = 1;
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_page_down(MemViewPanel* prPanel) {
    Dimension* prSize = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prSize = memviewpanel_get_size(prPanel);
    if (memviewpanel_scroll_down(
            prPanel, prSize->height) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_page_up(MemViewPanel* prPanel) {
    Dimension* prSize = NULL;
    SceUInt32 pgAmt = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prSize = memviewpanel_get_size(prPanel);
    /* Compute the number of bytes per page. */
    pgAmt = prSize->height * 4;
    /* If moving by one page would move past minOffset. */
    if (prPanel->offset - pgAmt < prPanel->minOffset) {
        /* Move to minOffset instead. */
        if (memviewpanel_seek(
                prPanel, prPanel->minOffset) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
        return MEMVIEWPANEL_SUCCESS;
    }
    if (memviewpanel_scroll_up(
            prPanel, prSize->height) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_redraw(MemViewPanel* prPanel) {
    Dimension* prSize = NULL;
    int row = 0;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    if (prPanel->dirty == 0) {
        return MEMVIEWPANEL_SUCCESS;
    }
    prSize = memviewpanel_get_size(prPanel);
    for (row = 0; row < prSize->height; row++) {
        if (render_row(prPanel, row) != MEMVIEWPANEL_SUCCESS) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    prPanel->dirty = 0;
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_scroll_down(MemViewPanel* prPanel, const int rows) {
    SceUInt32 address = 0;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    address = prPanel->offset;
    address += (rows * 4);
    if (memviewpanel_seek(prPanel, address) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_scroll_up(MemViewPanel* prPanel, const int rows) {
    SceUInt32 address = 0;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    address = prPanel->offset;
    address -= (rows * 4);
    if (memviewpanel_seek(prPanel, address) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_seek(MemViewPanel* prPanel, SceUInt32 address) {
    Dimension* prSize = NULL;
    CursorPos* prCursor = NULL;
    int bpp = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    if (address < prPanel->minOffset) {
        return MEMVIEWPANEL_BADADDR;
    }
    prSize = memviewpanel_get_size(prPanel);
    bpp = prSize->height * 4;
    if (address >= prPanel->maxOffset - bpp) {
        return MEMVIEWPANEL_BADADDR;
    }
    prPanel->offset = address;
    prCursor = memviewpanel_get_cursorpos(prPanel);
    if (cursorpos_set(prCursor, 0, 0) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (memviewpanel_invalidate(prPanel) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_set_cursorcolor(MemViewPanel* prPanel,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prColor = memviewpanel_get_cursorcolor(prPanel);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_set_panelcolor(MemViewPanel* prPanel, 
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prColor = memviewpanel_get_panelcolor(prPanel);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_set_position(MemViewPanel* prPanel, const int x, 
            const int y) {
    CursorPos* prPos = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPos = memviewpanel_get_position(prPanel);
    if (prPos == NULL) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (cursorpos_set(prPos, x, y) != CURSORPOS_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_set_size(MemViewPanel* prPanel, const int width, 
        const int height) {
    Dimension* prSize = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prSize = memviewpanel_get_size(prPanel);
    if (prSize == NULL) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (dimension_set(prSize, width, height) != DIMENSION_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

static int render_address_col(MemViewPanel* prPanel, const int row) {
    SceUInt32 address = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    address = row_address(prPanel, row);
    pspDebugScreenKprintf("0x%08X  ", address);
    return MEMVIEWPANEL_SUCCESS;
}

static int render_comment_col(MemViewPanel* prPanel, const int row) {
    SceUInt32 address = 0;
    SceUInt32 value = 0;
    Dimension* prSize = NULL;
    ColorConfig* prColor = NULL;
    EValueType rType = VT_None;
    char sFmt[10];
    char sComment[70];
    int cols = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prColor = row_color(prPanel, row);
    prSize = memviewpanel_get_size(prPanel);
    address = row_address(prPanel, row);
    value = row_value(prPanel, row);
    rType = row_type(prPanel, row);
    cols = prSize->width - 24;
    sprintf(sFmt, "%%-%ds", cols);
    pspDebugScreenSetTextColor(prColor->text);
    switch (rType) {
        case VT_Pointer:
            pspDebugScreenKprintf(sFmt, "Pointer");
            break;
        default:
            mipsDecode(sComment, value, address);
            pspDebugScreenKprintf(sFmt, sComment);
    }
    return MEMVIEWPANEL_SUCCESS;
}

static int render_row(MemViewPanel* prPanel, const int row) {
    Dimension* prSize = NULL;
    CursorPos* prCursor = NULL;
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    int x = 0;
    int y = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPos = memviewpanel_get_position(prPanel);
    prSize = memviewpanel_get_size(prPanel);
    if ((row < 0) || (row >= prSize->height)) {
        return MEMVIEWPANEL_INVIDX;
    }
    prColor = row_color(prPanel, row);
    prCursor = memviewpanel_get_cursorpos(prPanel);
    x = prPos->x;
    y = prPos->y + row;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y);
    if (render_address_col(prPanel, row) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (render_value_col(prPanel, row) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (render_comment_col(prPanel, row) != MEMVIEWPANEL_SUCCESS) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

static int render_value_col(MemViewPanel* prPanel, const int row) {
    SceUInt32 value = 0;
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prColor = row_color(prPanel, row);
    value = row_value(prPanel, row);
    pspDebugScreenSetTextColor(prColor->text);
    /* A Value that lies within the bounds of memory is considered a pointer. */
    if ((value >= prPanel->minOffset) && (value < prPanel->maxOffset)) {
        pspDebugScreenSetTextColor(prPanel->pointerColor);
    }
    pspDebugScreenKprintf("0x%08X  ", value);
    return MEMVIEWPANEL_SUCCESS;
}

static SceUInt32 row_address(MemViewPanel* prPanel, const int row) {
    SceUInt32 address = 0;
    if (prPanel != NULL) {
        address = prPanel->offset + (row * 4);
    }
    return address;
}

static ColorConfig* row_color(MemViewPanel* prPanel, const int row) {
    ColorConfig* prColor = NULL;
    CursorPos* prCursor = NULL;
    if (prPanel != NULL) {
        prCursor = memviewpanel_get_cursorpos(prPanel);
        if (prCursor->y == row) {
            prColor = memviewpanel_get_cursorcolor(prPanel);
        } else {
            prColor = memviewpanel_get_panelcolor(prPanel);
        }
    }
    return prColor;
}

static SceUInt32 row_destination(MemViewPanel* prPanel, const int row) {
    SceUInt32 dest = 0;
    SceUInt32 address = 0;
    SceUInt32 value = 0;
    int opcode = 0;
    int func = 0;
    
    if (prPanel != NULL) {
        address = row_address(prPanel, row);
        value = row_value(prPanel, row);
        opcode = mipsGetOpCode(value);
        switch (opcode) {
            case 0x01: /* REGIMM */
                func = mipsGetFunction(value);
                if ((func >= 0 && func < 3) || (func >= 0x10 && func < 0x13)) {
                    dest = (SceUInt32) mipsGetBranchDestination(value, address);
                }
                break;
            case 0x02: /* J */
            case 0x03: /* JAL */
                dest = (SceUInt32) mipsGetJumpDestination(value, address);
                break;
            case 0x04: /* BEQ */
            case 0x05: /* BNE */
            case 0x06: /* BLEZ */
            case 0x07: /* BGTZ */
            case 0x14: /* BEQL */
            case 0x15: /* BNEL */
            case 0x16: /* BLEZL */
            case 0x17: /* BGTZL */
                dest = (SceUInt32) mipsGetBranchDestination(value, address);
                break;
        }
        if ((dest < prPanel->minOffset) || (dest >= prPanel->maxOffset)) {
            dest = 0;
        }
    }
    return dest;
}

static EValueType row_type(MemViewPanel* prPanel, const int row) {
    EValueType rType = VT_None;
    SceUInt32 value = 0;
    if (prPanel != NULL) {
        value = row_value(prPanel, row);
        if ((value >= prPanel->minOffset) && (value < prPanel->maxOffset)) {
            rType = VT_Pointer;
        }
    }
    return rType;
}

static SceUInt32 row_value(MemViewPanel* prPanel, const int row) {
    SceUInt32 value = 0;
    SceUInt32 address = 0;
    SceUInt32* pValue = NULL;
    if (prPanel != NULL) {
        address = row_address(prPanel, row);
        pValue = (SceUInt32*)address;
        value = *pValue;
    }
    return value;
}
