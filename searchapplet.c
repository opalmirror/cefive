#include "searchapplet.h"

int searchapplet_button_circle(SearchApplet* prApp) {
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_button_cross(SearchApplet* prApp) {
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_button_ltrigger(SearchApplet* prApp) {
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_button_rtrigger(SearchApplet* prApp) {
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_button_square(SearchApplet* prApp) {
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_button_triangle(SearchApplet* prApp) {
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_dpad_down(SearchApplet* prApp) {
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    if (prApp->panel == 0) {
        prQview = searchapplet_get_queryview(prApp);
        if (queryview_dpad_down(prQview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    if (prApp->panel == 1) {
        prRview = searchapplet_get_resultview(prApp);
        if (resultview_dpad_down(prRview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_dpad_left(SearchApplet* prApp) {
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    if (prApp->panel == 0) {
        prQview = searchapplet_get_queryview(prApp);
        if (queryview_dpad_left(prQview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    if (prApp->panel == 1) {
        prRview = searchapplet_get_resultview(prApp);
        if (resultview_dpad_left(prRview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_dpad_right(SearchApplet* prApp) {
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    if (prApp->panel == 0) {
        prQview = searchapplet_get_queryview(prApp);
        if (queryview_dpad_right(prQview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    if (prApp->panel == 1) {
        prRview = searchapplet_get_resultview(prApp);
        if (resultview_dpad_right(prRview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_dpad_up(SearchApplet* prApp) {
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    if (prApp->panel == 0) {
        prQview = searchapplet_get_queryview(prApp);
        if (queryview_dpad_up(prQview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    if (prApp->panel == 1) {
        prRview = searchapplet_get_resultview(prApp);
        if (resultview_dpad_up(prRview) < 0) {
            return SEARCHAPPLET_FAILURE;
        }
        return SEARCHAPPLET_SUCCESS;
    }
    return SEARCHAPPLET_SUCCESS;
}

PanelConfig* searchapplet_get_config(SearchApplet* prApp) {
    if (prApp == NULL) {
        return NULL;
    }
    return &prApp->panelConfig;
}

ColorConfig* searchapplet_get_cursorcolor(SearchApplet* prApp) {
    PanelConfig* prConfig = NULL;
    if (prApp == NULL) {
        return NULL;
    }
    prConfig = searchapplet_get_config(prApp);
    return panelconfig_get_cursorcolor(prConfig);
}

ColorConfig* searchapplet_get_panelcolor(SearchApplet* prApp) {
    PanelConfig* prConfig = NULL;
    if (prApp == NULL) {
        return NULL;
    }
    prConfig = searchapplet_get_config(prApp);
    return panelconfig_get_panelcolor(prConfig);
}

CursorPos* searchapplet_get_position(SearchApplet* prApp) {
    PanelConfig* prConfig = NULL;
    if (prApp == NULL) {
        return NULL;
    }
    prConfig = searchapplet_get_config(prApp);
    return panelconfig_get_position(prConfig);
}

QueryView* searchapplet_get_queryview(SearchApplet* prApp) {
    if (prApp == NULL) {
        return NULL;
    }
    return &prApp->queryView;
}

ResultView* searchapplet_get_resultview(SearchApplet* prApp) {
    if (prApp == NULL) {
        return NULL;
    }
    return &prApp->resultView;
}

Dimension* searchapplet_get_size(SearchApplet* prApp) {
    PanelConfig* prConfig = NULL;
    if (prApp == NULL) {
        return NULL;
    }
    prConfig = searchapplet_get_config(prApp);
    return panelconfig_get_size(prConfig);
}

int searchapplet_init(SearchApplet* prApp) {
    PanelConfig* prConfig = NULL;
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    prConfig = searchapplet_get_config(prApp);
    if (panelconfig_init(prConfig) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prQview = searchapplet_get_queryview(prApp);
    if (queryview_init(prQview) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prRview = searchapplet_get_resultview(prApp);
    if (resultview_init(prRview) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_redraw(SearchApplet* prApp) {
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    prQview = searchapplet_get_queryview(prApp);
    if (queryview_redraw(prQview) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prRview = searchapplet_get_resultview(prApp);
    if (resultview_redraw(prRview) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_set_cursorcolor(SearchApplet* prApp,
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    prColor = searchapplet_get_cursorcolor(prApp);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prQview = searchapplet_get_queryview(prApp);
    if (queryview_set_cursorcolor(prQview, background, text) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prRview = searchapplet_get_resultview(prApp);
    if (resultview_set_cursorcolor(prRview, background, text) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_set_panelcolor(SearchApplet* prApp, 
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    prColor = searchapplet_get_panelcolor(prApp);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prQview = searchapplet_get_queryview(prApp);
    if (queryview_set_panelcolor(prQview, background, text) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prRview = searchapplet_get_resultview(prApp);
    if (resultview_set_panelcolor(prRview, background, text) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_set_position(SearchApplet* prApp, const int x, const int y) {
    CursorPos* prPos = NULL;
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    Dimension* prSize = NULL;
    int cury = 0;
    
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    prPos = searchapplet_get_position(prApp);
    if (cursorpos_set(prPos, x, y) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    /* Position the Query View at 1 row under the top. */
    cury = y + 1;
    prQview = searchapplet_get_queryview(prApp);
    if (queryview_set_position(prQview, x, cury) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    /* Position the Results Panel at 1 row under the Query View. */
    prSize = queryview_get_size(prQview);
    cury += prSize->height;
    prRview = searchapplet_get_resultview(prApp);
    if (resultview_set_position(prRview, x, cury) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    return SEARCHAPPLET_SUCCESS;
}

int searchapplet_set_size(SearchApplet* prApp, 
        const int width, const int height) {
    Dimension* prSize = NULL;
    QueryView* prQview = NULL;
    ResultView* prRview = NULL;
    Dimension* prQsz = NULL;
    int rh = 0;
    
    if (prApp == NULL) {
        return SEARCHAPPLET_NULLPTR;
    }
    prSize = searchapplet_get_size(prApp);
    if (dimension_set(prSize, width, height) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    prQview = searchapplet_get_queryview(prApp);
    prQsz = queryview_get_size(prQview);
    rh = height - 1 - prQsz->height;
    prRview = searchapplet_get_resultview(prApp);
    if (resultview_set_size(prRview, width, rh) < 0) {
        return SEARCHAPPLET_FAILURE;
    }
    return SEARCHAPPLET_SUCCESS;
}
