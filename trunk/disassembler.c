#include "disassembler.h"

static void drawAddressColumn(Disassembler *prPanel, int iRow);
static void drawAssemblyColumn(Disassembler *prPanel, int iRow);
static void drawCursor(Disassembler *prPanel);
static void drawTable(Disassembler *prPanel);
static void drawTableRow(Disassembler *prPanel, int iRow);
static void drawValueColumn(Disassembler *prPanel, int iRow);
static SceUInt32 getSelectedAddress(Disassembler *prPanel);
static SceUInt32 getSelectedInstructionDest(Disassembler *prPanel);
static SceUInt32 getSelectedOffset(Disassembler *prPanel);
static SceUInt32 getSelectedValue(Disassembler *prPanel);
static SceUInt32 getSelectedValuePointer(Disassembler *prPanel);
static SceUInt32 popAddress(Disassembler *prPanel);
static void pushAddress(Disassembler *prPanel, SceUInt32 addr);

AppletConfig* disassembler_get_appletconfig(Disassembler* prDasm) {
    AppletConfig* prConfig = NULL;
    if (prDasm != NULL) {
        prConfig = prDasm->prApCfg;
    }
    return prConfig;
}

ColorConfig* disassembler_get_cursorcolor(Disassembler* prDasm) {
    ColorConfig* prColor = NULL;
    AppletConfig* prConfig = NULL;
    
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            prColor = appletconfig_get_cursorcolor(prConfig);
        }
    }
    return prColor;
}

CursorPos* disassembler_get_cursorpos(Disassembler* prDasm) {
    CursorPos* prPos = NULL;
    if (prDasm != NULL) {
        prPos = &prDasm->cursor;
    }
    return prPos;
}

MemViewPanel* disassembler_get_memview(Disassembler* prDasm) {
    MemViewPanel* prPanel = NULL;
    if (prDasm != NULL) {
        prPanel = &prDasm->memViewPanel;
    }
    return prPanel;
}

ColorConfig* disassembler_get_panelcolor(Disassembler* prDasm) {
    ColorConfig* prColor = NULL;
    AppletConfig* prConfig = NULL;
    
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            prColor = appletconfig_get_panelcolor(prConfig);
        }
    }
    return prColor;
}

CursorPos* disassembler_get_position(Disassembler* prDasm) {
    CursorPos* prPos = NULL;
    AppletConfig *prConfig = NULL;
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            prPos = appletconfig_get_position(prConfig);
        }
    }
    return prPos;
}

Dimension* disassembler_get_size(Disassembler* prDasm) {
    Dimension* prSize = NULL;
    AppletConfig* prConfig = NULL;
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            prSize = appletconfig_get_size(prConfig);
        }
    }
    return prSize;
}

/* commitValueEdit
 *  Write the current value of the value editor to memory.
 * Parameters:
 *  prPanel - Disassembler pointer
 */
static void commitValueEdit(Disassembler *prPanel) {
    SceUInt32 value = 0;
    SceUInt32 offset = 0;
    SceUInt32 *pDest = NULL;
    if (prPanel == NULL) {
        return;
    }
    value = prPanel->value_editor.value;
    offset = getSelectedOffset(prPanel);
    pDest = (SceUInt32 *)offset;
    *pDest = value;
    sceKernelDcacheWritebackInvalidateRange(offset, 4);
    sceKernelIcacheInvalidateRange(offset, 4);
}

void disassembler_attempt_jump(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    SceUInt32 dest = 0;
    SceUInt32 valptr = getSelectedValuePointer(prPanel);
    SceUInt32 insdst = getSelectedInstructionDest(prPanel);
    if (valptr != 0) {
        dest = valptr;
    }
    if (insdst != 0) {
        dest = insdst;
    }
    if (dest == 0) {
        return;
    }
    dest -= prPanel->config.base_address;
    dest += prPanel->config.min_offset;
    pushAddress(prPanel, getSelectedOffset(prPanel));
    disassemblerSeek(prPanel, dest);
    prPanel->cursor.y = 0;
    prPanel->dirty = 1;
}

void disassemblerAttemptReturn(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->cur_jump <= 0) {
        return;
    }
    SceUInt32 dest = popAddress(prPanel);
    if (dest != 0) {
        disassemblerSeek(prPanel, dest);
        prPanel->cursor.y = 0;
        prPanel->dirty = 1;
    }
}

void disassembler_circle_button(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 1) {
        if (prPanel->cursor.x == 1) {
            prPanel->value_editor.value = getSelectedValue(prPanel);
            prPanel->value_editor.editing = 0;
            prPanel->editing = 0;
            prPanel->cursordirty = 1;
        }
    }
}

