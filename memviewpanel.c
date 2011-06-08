#include "memviewpanel.h"

static int attempt_jump(MemViewPanel* prPanel);
static int attempt_return(MemViewPanel* prPanel);
static int hexpad_hide(MemViewPanel* prPanel);
static int hexpad_show(MemViewPanel* prPanel);
static int render_address_col(MemViewPanel* prPanel, const int row);
static int render_comment_col(MemViewPanel* prPanel, const int row);
static int render_row(MemViewPanel* prPanel, const int row);
static int render_value_col(MemViewPanel* prPanel, const int row);

/** Return the address of the indicated row.
 * 
 * @param prPanel Pointer to a MemViewPanel struct representing the Memory
 * View Panel.
 * @param row int indicating the row to return the address of.
 * @return A SceUInt32 is returned.
 */
static SceUInt32 row_address(MemViewPanel* prPanel, const int row);

/** Return a pointer to a ColorConfig struct representing the current Color
 * Configuration for the indicated row.
 * 
 * @param prPanel Pointer to a MemViewPanel struct representing the Memory
 * View Panel.
 * @param row int indicating the row the return the address of.
 * @return A pointer to a ColorConfig struct or NULL is returned.
 */
static ColorConfig* row_color(MemViewPanel* prPanel, const int row);

/** Return the destination address of the indicated row.  The destination
 * address is determined by interpreting the value of the indicated row as a
 * MIPS32 instruction, then determining the destination of that instruction.
 * If no destination can be determined, 0 is returned.
 * 
 * @param prPanel Pointer to a MemViewPanel struct representing the Memory
 * View Panel.
 * @param row int indicating the row to return the destination of.
 * @return A SceUInt32 containing the destination or 0 is returned.
 */
static SceUInt32 row_destination(MemViewPanel* prPanel, const int row);

/** Return an EValueType value denoting the type of value in the indicated row.
 * 
 * @param prPanel Pointer to a MemViewPanel struct representing the Memory
 * View Panel.
 * @return An EValueType value is returned.
 */
static EValueType row_type(MemViewPanel* prPanel, const int row);

/** Return a SceInt32 containing the value of memory at the address of the
 * indicated row.
 * 
 * @param prPanel Pointer to a MemViewPanel struct representing the Memory
 * View Panel.
 * @param row int indicating the row to return the value of.
 * @return A SceUInt32 value indicating the value of memory.
 */
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

