#include "appletmenu.h"

static int render_row(AppletMenu* prMenu, const int row);

int appletmenu_additem(AppletMenu* prMenu, const int id, const char* text) {
    MenuModel *prModel = NULL;
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prModel = appletmenu_get_model(prMenu);
    if (menumodel_additem(prModel, id, text) < 0) {
        return APPLETMENU_FAILURE;
    }
    return APPLETMENU_SUCCESS;
}

int appletmenu_circle_button(AppletMenu* prMenu) {
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    
    return APPLETMENU_SUCCESS;
}

int appletmenu_cross_button(AppletMenu* prMenu) {
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    if (appletmenu_is_visible(prMenu)) {
        prMenu->visible = 0;
        prMenu->dirty = 1;
    }
    return APPLETMENU_SUCCESS;
}

int appletmenu_cursor_down(AppletMenu* prMenu) {
    CursorPos* prPos = NULL;
    MenuModel* prModel = NULL;
    int y = 0;
    int oldy = 0;
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prModel = appletmenu_get_model(prMenu);
    prPos = appletmenu_get_cursorpos(prMenu);
    y = prPos->y;
    oldy = y;
    y++;
    if (y >= prModel->rowCount) {
        y = 0;
    }
    prPos->y = y;
    if (render_row(prMenu, oldy) != APPLETMENU_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    if (render_row(prMenu, y) != APPLETMENU_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    return APPLETMENU_SUCCESS;
}

int appletmenu_cursor_up(AppletMenu* prMenu) {
    CursorPos* prPos = NULL;
    MenuModel* prModel = NULL;
    int y = 0;
    int oldy = 0;
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prModel = appletmenu_get_model(prMenu);
    prPos = appletmenu_get_cursorpos(prMenu);
    y = prPos->y;
    oldy = y;
    y--;
    if (y < 0) {
        y = prModel->rowCount - 1;
    }
    prPos->y = y;
    if (render_row(prMenu, oldy) != APPLETMENU_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    if (render_row(prMenu, y) != APPLETMENU_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    return APPLETMENU_SUCCESS;
}

AppletConfig* appletmenu_get_appletconfig(AppletMenu* prMenu) {
    if (prMenu != NULL) {
        return prMenu->prApCfg;
    }
    return NULL;
}

AppletMenuConfig* appletmenu_get_config(AppletMenu* prMenu) {
    if (prMenu != NULL) {
        return &prMenu->config;
    }
    return NULL;
}

ColorConfig* appletmenu_get_cursorcolor(AppletMenu* prMenu) {
    AppletConfig* prApCfg = NULL;
    
    if (prMenu != NULL) {
        prApCfg = appletmenu_get_appletconfig(prMenu);
        if (prApCfg != NULL) {
            return appletconfig_get_cursorcolor(prApCfg);
        }
    }
    return NULL;
}

CursorPos* appletmenu_get_cursorpos(AppletMenu* prMenu) {
    if (prMenu != NULL) {
        return &prMenu->cursor;
    }
    return NULL;
}

MenuModel* appletmenu_get_model(AppletMenu* prMenu) {
    if (prMenu != NULL) {
        return &prMenu->model;
    }
    return NULL;
}

ColorConfig* appletmenu_get_panelcolor(AppletMenu* prMenu) {
    AppletConfig* prApCfg = NULL;
    
    if (prMenu != NULL) {
        prApCfg = appletmenu_get_appletconfig(prMenu);
        if (prApCfg != NULL) {
            return appletconfig_get_statuscolor(prApCfg);
        }
    }
    return NULL;
}

CursorPos* appletmenu_get_position(AppletMenu* prMenu) {
    AppletMenuConfig* prConfig = NULL;
    
    if (prMenu != NULL) {
        prConfig = appletmenu_get_config(prMenu);
        return appletmenuconfig_get_position(prConfig);
    }
    
    return NULL;
}

int appletmenu_get_selectedindex(AppletMenu* prMenu) {
    CursorPos* prCursor = NULL;
    int index = APPLETMENU_FAILURE;
    if (prMenu != NULL) {
        prCursor = appletmenu_get_cursorpos(prMenu);
        if (prCursor != NULL) {
            index = prCursor->y;
        }
    }
    return index;
}

Dimension* appletmenu_get_size(AppletMenu* prMenu) {
    AppletMenuConfig* prConfig = NULL;
    
    if (prMenu != NULL) {
        prConfig = appletmenu_get_config(prMenu);
        return appletmenuconfig_get_size(prConfig);
    }
    
    return NULL;
}

int appletmenu_init(AppletMenu* prMenu) {
    AppletMenuConfig* prConfig = NULL;
    CursorPos* prPos = NULL;
    MenuModel* prModel = NULL;
    
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prMenu->visible = 0;
    prConfig = appletmenu_get_config(prMenu);
    if (appletmenuconfig_init(prConfig) != APPLETMENUCONFIG_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    
    prPos = appletmenu_get_cursorpos(prMenu);
    if (cursorpos_init(prPos) != CURSORPOS_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    
    prModel = appletmenu_get_model(prMenu);
    if (menumodel_init(prModel) != MENUMODEL_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    menumodel_additem(prModel, 0, "Cheat Panel");
    menumodel_additem(prModel, 1, "Disassembler");
    menumodel_additem(prModel, 2, "Hex Editor");
    menumodel_additem(prModel, 3, "Search Panel");
    menumodel_additem(prModel, 4, "Game Info");
    menumodel_additem(prModel, 5, "Options");
    appletmenu_invalidate(prMenu);
    return APPLETMENU_SUCCESS;
}

int appletmenu_invalidate(AppletMenu* prMenu) {
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prMenu->dirty = 1;
    return APPLETMENU_SUCCESS;
}

int appletmenu_is_visible(AppletMenu* prMenu) {
    int visible = 0;
    if (prMenu != NULL) {
        visible = prMenu->visible;
    }
    return (visible != 0);
}

int appletmenu_redraw(AppletMenu* prMenu) {
    MenuModel* prModel = NULL;
    int i = 0;
    
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    if (prMenu->dirty != 1) {
        return APPLETMENU_SUCCESS;
    }
    prModel = appletmenu_get_model(prMenu);
    for (i = 0; i < prModel->rowCount; i++) {
        if (render_row(prMenu, i) != APPLETMENU_SUCCESS) {
            return APPLETMENU_FAILURE;
        }
    }
    prMenu->dirty = 0;
    return APPLETMENU_SUCCESS;
}

int appletmenu_set_position(AppletMenu* prMenu, int x, int y) {
    CursorPos* prPos = NULL;
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prPos = appletmenu_get_position(prMenu);
    if (cursorpos_set(prPos, x, y) != CURSORPOS_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    
    return APPLETMENU_SUCCESS;
}

int appletmenu_set_size(AppletMenu* prMenu, int width, int height) {
    Dimension* prSize = NULL;
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prSize = appletmenu_get_size(prMenu);
    if (dimension_set(prSize, width, height) != DIMENSION_SUCCESS) {
        return APPLETMENU_FAILURE;
    }
    return APPLETMENU_SUCCESS;
}

void appletmenuCrossButton(AppletMenu *prMenu) {
    appletmenu_cross_button(prMenu);
}

/* handles down scrolling in applet menu */
void appletmenuCursorDown(AppletMenu *prMenu) {
    appletmenu_cursor_down(prMenu);
}

/* handles up scrolling in applet menu */
void appletmenuCursorUp(AppletMenu *prMenu) {
    appletmenu_cursor_up(prMenu);
}

int appletmenuGetSelectedIndex(AppletMenu *prMenu) {
    return appletmenu_get_selectedindex(prMenu);
}

void appletmenuRedraw(AppletMenu *prMenu) {
    appletmenu_redraw(prMenu);
}

static int render_row(AppletMenu* prMenu, const int row) {
    MenuModel* prModel = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prColor = NULL;
    MenuItem* prItem = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    int x = 0;
    int y = 0;
    char sFs[20];
    
    if (prMenu == NULL) {
        return APPLETMENU_NULLPTR;
    }
    prModel = appletmenu_get_model(prMenu);
    if ((row < 0) || (row >= prModel->rowCount)) {
        return APPLETMENU_INVIDX;
    }
    prItem = menumodel_get(prModel, row);
    if (prItem == NULL) {
        return APPLETMENU_FAILURE;
    }
    prColor = appletmenu_get_panelcolor(prMenu);
    prCursor = appletmenu_get_cursorpos(prMenu);
    if (prCursor->y == row) {
        prColor = appletmenu_get_cursorcolor(prMenu);
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    
    prPos = appletmenu_get_position(prMenu);
    prSize = appletmenu_get_size(prMenu);
    x = prPos->x;
    y = prPos->y + row;
    pspDebugScreenSetXY(x, y);
    sprintf(sFs, "%%-%ds", prSize->width);
    pspDebugScreenKprintf(sFs, prItem->text);
    return APPLETMENU_SUCCESS;
}