void disassembler_cross_button(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int col = prPanel->cursor.x;
    if (col == 0) {
        dwordeditorCrossButton(&prPanel->address_editor);
        if (prPanel->editing == 0) {
            prPanel->address_editor.value = getSelectedAddress(prPanel);
        }
    } else {
        dwordeditorCrossButton(&prPanel->value_editor);
        if (prPanel->editing == 0) {
            prPanel->value_editor.value = getSelectedValue(prPanel);
        } else {
            commitValueEdit(prPanel);
        }
    }
    prPanel->editing = (prPanel->editing == 1) ? 0 : 1;
    prPanel->cursordirty = 1;
}

void disassembler_cursor_down(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_cursor_down(prMemView);
}

void disassembler_cursor_left(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_cursor_left(prMemView);
}

void disassembler_cursor_right(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_cursor_right(prMemView);
}

void disassembler_cursor_up(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_cursor_up(prMemView);
}

void disassemblerDpadDown(Disassembler *prPanel) {
    int amount;
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        disassembler_cursor_down(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadDown(&prPanel->address_editor);
            amount = dwordeditorIncrementAmount(&prPanel->address_editor);
            disassemblerSeek(prPanel, prPanel->offset - amount);
        } else {
            dwordeditorDpadDown(&prPanel->value_editor);
        }
    }
    prPanel->cursordirty = 1;
}

void disassemblerDpadLeft(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        disassembler_cursor_left(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadLeft(&prPanel->address_editor);
        } else {
            dwordeditorDpadLeft(&prPanel->value_editor);
        }
    }
    prPanel->cursordirty = 1;
}

void disassemblerDpadRight(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        disassembler_cursor_right(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadRight(&prPanel->address_editor);
        } else {
            dwordeditorDpadRight(&prPanel->value_editor);
        }
    }
    prPanel->cursordirty = 1;
}

void disassemblerDpadUp(Disassembler *prPanel) {
    int amount = 0;
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        disassembler_cursor_up(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadUp(&prPanel->address_editor);
            amount = dwordeditorIncrementAmount(&prPanel->address_editor);
            disassemblerSeek(prPanel, prPanel->offset + amount);
        } else {
            dwordeditorDpadUp(&prPanel->value_editor);
        }
    }
    prPanel->cursordirty = 1;
}