static int edit_value(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    SceUInt32 value = 0;
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    prCursor = memviewpanel_get_cursorpos(prPanel);
    value = row_value(prPanel, prCursor->y);
    if (hexpad_set_value(prPad, value) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (hexpad_show(prPanel) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    prPanel->editing = 1;
    prPanel->editMode = EM_UInt32;
    return MEMVIEWPANEL_SUCCESS;
}

static int hexpad_hide(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    SceUInt32 address = 0;
    SceUInt32 value = 0;
    SceUInt32* pMem = NULL;
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prCursor = memviewpanel_get_cursorpos(prPanel);
    address = row_address(prPanel, prCursor->y);
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPanel->editing == 1) {
        if (prPanel->editMode == EM_Address) {
            if (prPad->cancelled == 0) {
                value = hexpad_get_value(prPad);
                if ((value < prPanel->minOffset) || 
                        (value >= prPanel->maxOffset)) {
                    return MEMVIEWPANEL_BADADDR;
                }
                if (memviewpanel_seek(prPanel, value) < 0) {
                    return MEMVIEWPANEL_FAILURE;
                }
            }
        }
        if (prPanel->editMode == EM_UInt32) {
            if (prPad->cancelled == 0) {
                value = hexpad_get_value(prPad);
                pMem = (SceUInt32*)(address | 0x40000000);
                *pMem = value;
                sceKernelDcacheWritebackInvalidateRange(pMem, 4);
                sceKernelIcacheInvalidateRange(pMem, 4);
            }
        }
    }
    if(memviewpanel_invalidate(prPanel) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    prPad->visible = 0;
    return MEMVIEWPANEL_SUCCESS;
}

static int hexpad_show(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        return MEMVIEWPANEL_SUCCESS;
    }
    if (hexpad_set_position(prPad, 4, 4) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    prPad->digit = 0;
    prPad->visible = 1;
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_analog(MemViewPanel* prPanel, unsigned char x, unsigned char y)
{
    int iy = 0;
    int dy = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    
    /* The deflections are returned as a value between 0 and 255 with 127 being
     * the center of the stick.  In order to translate that into an integer
     * deflection indicating the distance past center of the stick, subtract
     * 127 from the current deflection.
     */
    iy = y - 128;
    /* I want 8 levels of deflection with the first level being deadzone. */
    dy = (int)(iy / 16);
    if (dy < -1) {
        if (memviewpanel_scroll_up(prPanel, (dy * -1) - 1) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    if (dy > 1) {
        if (memviewpanel_scroll_down(prPanel, dy - 1) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_button_circle(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_circle(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_button_cross(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_cross(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        if (edit_value(prPanel) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_button_ltrigger(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_prev_digit(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;    
}

int memviewpanel_button_rtrigger(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_next_digit(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;    
}

int memviewpanel_button_square(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_square(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_button_triangle(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_triangle(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        if (memviewpanel_jump_to(prPanel) < 0) {
            return MEMVIEWPANEL_FAILURE;
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

int memviewpanel_dpad_down(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_down(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        if (memviewpanel_cursor_down(prPanel) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_dpad_left(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_left(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        if (memviewpanel_cursor_left(prPanel) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_dpad_right(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_right(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        if (memviewpanel_cursor_right(prPanel) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_dpad_up(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_up(prPad) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    } else {
        if (memviewpanel_cursor_up(prPanel) < 0) {
            return MEMVIEWPANEL_FAILURE;
        }
    }
    return MEMVIEWPANEL_SUCCESS;
}

PanelConfig* memviewpanel_get_config(MemViewPanel* prPanel) {
    if (prPanel != NULL) {
        return &prPanel->panelConfig;
    }
    return NULL;
}

ColorConfig* memviewpanel_get_cursorcolor(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        return panelconfig_get_cursorcolor(prConfig);
    }
    
    return NULL;
}

CursorPos* memviewpanel_get_cursorpos(MemViewPanel* prPanel) {
    if (prPanel != NULL) {
        return &prPanel->cursorPos;
    }
    return NULL;
}

GGame* memviewpanel_get_game(MemViewPanel* prPanel) {
    if (prPanel == NULL) {
        return NULL;
    }
    return prPanel->game;
}

HexPad* memviewpanel_get_hexpad(MemViewPanel* prPanel) {
    if (prPanel != NULL) {
        return &prPanel->hexPad;
    }
    return NULL;
}

JumpStack* memviewpanel_get_jumpstack(MemViewPanel* prPanel) {
    if (prPanel != NULL) {
        return &prPanel->jumpStack;
    }
    return NULL;
}

ColorConfig* memviewpanel_get_padcolor(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    if (prPanel != NULL) {
        prPad = memviewpanel_get_hexpad(prPanel);
        return hexpad_get_panelcolor(prPad);
    }
    return NULL;
}

ColorConfig* memviewpanel_get_panelcolor(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        return panelconfig_get_panelcolor(prConfig);
    }
    return NULL;
}

CursorPos* memviewpanel_get_position(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        return panelconfig_get_position(prConfig);
    }
    return NULL;
}

SceUInt32 memviewpanel_get_seladdr(MemViewPanel* prPanel) {
    CursorPos* prCursor = NULL;
    SceUInt32 address = 0;
    if (prPanel != NULL) {
        prCursor = memviewpanel_get_cursorpos(prPanel);
        address = prPanel->offset + (prCursor->y * 4);
    }
    return address;
}

Dimension* memviewpanel_get_size(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = memviewpanel_get_config(prPanel);
        return panelconfig_get_size(prConfig);
    }
    return NULL;
}

int memviewpanel_init(MemViewPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    JumpStack* prStack = NULL;
    HexPad* prPad = NULL;
    
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
    
    prPad = memviewpanel_get_hexpad(prPanel);
    if (hexpad_init(prPad) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    
    prPanel->game = NULL;
    
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_invalidate(MemViewPanel* prPanel) {
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPanel->dirty = 1;
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_jump_to(MemViewPanel* prPanel) {
    HexPad* prPad = NULL;
    CursorPos* prCursor = NULL;
    SceUInt32 address = 0;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    prCursor = memviewpanel_get_cursorpos(prPanel);
    address = memviewpanel_get_seladdr(prPanel);
    /* display the current marked address in previously called hexpad */
    if (hexpad_set_value(prPad, address) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    if (hexpad_show(prPanel) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    prPanel->editing = 1;
    prPanel->editMode = EM_Address;
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
    HexPad* prPad = NULL;
    int row = 0;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    if (prPanel->editing == 1) {
        prPad = memviewpanel_get_hexpad(prPanel);
        if (prPad->visible == 1) {
            if (hexpad_redraw(prPad) < 0) {
                return MEMVIEWPANEL_FAILURE;
            }
        } else {
            if (hexpad_hide(prPanel) < 0) {
                return MEMVIEWPANEL_FAILURE;
            }
            prPanel->editing = 0;
        }
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
    HexPad* prPad = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prColor = memviewpanel_get_cursorcolor(prPanel);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    prPad = memviewpanel_get_hexpad(prPanel);
    if (hexpad_set_cursorcolor(prPad, background, text) < 0) {
        return MEMVIEWPANEL_FAILURE;
    }
    return MEMVIEWPANEL_SUCCESS;
}

int memviewpanel_set_padcolor(MemViewPanel* prPanel,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prColor = memviewpanel_get_padcolor(prPanel);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
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
    if (colorconfig_setcolor(prColor, background, text) < 0) {
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
    GGame* prGame = NULL;
    Glabel* prLabel = NULL;
    char sFmt[10];
    char sComment[70];
    int cols = 0;
    int opcode = 0;
    SceUInt32 jdest = 0;
    
    if (prPanel == NULL) {
        return MEMVIEWPANEL_NULLPTR;
    }
    prGame = memviewpanel_get_game(prPanel);
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
            if (prGame != NULL) {
                prLabel = ggame_find_label(prGame, value);
                if (prLabel != NULL) {
                    sprintf(sComment, "Pointer (%s)", prLabel->text);
                    pspDebugScreenKprintf(sFmt, sComment);
                    break;
                }
            }
            sprintf(sComment, "Pointer (0x%08X)", value);
            pspDebugScreenKprintf(sFmt, sComment);
            break;
        default:
            opcode = mipsGetOpCode(value);
            if ((opcode == 0x03) || (opcode == 0x02)) {
                jdest = mipsGetJumpDestination(value, address);
                prLabel = ggame_find_label(prGame, jdest);
                if (prLabel != NULL) {
                    if (opcode == 0x02) {
                        sprintf(sComment, "J %s", prLabel->text);
                    }
                    if (opcode == 0x03) {
                        sprintf(sComment, "JAL %s", prLabel->text);
                    }
                    pspDebugScreenKprintf(sFmt, sComment);
                    return MEMVIEWPANEL_SUCCESS;
                }
            }
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
    CursorPos* prCursor = NULL;
    if (prPanel != NULL) {
        prCursor = memviewpanel_get_cursorpos(prPanel);
        if (prCursor->y == row) {
            return memviewpanel_get_cursorcolor(prPanel);
        } else {
            return memviewpanel_get_panelcolor(prPanel);
        }
    }
    return NULL;
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
    SceUInt32 value = 0;
    SceUInt32 address = 0;
    
    if (prPanel != NULL) {
        value = row_value(prPanel, row);
        if ((value >= prPanel->minOffset) && (value < prPanel->maxOffset)) {
            return VT_Pointer;
        }
        address = row_address(prPanel, row);
    }
    return VT_None;
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
