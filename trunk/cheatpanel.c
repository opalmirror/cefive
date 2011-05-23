#include "cheatpanel.h"

static int render_row(CheatPanel* prPanel, const int row);
static void toggle_constant(CheatPanel *prPanel, int index);
static void toggle_selected(CheatPanel *prPanel, int index);

AppletConfig* cheatpanel_get_appletconfig(CheatPanel* prPanel) {
    AppletConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = prPanel->prApCfg;
    }
    return prConfig;
}

CheatEngine* cheatpanel_get_cheatengine(CheatPanel* prPanel) {
    CheatEngine* prEngine = NULL;
    if (prPanel != NULL) {
        prEngine = prPanel->prEngine;
    }
    return prEngine;
}

CheatPanelConfig* cheatpanel_get_config(CheatPanel* prPanel) {
    CheatPanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = &prPanel->config;
    }
    return prConfig;
}

ColorConfig* cheatpanel_get_constantcolor(CheatPanel* prPanel) {
    ColorConfig* prColor = NULL;
    CheatPanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = cheatpanel_get_config(prPanel);
        if (prConfig != NULL) {
            prColor = &prConfig->constantcolor;
        }
    }
    return prColor;
}

ColorConfig* cheatpanel_get_cursorcolor(CheatPanel* prPanel) {
    ColorConfig* prColor = NULL;
    AppletConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = cheatpanel_get_appletconfig(prPanel);
        if (prConfig != NULL) {
            prColor = appletconfig_get_cursorcolor(prConfig);
        }
    }
    return prColor;
}

CursorPos* cheatpanel_get_cursorpos(CheatPanel* prPanel) {
    CursorPos* prPos = NULL;
    if (prPanel != NULL) {
        prPos = &prPanel->cursor;
    }
    return prPos;
}

ColorConfig* cheatpanel_get_panelcolor(CheatPanel* prPanel) {
    ColorConfig* prColor = NULL;
    AppletConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = cheatpanel_get_appletconfig(prPanel);
        if (prConfig != NULL) {
            prColor = appletconfig_get_panelcolor(prConfig);
        }
    }
    return prColor;
}

ColorConfig* cheatpanel_get_selectedcolor(CheatPanel* prPanel) {
    ColorConfig* prColor = NULL;
    CheatPanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = cheatpanel_get_config(prPanel);
        if (prConfig != NULL) {
            prColor = &prConfig->selectedcolor;
        }
    }
    return prColor;
}

int cheatpanel_invalidate(CheatPanel* prPanel) {
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prPanel->dirty = 1;
    return CHEATPANEL_SUCCESS;
}

int cheatpanel_set_constantcolor(CheatPanel* prPanel, u32 bg, u32 fg) {
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prColor = cheatpanel_get_constantcolor(prPanel);
    if (colorconfig_setcolor(prColor, bg, fg) != COLORCONFIG_SUCCESS) {
        return CHEATPANEL_FAILURE;
    }
    return CHEATPANEL_SUCCESS;
}

int cheatpanel_set_cursorcolor(CheatPanel* prPanel, u32 bg, u32 fg) {
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prColor = cheatpanel_get_cursorcolor(prPanel);
    if (colorconfig_setcolor(prColor, bg, fg) != COLORCONFIG_SUCCESS) {
        return CHEATPANEL_FAILURE;
    }
    return CHEATPANEL_SUCCESS;
}

int cheatpanel_set_cursorpos(CheatPanel* prPanel, int x, int y) {
    CursorPos* prPos = NULL;
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prPos = cheatpanel_get_cursorpos(prPanel);
    if (cursorpos_set(prPos, x, y) != CURSORPOS_SUCCESS) {
        return CHEATPANEL_FAILURE;
    }
    return CHEATPANEL_SUCCESS;
}