int disassembler_init(Disassembler *prPanel, AppletConfig *prApCfg) {
    MemViewPanel* prMemView = NULL;
    ColorConfig* prColor = NULL;
    
    if (prPanel == NULL) {
        return DISASSEMBLER_MEMORY;
    }
    prPanel->prApCfg = prApCfg;
    prPanel->config.address_color.background = DISASSEMBLER_DEFBGCOLOR;
    prPanel->config.address_color.text = DISASSEMBLER_DEFFGCOLOR;
    prPanel->config.base_address = 0x08800000;
    prPanel->config.code_color.background = DISASSEMBLER_DEFBGCOLOR;
    prPanel->config.code_color.text = DISASSEMBLER_DEFFGCOLOR;
    prPanel->config.cursor_color.background = DISASSEMBLER_DEFBGCOLOR;
    prPanel->config.cursor_color.text = DISASSEMBLER_DEFFGCOLOR;
    prPanel->config.cursorrow_color.background = DISASSEMBLER_DEFBGCOLOR;
    prPanel->config.cursorrow_color.text = DISASSEMBLER_DEFFGCOLOR;
    prPanel->config.tablesize.height = 20;
    prPanel->config.max_offset = 0x49FFFFFF;
    prPanel->config.min_offset = 0x48800000;
    prPanel->config.tablepos.y = 7;
    prPanel->config.value_color.background = DISASSEMBLER_DEFBGCOLOR;
    prPanel->config.value_color.text = DISASSEMBLER_DEFFGCOLOR;
    prPanel->offset = 0x48804000;
    prPanel->cursor.x = 0;
    prPanel->cursor.y = 0;
    prPanel->editing = 0;
    dwordeditorInit(&(prPanel->address_editor));
    prPanel->address_editor.increments[7] = 4;
    dwordeditorInit(&(prPanel->value_editor));
    prPanel->dirty = 1;

    AddressColumn* prAddr = &(prPanel->rRow.rAddress);
    addresscolumn_init(prAddr);
    prAddr->color.background = prPanel->config.address_color.background;
    prAddr->color.text = prPanel->config.address_color.text;

    DwordColumn* prValue = &(prPanel->rRow.rValue);
    dwordcolumn_init(prValue);
    prValue->color.background = prPanel->config.value_color.background;
    prValue->color.text = prPanel->config.value_color.text;

    TextColumn* prAssembly = &(prPanel->rRow.rAssembly);
    textcolumn_init(prAssembly);
    prAssembly->prColor = &(prPanel->config.code_color);
    prAssembly->prCursor = &(prPanel->config.cursorrow_color);

    prPanel->prGameInfo = NULL;
    
    prMemView = disassembler_get_memview(prPanel);
    if (memviewpanel_init(prMemView) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    if (memviewpanel_set_size(prMemView, 68, 30) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    if (memviewpanel_set_position(prMemView, 0, 2) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    prColor = disassembler_get_panelcolor(prPanel);
    if (prColor != NULL) {
        if (memviewpanel_set_panelcolor(prMemView, 
                prColor->background, prColor->text) != MEMVIEWPANEL_SUCCESS) {
            return DISASSEMBLER_FAILURE;
        }
    }
    prColor = disassembler_get_cursorcolor(prPanel);
    if (prColor != NULL) {
        if (memviewpanel_set_cursorcolor(prMemView,
                prColor->background, prColor->text) != MEMVIEWPANEL_SUCCESS) {
            return DISASSEMBLER_FAILURE;
        }
    }
    return DISASSEMBLER_SUCCESS;
}

void disassemblerRedraw(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    ColorConfig* prColor = NULL;
    char sFmt[10];
    
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    if (memviewpanel_redraw(prMemView) != MEMVIEWPANEL_SUCCESS) {
        return;
    }
    if (prPanel->dirty == 0) {
        return;
    }
    prPos = disassembler_get_position(prPanel);
    pspDebugScreenSetXY(prPos->x, prPos->y + 1);
    prColor = disassembler_get_panelcolor(prPanel);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    prSize = disassembler_get_size(prPanel);
    sprintf(sFmt, "%%-%ds", prSize->width);
    pspDebugScreenKprintf(sFmt, "Disassembler");
    memviewpanel_invalidate(prMemView);
    prPanel->dirty = 0;
}

void disassemblerPageDown(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_page_down(prMemView);
}

void disassemblerPageUp(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_page_up(prMemView);
}

void disassemblerScrollDown(Disassembler *prPanel) {
    /* Scrolling handled through MemViewPanel now. */
}

void disassemblerScrollUp(Disassembler *prPanel) {
    /* Scrolling handled through MemViewPanel now. */
}

int disassemblerSeek(Disassembler *prPanel, SceUInt32 offset) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prPanel);
    if (memviewpanel_seek(prMemView, offset) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

void disassemblerSquareButton(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
}

SceUInt32 disassemblerTell(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    SceUInt32 pos = 0;
    if (prPanel != NULL) {
        prMemView = disassembler_get_memview(prPanel);
        pos = prMemView->offset;
    }
    return pos;
}

void disassemblerTriangleButton(Disassembler *prPanel) {
    if (prPanel == NULL) {
        return;
    }
}

static void drawCursor(Disassembler *prPanel) {
    DwordEditor* prEditor = NULL;
    int iRow = 0;
    int iCol = 0;
    int tt = 0;
    
    if (prPanel == NULL) {
        return;
    }
    iRow = prPanel->cursor.y;
    iCol = prPanel->cursor.x;
    SceUInt32 moff = prPanel->offset - prPanel->config.min_offset;
    SceUInt32 base = prPanel->config.base_address;
    SceUInt32 vaddr = base + moff + (iRow * 4);
    SceUInt32 *pVal = (SceUInt32 *)(prPanel->offset + (iRow * 4));
    tt = prPanel->config.tablepos.y;
    if (iCol == 0) {
        prEditor = &prPanel->address_editor;
        pspDebugScreenSetXY(0, tt + iRow);
        if (prPanel->editing == 0) {
            prEditor->value = vaddr;
        }
        dwordeditorRedraw(prEditor);
    }
    if (iCol == 1) {
        prEditor = &prPanel->value_editor;
        pspDebugScreenSetXY(11, tt + iRow);
        if (prPanel->editing == 0) {
            prEditor->value = *pVal;
        }
        dwordeditorRedraw(prEditor);
    }
    prPanel->cursordirty = 0;
}

static void drawValueColumn(Disassembler *prPanel, int iRow) {
}

static SceUInt32 getSelectedAddress(Disassembler *prPanel) {
    SceUInt32 addr = 0;
    return addr;
}

static SceUInt32 getSelectedInstructionDest(Disassembler *prPanel) {
    SceUInt32 dest = 0;
    return dest;
}

static SceUInt32 getSelectedOffset(Disassembler *prPanel) {
    SceUInt32 offset = 0;
    return offset;
}

static SceUInt32 getSelectedValue(Disassembler *prPanel) {
    SceUInt32 value = 0;
    return value;
}

static SceUInt32 getSelectedValuePointer(Disassembler *prPanel) {
    SceUInt32 dest = 0;
    return dest;
}

static SceUInt32 popAddress(Disassembler *prPanel) {
    SceUInt32 addr = 0;
    return addr;
}

static void pushAddress(Disassembler *prPanel, SceUInt32 addr) {
}
