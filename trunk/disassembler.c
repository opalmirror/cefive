#include "disassembler.h"

static void commit_value_edit(Disassembler *prPanel);
static SceUInt32 get_selected_offset(Disassembler *prPanel);

static void commit_value_edit(Disassembler *prPanel) {
    SceUInt32 value = 0;
    SceUInt32 offset = 0;
    SceUInt32 *pDest = NULL;
    if (prPanel == NULL) {
        return;
    }
    //value = prPanel->value_editor.value;
    offset = get_selected_offset(prPanel);
    pDest = (SceUInt32 *)offset;
    *pDest = value;
    sceKernelDcacheWritebackInvalidateRange(offset, 4);
    sceKernelIcacheInvalidateRange(offset, 4);
}

void disassembler_button_circle(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_button_circle(prMemView);
}

void disassembler_button_cross(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_button_cross(prMemView);
}

int disassembler_button_ltrigger(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prPanel);
    if (memviewpanel_button_ltrigger(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_button_rtrigger(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prPanel);
    if (memviewpanel_button_rtrigger(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

void disassembler_button_square(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_button_square(prMemView);
}

void disassembler_button_triangle(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_button_triangle(prMemView);
}

void disassembler_dpad_down(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_dpad_down(prMemView);
}

void disassembler_dpad_left(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_dpad_left(prMemView);
}

void disassembler_dpad_right(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_dpad_right(prMemView);
}

void disassembler_dpad_up(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_dpad_up(prMemView);
}

AppletConfig* disassembler_get_appletconfig(Disassembler* prDasm) {
    if (prDasm != NULL) {
        return prDasm->prApCfg;
    }
    return NULL;
}

ColorConfig* disassembler_get_cursorcolor(Disassembler* prDasm) {
    AppletConfig* prConfig = NULL;
    
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            return appletconfig_get_cursorcolor(prConfig);
        }
    }
    return NULL;
}

MemViewPanel* disassembler_get_memview(Disassembler* prDasm) {
    if (prDasm != NULL) {
        return &prDasm->memViewPanel;
    }
    return NULL;
}

ColorConfig* disassembler_get_panelcolor(Disassembler* prDasm) {
    AppletConfig* prConfig = NULL;
    
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            return appletconfig_get_panelcolor(prConfig);
        }
    }
    return NULL;
}

CursorPos* disassembler_get_position(Disassembler* prDasm) {
    AppletConfig *prConfig = NULL;
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            return appletconfig_get_position(prConfig);
        }
    }
    return NULL;
}

Dimension* disassembler_get_size(Disassembler* prDasm) {
    AppletConfig* prConfig = NULL;
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            return appletconfig_get_size(prConfig);
        }
    }
    return NULL;
}

ColorConfig* disassembler_get_statuscolor(Disassembler* prDasm) {
    AppletConfig* prConfig = NULL;
    if (prDasm != NULL) {
        prConfig = disassembler_get_appletconfig(prDasm);
        if (prConfig != NULL) {
            return appletconfig_get_statuscolor(prConfig);
        }
    }
    return NULL;
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
    prPanel->editing = 0;
    prPanel->dirty = 1;

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
    prColor = disassembler_get_statuscolor(prPanel);
    if (prColor != NULL) {
        if (memviewpanel_set_padcolor(prMemView, 
                prColor->background, prColor->text) < 0) {
            return DISASSEMBLER_FAILURE;
        }
    }
    
    return DISASSEMBLER_SUCCESS;
}

int disassembler_is_editing(Disassembler *prPanel) {
    int editing = 0;
    MemViewPanel* prMemView = NULL;
    if (prPanel != NULL) {
        prMemView = disassembler_get_memview(prPanel);
        editing = (prMemView->editing == 1);
    }
    return editing;
}

void disassembler_page_down(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_page_down(prMemView);
}

void disassembler_page_up(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    if (prPanel == NULL) {
        return;
    }
    prMemView = disassembler_get_memview(prPanel);
    memviewpanel_page_up(prMemView);
}

void disassembler_redraw(Disassembler *prPanel) {
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

int disassembler_seek(Disassembler *prPanel, SceUInt32 offset) {
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

SceUInt32 disassembler_tell(Disassembler *prPanel) {
    MemViewPanel* prMemView = NULL;
    SceUInt32 pos = 0;
    if (prPanel != NULL) {
        prMemView = disassembler_get_memview(prPanel);
        pos = prMemView->offset;
    }
    return pos;
}

static SceUInt32 get_selected_offset(Disassembler *prPanel) {
    SceUInt32 offset = 0;
    return offset;
}
