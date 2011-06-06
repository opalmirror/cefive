#include "ceditor.h"

static int cursorcolor(CEditor* prEd);
static int panelcolor(CEditor* prEd);

int ceditor_button_circle(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_button_circle(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_button_cross(CEditor* prEd) {
    
}

int ceditor_button_ltrigger(CEditor* prEd) {
    
}

int ceditor_button_rtrigger(CEditor* prEd) {
    
}

int ceditor_button_square(CEditor* prEd) {
    
}

int ceditor_button_triangle(CEditor* prEd) {
    
}

int ceditor_dpad_down(CEditor* prEd) {
    
}

int ceditor_dpad_left(CEditor* prEd) {
    
}

int ceditor_dpad_right(CEditor* prEd) {
    
}

int ceditor_dpad_up(CEditor* prEd) {
    
}

int ceditor_init(CEditor* prEd, CheatEngine* prEngine) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (panelconfig_init(&prEd->panelConfig) < 0) {
        return CEDITOR_FAILURE;
    }
    prEd->cheatEngine = prEngine;
    if (blockview_init(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    if (prEngine != NULL) {
        prEd->blockView.blockList = prEngine->blocklist;
    }
    prEd->index = -1;
    prEd->dirty = 1;
    return CEDITOR_SUCCESS;
}

int ceditor_page_down(CEditor* prEd) {
    
}

int ceditor_page_up(CEditor* prEd) {
    
}

int ceditor_redraw(CEditor* prEd) {
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    Cheat* prCheat = NULL;
    char sFmt[10];
    char sLine[70];
    
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_redraw(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    if (prEd->dirty == 0) {
        return CEDITOR_SUCCESS;
    }
    prPos = &prEd->panelConfig.rTop;
    prSize = &prEd->panelConfig.rSize;
    pspDebugScreenSetXY(prPos->x, prPos->y);
    panelcolor(prEd);
    sprintf(sFmt, "%%-%ds", prSize->width);
    pspDebugScreenKprintf(sFmt, "Cheat Editor");
    
    if (prEd->index >= 0 && prEd->cheatEngine != NULL) {
        prCheat = cheatengine_get_cheat(prEd->cheatEngine, prEd->index);
        if (prCheat != NULL) {
            pspDebugScreenSetXY(prPos->x, prPos->y + 1);
            sprintf(sLine, "Name: %s", prCheat->name);
            panelcolor(prEd);
            pspDebugScreenKprintf(sFmt, sLine);
        }
    }
    
    prEd->dirty = 0;
    return CEDITOR_SUCCESS;
}

static int cursorcolor(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    pspDebugScreenSetBackColor(prEd->panelConfig.rCursor.background);
    pspDebugScreenSetTextColor(prEd->panelConfig.rCursor.text);
    return CEDITOR_SUCCESS;
}

static int panelcolor(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    pspDebugScreenSetBackColor(prEd->panelConfig.rColor.background);
    pspDebugScreenSetTextColor(prEd->panelConfig.rColor.text);
    return CEDITOR_SUCCESS;
}
