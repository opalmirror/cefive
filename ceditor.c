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
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_button_cross(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_button_ltrigger(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_button_ltrigger(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_button_rtrigger(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_button_rtrigger(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_button_square(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_button_square(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_button_triangle(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_button_triangle(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_dpad_down(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_dpad_down(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_dpad_left(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_dpad_left(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_dpad_right(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_dpad_right(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_dpad_up(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_dpad_up(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
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
    if (ceditor_set_position(prEd, CEDITOR_POS_X, CEDITOR_POS_Y) < 0) {
        return CEDITOR_FAILURE;
    }
    if (ceditor_set_size(prEd, CEDITOR_SIZE_W, CEDITOR_SIZE_H) < 0) {
        return CEDITOR_FAILURE;
    }
    if (ceditor_set_panelcolor(prEd, CEDITOR_PANEL_BG, CEDITOR_PANEL_FG) < 0) {
        return CEDITOR_FAILURE;
    }
    if (ceditor_set_cursorcolor(prEd, CEDITOR_CURSOR_BG, CEDITOR_CURSOR_FG) < 0)
    {
        return CEDITOR_FAILURE;
    }
    prEd->dirty = 1;
    return CEDITOR_SUCCESS;
}

int ceditor_page_down(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_page_down(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
}

int ceditor_page_up(CEditor* prEd) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (blockview_page_up(&prEd->blockView) < 0) {
        return CEDITOR_FAILURE;
    }
    return CEDITOR_SUCCESS;
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

int ceditor_set_cursorcolor(CEditor* prEd, const u32 background, const u32 text)
{
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    prEd->panelConfig.rCursor.background = background;
    prEd->panelConfig.rCursor.text = text;
    if (blockview_set_cursorcolor(&prEd->blockView, background, text) < 0) {
        return CEDITOR_FAILURE;
    }
    prEd->dirty = 1;
    return CEDITOR_SUCCESS;    
}

int ceditor_set_index(CEditor* prEd, const int index) {
    Cheat* prCheat = NULL;
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    if (prEd->cheatEngine == NULL) {
        return CEDITOR_INVENG;
    }
    if ((index < 0) || (index >= prEd->cheatEngine->cheat_count)) {
        return CEDITOR_INVIDX;
    }
    prCheat = cheatengine_get_cheat(prEd->cheatEngine, index);
    if (prCheat == NULL) {
        return CEDITOR_INVCHT;
    }
    prEd->index = index;
    prEd->blockView.blockIndex = prCheat->block;
    prEd->blockView.blockCount = prCheat->len;
    prEd->blockView.dirty = 1;
    prEd->dirty = 1;
    return CEDITOR_SUCCESS;
}

int ceditor_set_panelcolor(CEditor* prEd, const u32 background, const u32 text)
{
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    prEd->panelConfig.rColor.background = background;
    prEd->panelConfig.rColor.text = text;
    if (blockview_set_panelcolor(&prEd->blockView, background, text) < 0) {
        return CEDITOR_FAILURE;
    }
    prEd->dirty = 1;
    return CEDITOR_SUCCESS;    
}

int ceditor_set_position(CEditor* prEd, const int x, const int y) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    prEd->panelConfig.rTop.x = x;
    prEd->panelConfig.rTop.y = y;
    if (blockview_set_position(&prEd->blockView, x, y + 2) < 0) {
        return CEDITOR_FAILURE;
    }
    prEd->dirty = 1;
    return CEDITOR_SUCCESS;
}

int ceditor_set_size(CEditor* prEd, const int width, const int height) {
    if (prEd == NULL) {
        return CEDITOR_NULLPTR;
    }
    prEd->panelConfig.rSize.width = width;
    prEd->panelConfig.rSize.height = height;
    if (blockview_set_size(&prEd->blockView, width, height - 2) < 0) {
        return CEDITOR_FAILURE;
    }
    prEd->dirty = 1;
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