int cheatpanel_set_selectedcolor(CheatPanel* prPanel, u32 bg, u32 fg) {
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prColor = cheatpanel_get_selectedcolor(prPanel);
    if (colorconfig_setcolor(prColor, bg, fg) != COLORCONFIG_SUCCESS) {
        return CHEATPANEL_FAILURE;
    }
    return CHEATPANEL_SUCCESS;
}

int cheatpanel_set_tableheight(CheatPanel* prPanel, int height) {
    CheatPanelConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prConfig = cheatpanel_get_config(prPanel);
    prConfig->table_height = height;
    return CHEATPANEL_SUCCESS;
}

void cheatpanel_circle_button(CheatPanel *prPanel) {
}

void cheatpanelCrossButton(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int si = cheatpanelGetSelectedIndex(prPanel);
    if (si == -1) {
        return;
    }
    toggle_selected(prPanel, si);
}

void cheatpanelDpadDown(CheatPanel *prPanel) {
    CheatEngine* prEngine = NULL;
    CursorPos* prCursor = NULL;
    int lpp = 0;
    int y = 0;
    int cc = 0;
    int si = 0;
    int oldy = 0;
    
    if (prPanel == NULL) {
        return;
    }
    prEngine = cheatpanel_get_cheatengine(prPanel);
    if (prEngine == NULL) {
        return;
    }
    prCursor = cheatpanel_get_cursorpos(prPanel);
    lpp = prPanel->config.table_height;
    y = prCursor->y;
    cc = prEngine->cheat_count;
    si = cheatpanelGetSelectedIndex(prPanel);
    if (si < cc) {
        y++;
    }
    if (y >= lpp) {
        cheatpanelScrollDown(prPanel);
        y = lpp - 1;
    }
    if (prCursor->y != y) {
        oldy = prCursor->y;
        prCursor->y = y;
        render_row(prPanel, oldy);
        render_row(prPanel, y);
    }
}

void cheatpanelDpadLeft(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
}

void cheatpanelDpadRight(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
}

void cheatpanelDpadUp(CheatPanel* prPanel) {
    CursorPos* prCursor = NULL;
    int y = 0;
    int oldy = 0;
    
    if (prPanel == NULL) {
        return;
    }
    prCursor = cheatpanel_get_cursorpos(prPanel);
    y = prCursor->y;
    y--;
    if (y < 0) {
        cheatpanelScrollUp(prPanel);
        prCursor->y = 0;
    } else {
        if (y != prCursor->y) {
            oldy = prCursor->y;
            prCursor->y = y;
            render_row(prPanel, oldy);
            render_row(prPanel, y);
        }
    }
}

int cheatpanelGetSelectedIndex(CheatPanel *prPanel) {
    int index = -1;
    index = prPanel->page_position + prPanel->cursor.y;
    return index;
}

void cheatpanelInit(CheatPanel* prPanel, CheatEngine* prEngine) {
    if (prPanel == NULL) {
        return;
    }
    if (prEngine == NULL) {
        return;
    }
    prPanel->prEngine = prEngine;
    prPanel->prApCfg = NULL;
    prPanel->dirty = 1;
}

void cheatpanelRedraw(CheatPanel *prPanel) {
    CheatPanelConfig* prConfig = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCursor = NULL;
    int i;
    int iTop;
    int iSelIndex;

    if (prPanel == NULL) {
        return;
    }
    if (prPanel->dirty == 0) {
        return;
    }
    
    prConfig = cheatpanel_get_config(prPanel);
    prApCfg = cheatpanel_get_appletconfig(prPanel);
    
    iTop = prConfig->top_row;
    iSelIndex = cheatpanelGetSelectedIndex(prPanel);

    pspDebugScreenSetXY(0, iTop);
    if (prApCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prApCfg);
        prCursor = appletconfig_get_cursorcolor(prApCfg);
    } else {
        prColor = &prConfig->color;
        prCursor = &prConfig->cursorrowcolor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("%-67s", "Cheat Panel");
    iTop++;

    for (i = 0; i < prPanel->config.table_height; i++) {
        render_row(prPanel, i);
    }
    prPanel->dirty = 0;
}

