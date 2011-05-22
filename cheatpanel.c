#include "cheatpanel.h"

static void toggleCheatConstant(CheatPanel *prPanel, int index);
static void toggleCheatSelected(CheatPanel *prPanel, int index);

AppletConfig* cheatpanel_get_appletconfig(CheatPanel* prPanel) {
    AppletConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = prPanel->prApCfg;
    }
    return prConfig;
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

void cheatpanelCircleButton(CheatPanel *prPanel) {
}

void cheatpanelCrossButton(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int si = cheatpanelGetSelectedIndex(prPanel);
    if (si == -1) {
        return;
    }
    toggleCheatSelected(prPanel, si);
}

void cheatpanelDpadDown(CheatPanel *prPanel) {
    CheatEngine* prEngine = NULL;
    if (prPanel == NULL) {
        return;
    }
    prEngine = prPanel->prEngine;
    if (prEngine == NULL) {
        return;
    }
    int lpp = prPanel->config.table_height;
    int y = prPanel->cursor.y;
    int cc = prEngine->cheat_count;
    int si = cheatpanelGetSelectedIndex(prPanel);
    if (si < cc) {
        y++;
    }
    if (y >= lpp) {
        cheatpanelScrollDown(prPanel);
        y = lpp - 1;
    }
    prPanel->cursor.y = y;
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

void cheatpanelDpadUp(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int y = prPanel->cursor.y;
    y--;
    if (y < 0) {
        cheatpanelScrollUp(prPanel);
        y = 0;
    }
    prPanel->cursor.y = y;
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
}

void cheatpanelRedraw(CheatPanel *prPanel) {
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCursor = NULL;
    int i;
    char lbuf[69];
    int iCheat;
    int iTop;
    u32 bg;
    u32 fg;
    int iSelIndex;

    if (prPanel == NULL) {
        return;
    }
    prApCfg = prPanel->prApCfg;
    iTop = prPanel->config.top_row;
    iSelIndex = cheatpanelGetSelectedIndex(prPanel);
    CheatEngine* prEngine = prPanel->prEngine;
    Cheat* prCheat = NULL;
    unsigned char flags = (unsigned char)0;
    pspDebugScreenSetXY(0, iTop);
    if (prApCfg != NULL) {
        prColor = &prApCfg->rPanel.rColor;
        prCursor = &prApCfg->rPanel.rCursor;
    } else {
        prColor = &prPanel->config.color;
        prCursor = &prPanel->config.cursorrowcolor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("%-67s", "Cheat Panel");
    iTop++;

    for (i = 0; i < prPanel->config.table_height; i++) {
        bg = prColor->background;
        fg = prColor->text;
        if (i == prPanel->cursor.y && prEngine->cheat_count > 0) {
            bg = prCursor->background;
            fg = prCursor->text;
        }
        sprintf(lbuf, "%67s", "");
        iCheat = prPanel->page_position + i;
        if (iCheat < prEngine->cheat_count) {
            prCheat = cheatengineGetCheat(prEngine, iCheat);
            if (prCheat->name != NULL) {
                sprintf(lbuf, "%-67s", prCheat->name);
            }
            flags = prCheat->flags;
            if (cheat_is_selected(prCheat) == 1) {
                fg = prPanel->config.selectedcolor.text;
            }
            if (cheat_is_constant(prCheat) == 1) {
                fg = prPanel->config.constantcolor.text;
            }
        }
        pspDebugScreenSetBackColor(bg);
        pspDebugScreenSetTextColor(fg);
        pspDebugScreenSetXY(0, iTop + i);
        pspDebugScreenPuts(lbuf);
    }
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
}

void cheatpanelSquareButton(CheatPanel *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    int si = cheatpanelGetSelectedIndex(prPanel);
    if (si == -1) {
        return;
    }
    toggleCheatConstant(prPanel, si);
}

void cheatpanelTriangleButton(CheatPanel *prPanel) {
}

static void toggleCheatConstant(CheatPanel *prPanel, int index) {
    if (prPanel == NULL) {
        return;
    }
    Cheat *prCheat = cheatengineGetCheat(prPanel->prEngine, index);
    if (cheat_is_constant(prCheat)) {
        cheatengineSetCheatInactive(prPanel->prEngine, index);
    } else {
        cheatengineSetCheatConstant(prPanel->prEngine, index);
    }
}

static void toggleCheatSelected(CheatPanel *prPanel, int index) {
    if (prPanel == NULL) {
        return;
    }
    Cheat *prCheat = cheatengineGetCheat(prPanel->prEngine, index);
    if (cheat_is_selected(prCheat)) {
        cheatengineSetCheatInactive(prPanel->prEngine, index);
    } else {
        cheatengineSetCheatSelected(prPanel->prEngine, index);
    }
}
