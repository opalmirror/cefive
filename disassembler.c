#include "disassembler.h"

int disassembler_button_circle(Disassembler* prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_button_circle(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_button_cross(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_button_cross(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_button_ltrigger(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_button_ltrigger(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_button_rtrigger(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_button_rtrigger(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_button_square(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_button_square(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_button_triangle(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_button_triangle(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_dpad_down(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_dpad_down(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_dpad_left(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_dpad_left(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_dpad_right(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_dpad_right(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_dpad_up(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_dpad_up(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
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

int disassembler_init(Disassembler *prDasm, AppletConfig *prApCfg) {
    MemViewPanel* prMemView = NULL;
    ColorConfig* prColor = NULL;
    
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prDasm->prApCfg = prApCfg;
    prDasm->editing = 0;
    prDasm->dirty = 1;

    prDasm->prGameInfo = NULL;
    
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_init(prMemView) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    if (memviewpanel_set_size(prMemView, 68, 30) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    if (memviewpanel_set_position(prMemView, 0, 2) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    prColor = disassembler_get_panelcolor(prDasm);
    if (prColor != NULL) {
        if (memviewpanel_set_panelcolor(prMemView, 
                prColor->background, prColor->text) != MEMVIEWPANEL_SUCCESS) {
            return DISASSEMBLER_FAILURE;
        }
    }
    prColor = disassembler_get_cursorcolor(prDasm);
    if (prColor != NULL) {
        if (memviewpanel_set_cursorcolor(prMemView,
                prColor->background, prColor->text) != MEMVIEWPANEL_SUCCESS) {
            return DISASSEMBLER_FAILURE;
        }
    }
    prColor = disassembler_get_statuscolor(prDasm);
    if (prColor != NULL) {
        if (memviewpanel_set_padcolor(prMemView, 
                prColor->background, prColor->text) < 0) {
            return DISASSEMBLER_FAILURE;
        }
    }
    
    return DISASSEMBLER_SUCCESS;
}

int disassembler_is_editing(Disassembler *prDasm) {
    int editing = 0;
    MemViewPanel* prMemView = NULL;
    if (prDasm != NULL) {
        prMemView = disassembler_get_memview(prDasm);
        editing = (prMemView->editing == 1);
    }
    return editing;
}

int disassembler_page_down(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_page_down(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_page_up(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_page_up(prMemView) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

int disassembler_redraw(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    ColorConfig* prColor = NULL;
    char sFmt[10];
    
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_redraw(prMemView) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    if (prDasm->dirty == 0) {
        return DISASSEMBLER_SUCCESS;
    }
    prPos = disassembler_get_position(prDasm);
    pspDebugScreenSetXY(prPos->x, prPos->y + 1);
    prColor = disassembler_get_panelcolor(prDasm);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    prSize = disassembler_get_size(prDasm);
    sprintf(sFmt, "%%-%ds", prSize->width);
    pspDebugScreenKprintf(sFmt, "Disassembler");
    memviewpanel_invalidate(prMemView);
    prDasm->dirty = 0;
    return DISASSEMBLER_SUCCESS;
}

int disassembler_seek(Disassembler *prDasm, SceUInt32 offset) {
    MemViewPanel* prMemView = NULL;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_seek(prMemView, offset) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    return DISASSEMBLER_SUCCESS;
}

SceUInt32 disassembler_tell(Disassembler *prDasm) {
    MemViewPanel* prMemView = NULL;
    SceUInt32 pos = 0;
    if (prDasm != NULL) {
        prMemView = disassembler_get_memview(prDasm);
        pos = prMemView->offset;
    }
    return pos;
}
