#include "appletmenuconfig.h"

ColorConfig* appletmenuconfig_get_cursorcolor(AppletMenuConfig* prConfig) {
    if (prConfig != NULL) {
        return &prConfig->cursor;
    }
    return NULL;
}

ColorConfig* appletmenuconfig_get_panelcolor(AppletMenuConfig* prConfig) {
    if (prConfig != NULL) {
        return &prConfig->color;
    }
    return NULL;
}

CursorPos* appletmenuconfig_get_position(AppletMenuConfig* prConfig) {
    if (prConfig != NULL) {
        return &prConfig->top;
    }
    return NULL;
}

Dimension* appletmenuconfig_get_size(AppletMenuConfig* prConfig) {
    if (prConfig != NULL) {
        return &prConfig->size;
    }
    return NULL;
}

int appletmenuconfig_init(AppletMenuConfig* prConfig) {
    ColorConfig* prColor = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    
    if (prConfig == NULL) {
        return APPLETMENUCONFIG_NULLPTR;
    }
    prColor = appletmenuconfig_get_panelcolor(prConfig);
    if (colorconfig_init(prColor) != COLORCONFIG_SUCCESS) {
        return APPLETMENUCONFIG_FAILURE;
    }
    prColor = appletmenuconfig_get_cursorcolor(prConfig);
    if (colorconfig_init(prColor) != COLORCONFIG_SUCCESS) {
        return APPLETMENUCONFIG_FAILURE;
    }
    prPos = appletmenuconfig_get_position(prConfig);
    if (cursorpos_init(prPos) != CURSORPOS_SUCCESS) {
        return APPLETMENUCONFIG_FAILURE;
    }
    prSize = appletmenuconfig_get_size(prConfig);
    if (dimension_init(prSize) != DIMENSION_SUCCESS) {
        return APPLETMENUCONFIG_FAILURE;
    }
    return APPLETMENUCONFIG_SUCCESS;
}
