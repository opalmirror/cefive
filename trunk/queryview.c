#include "queryview.h"

int queryview_button_circle(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_cross(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_ltrigger(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_rtrigger(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_square(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_button_triangle(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_down(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_left(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_right(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_dpad_up(QueryView *prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

PanelConfig* queryview_get_config(QueryView* prView) {
    if (prView != NULL) {
        return &prView->panelConfig;
    }
    return NULL;
}

ColorConfig* queryview_get_cursorcolor(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_cursorcolor(prConfig);
    }
    return NULL;
}

CursorPos* queryview_get_cursorpos(QueryView* prView) {
    if (prView != NULL) {
        return &prView->cursorPos;
    }
    return NULL;
}

HexPad* queryview_get_hexpad(QueryView* prView) {
    if (prView != NULL) {
        return &prView->hexPad;
    }
    return NULL;
}

ColorConfig* queryview_get_panelcolor(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_panelcolor(prConfig);
    }
    return NULL;
}

CursorPos* queryview_get_position(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_position(prConfig);
    }
    return NULL;
}

SearchQuery* queryview_get_query(QueryView* prView) {
    if (prView != NULL) {
        return &prView->searchQuery;
    }
    return NULL;
}

Dimension* queryview_get_size(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = queryview_get_config(prView);
        return panelconfig_get_size(prConfig);
    }
    return NULL;
}

int queryview_init(QueryView* prView) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prConfig = queryview_get_config(prView);
    if (panelconfig_init(prConfig) < 0) {
        return QUERYVIEW_FAILURE;
    }
    prCursor = queryview_get_cursorpos(prView);
    if (cursorpos_init(prCursor) < 0) {
        return QUERYVIEW_FAILURE;
    }
    prPad = queryview_get_hexpad(prView);
    if (hexpad_init(prPad) < 0) {
        return QUERYVIEW_FAILURE;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_redraw(QueryView* prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_set_cursorcolor(QueryView* prView,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prColor = queryview_get_cursorcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return QUERYVIEW_FAILURE;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_set_cursorpos(QueryView* prView, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prCursor = queryview_get_cursorpos(prView);
    if (cursorpos_set(prCursor, x, y) < 0) {
        return QUERYVIEW_FAILURE;
    }
    
    return QUERYVIEW_SUCCESS;
}

int queryview_set_panelcolor(QueryView* prView,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prColor = queryview_get_panelcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_set_position(QueryView* prView, const int x, const int y) {
    CursorPos* prPos = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prPos = queryview_get_position(prView);
    if (cursorpos_set(prPos, x, y) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

int queryview_set_size(QueryView* prView,
        const int width, const int height) {
    Dimension* prSize = NULL;
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    prSize = queryview_get_size(prView);
    if (dimension_set(prSize, width, height) < 0) {
        return QUERYVIEW_FAILURE;
    }
    return QUERYVIEW_SUCCESS;
}

static int render_value_row(QueryView* prView) {
    if (prView == NULL) {
        return QUERYVIEW_NULLPTR;
    }
    return QUERYVIEW_SUCCESS;
}