void cheatpanelScrollDown(CheatPanel *prPanel) {
    CheatEngine* prEngine = NULL;
    if (prPanel == NULL) {
        return;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return;
    }
    int pp = prPanel->page_position;
    int cc = prEngine->cheat_count;
    int lpp = prPanel->config.table_height;
    if (cc <= lpp) {
        return;
    }
    pp++;
    if (pp + lpp >= cc) {
        return;
    }
    prPanel->page_position = pp;
    cheatpanel_invalidate(prPanel);
}

void cheatpanelScrollUp(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int pp = prPanel->page_position;
    pp--;
    if (pp < 0) {
        pp = 0;
    }
    prPanel->page_position = pp;
    cheatpanel_invalidate(prPanel);
}

void cheatpanelSquareButton(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int si = cheatpanelGetSelectedIndex(prPanel);
    if (si == -1) {
        return;
    }
    toggle_constant(prPanel, si);
}

void cheatpanelTriangleButton(CheatPanel *prPanel) {
}

static void toggle_constant(CheatPanel *prPanel, int index) {
    if (prPanel == NULL) {
        return;
    }
    Cheat *prCheat = cheatengineGetCheat(prPanel->prEngine, index);
    if (cheat_is_constant(prCheat)) {
        cheatengineSetCheatInactive(prPanel->prEngine, index);
    } else {
        cheatengineSetCheatConstant(prPanel->prEngine, index);
    }
    cheatpanel_invalidate(prPanel);
}

static void toggle_selected(CheatPanel *prPanel, int index) {
    if (prPanel == NULL) {
        return;
    }
    Cheat *prCheat = cheatengineGetCheat(prPanel->prEngine, index);
    if (cheat_is_selected(prCheat)) {
        cheatengineSetCheatInactive(prPanel->prEngine, index);
    } else {
        cheatengineSetCheatSelected(prPanel->prEngine, index);
    }
    cheatpanel_invalidate(prPanel);
}

static int render_row(CheatPanel* prPanel, const int row) {
    ColorConfig* prColor = NULL;
    ColorConfig* prHi = NULL;
    CursorPos* prCursor = NULL;
    CheatEngine* prEngine = NULL;
    Cheat* prCheat = NULL;
    CheatPanelConfig* prConfig = NULL;
    int index = 0;
    int y = 0;
    
    if (prPanel == NULL) {
        return CHEATPANEL_NULLPTR;
    }
    prConfig = cheatpanel_get_config(prPanel);
    prEngine = cheatpanel_get_cheatengine(prPanel);
    if (prEngine == NULL) {
        return CHEATPANEL_INVENGINE;
    }
    prCursor = cheatpanel_get_cursorpos(prPanel);
    prColor = cheatpanel_get_panelcolor(prPanel);
    if (prCursor->y == row) {
        prColor = cheatpanel_get_cursorcolor(prPanel);
    }
    index = prPanel->page_position + row;
    y = prConfig->top_row + row + 1;
    if (index >= prEngine->cheat_count) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
        pspDebugScreenSetXY(0, y);
        pspDebugScreenKprintf("%67s", "");
        return CHEATPANEL_SUCCESS;
    }
    prCheat = cheatengineGetCheat(prEngine, index);
    if (prCheat == NULL) {
        return CHEATPANEL_FAILURE;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    if (cheat_is_constant(prCheat)) {
        prHi = cheatpanel_get_constantcolor(prPanel);
        pspDebugScreenSetTextColor(prHi->text);
    }
    if (cheat_is_selected(prCheat)) {
        prHi = cheatpanel_get_selectedcolor(prPanel);
        pspDebugScreenSetTextColor(prHi->text);
    }
    
    pspDebugScreenSetXY(0, y);
    pspDebugScreenKprintf("%-67s", prCheat->name);
    return CHEATPANEL_SUCCESS;
}
