#include "disassembler.h"
#include "geelog.h"

static int render_status(Disassembler* prDasm);
static int update_ggame(Disassembler* prDasm);

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

GGame* disassembler_get_game(Disassembler* prDasm) {
    if (prDasm == NULL) {
        return NULL;
    }
    return prDasm->game;
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
    prDasm->gameLoaded = 0;
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
    if (prDasm->gameLoaded == 0) {
        update_ggame(prDasm);
    }
    prMemView = disassembler_get_memview(prDasm);
    if (memviewpanel_redraw(prMemView) != MEMVIEWPANEL_SUCCESS) {
        return DISASSEMBLER_FAILURE;
    }
    if (render_status(prDasm) < 0) {
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
    render_status(prDasm);
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

static int render_status(Disassembler* prDasm) {
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    MemViewPanel* prPanel = NULL;
    ColorConfig* prColor = NULL;
    GGame* prGame = NULL;
    GsegMap* prMap = NULL;
    Gsegment* prSeg = NULL;
    GlabelMap* prLmap = NULL;
    Glabel* prLabel = NULL;
    SceUInt32 address = 0;
    char sFmt[10];
    char sLine[71];
    int x = 0;
    int y = 0;
    
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prPanel = disassembler_get_memview(prDasm);
    prPos = memviewpanel_get_position(prPanel);
    prSize = memviewpanel_get_size(prPanel);
    x = prPos->x;
    y = prPos->y + prSize->height;
    pspDebugScreenSetXY(x, y);
    prColor = disassembler_get_statuscolor(prDasm);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    sprintf(sFmt, "%%-%ds", prSize->width);
    prGame = disassembler_get_game(prDasm);
    if (prGame != NULL) {
        prMap = ggame_get_segmap(prGame);
        prLmap = ggame_get_labelmap(prGame);
        address = memviewpanel_get_seladdr(prPanel);
        prSeg = gsegmap_find(prMap, address);
        prLabel = glabelmap_find(prLmap, address);
        if ((prLabel != NULL) && (prSeg != NULL)) {
            sprintf(sLine, "Segment: %s Label: %s", prSeg->name, prLabel->text);
            pspDebugScreenKprintf(sFmt, sLine);
            return DISASSEMBLER_SUCCESS;
        }
        if (prSeg != NULL) {
            sprintf(sLine, "Segment: %s", prSeg->name);
            pspDebugScreenKprintf(sFmt, sLine);
            return DISASSEMBLER_SUCCESS;
        }
        if (prLabel != NULL) {
            sprintf(sLine, "Label: %s", prLabel->text);
            pspDebugScreenKprintf(sFmt, sLine);
            return DISASSEMBLER_SUCCESS;
        }
    }
    pspDebugScreenKprintf(sFmt, "");
    return DISASSEMBLER_SUCCESS;
}

static int update_ggame(Disassembler* prDasm) {
    const char* sFunc = "update_ggame";
    GGame* prGame = NULL;
    SceModule* prModule = NULL;
    SceUInt32 address = 0;
    if (prDasm == NULL) {
        return DISASSEMBLER_NULLPTR;
    }
    prGame = disassembler_get_game(prDasm);
    geelog_flog(LOG_DEBUG, sFunc, "Getting Game Module.");
    prModule = ggame_get_module(prGame);
    geelog_flog(LOG_DEBUG, sFunc, "Loading Game Module.");
    if (ggame_load_module(prGame) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    geelog_flog(LOG_DEBUG, sFunc, "Locating module_start.");
    address = ggame_get_module_start(prGame);
    if (address == 0) {
        geelog_flog(LOG_DEBUG, sFunc, "Failed to locate module_start.");
        return DISASSEMBLER_FAILURE;
    }
    if (disassembler_seek(prDasm, address) < 0) {
        return DISASSEMBLER_FAILURE;
    }
    prDasm->gameLoaded = 1;
    geelog_flog(LOG_DEBUG, sFunc, "Game loaded.");
    return DISASSEMBLER_SUCCESS;
}
