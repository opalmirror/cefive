#include "cheatview.h"

static int active_color(CheatView* prView);
static int constant_color(CheatView* prView);
static int cursor_color(CheatView* prView);
static int panel_color(CheatView* prView);
static int render_row(CheatView* prView, const int row);
static int row_index(CheatView* prView, const int row);

static int active_color(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    pspDebugScreenSetTextColor(prView->activeColor);
    return CHEATVIEW_SUCCESS;
}

int cheatview_button_circle(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_button_cross(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_button_ltrigger(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_button_rtrigger(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_button_square(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_button_triangle(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_dpad_down(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_dpad_left(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_dpad_right(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_dpad_up(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_init(CheatView* prView, CheatEngine* prEngine) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    if (panelconfig_init(&prView->panelConfig) < 0) {
        return CHEATVIEW_FAILURE;
    }
    if (cursorpos_init(&prView->cursorPos) < 0) {
        return CHEATVIEW_FAILURE;
    }
    prView->cheatEngine = prEngine;
    prView->pageIndex = 0;
    
    prView->panelConfig.rTop.x = CHEATVIEW_POS_X;
    prView->panelConfig.rTop.y = CHEATVIEW_POS_Y;
    prView->panelConfig.rSize.width = CHEATVIEW_SIZE_W;
    prView->panelConfig.rSize.height = CHEATVIEW_SIZE_H;
    prView->panelConfig.rColor.background = CHEATVIEW_PANEL_BG;
    prView->panelConfig.rColor.text = CHEATVIEW_PANEL_FG;
    prView->activeColor = CHEATVIEW_ACTIVE;
    prView->constantColor = CHEATVIEW_CONSTANT;
    
    prView->dirty = 1;
    return CHEATVIEW_SUCCESS;
}

int cheatview_page_down(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_page_up(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

int cheatview_redraw(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    
    return CHEATVIEW_SUCCESS;
}

static int constant_color(CheatView* prView) {
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    pspDebugScreenSetTextColor(prView->constantColor);
    return CHEATVIEW_SUCCESS;
}

static int cursor_color(CheatView* prView) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    prColor = &prView->panelConfig.rCursor;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    return CHEATVIEW_SUCCESS;
}

static int panel_color(CheatView* prView) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    prColor = &prView->panelConfig.rColor;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    return CHEATVIEW_SUCCESS;
}

static int render_row(CheatView* prView, const int row) {
    Cheat* prCheat = NULL;
    CheatEngine* prEngine = NULL;
    CursorPos* prCursor = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    int index = 0;
    char sFmt[10];
    
    if (prView == NULL) {
        return CHEATVIEW_NULLPTR;
    }
    prEngine = prView->cheatEngine;
    if (prEngine == NULL) {
        return CHEATVIEW_INVENG;
    }
    prSize = &prView->panelConfig.rSize;
    prPos = &prView->panelConfig.rTop;
    prCursor = &prView->cursorPos;
    
    sprintf(sFmt, "%%-%ds", prSize->width);
    pspDebugScreenSetXY(prPos->x, prPos->y + row + 1);
    if (panel_color(prView) < 0) {
        return CHEATVIEW_FAILURE;
    }
    index = row_index(prView, row);
    if ((index >= prEngine->cheat_count) || (index < 0)) {
        pspDebugScreenKprintf(sFmt, "");
        return CHEATVIEW_SUCCESS;
    }
    
    prCheat = cheatengine_get_cheat(prEngine, index);
    if (prCheat == NULL) {
        pspDebugScreenKprintf(sFmt, "");
        return CHEATVIEW_FAILURE;
    }
    if (prCursor->y == row) {
        if (cursor_color(prView) < 0) {
            return CHEATVIEW_FAILURE;
        }
    }
    if (strlen(prCheat->name) > 0) {
        if (cheat_is_selected(prCheat)) {
            if (active_color(prView) < 0) {
                return CHEATVIEW_FAILURE;
            }
        }
        if (cheat_is_constant(prCheat)) {
            if (constant_color(prView) < 0) {
                return CHEATVIEW_FAILURE;
            }
        }
        pspDebugScreenKprintf(sFmt, prCheat->name);
    } else {
        pspDebugScreenKprintf(sFmt, "");
    }
    return CHEATVIEW_SUCCESS;
}

static int row_index(CheatView* prView, const int row) {
    if (prView == NULL) {
        return -1;
    }
    return prView->pageIndex + row;
}
