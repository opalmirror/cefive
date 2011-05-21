#include "appletmenuconfig.h"

ColorConfig* appletmenuconfig_get_cursorcolor(AppletMenuConfig* prConfig) {
    ColorConfig* prColor = NULL;
    if (prConfig != NULL) {
        prColor = &prConfig->cursor;
    }
    return prColor;
}

ColorConfig* appletmenuconfig_get_panelcolor(AppletMenuConfig* prConfig) {
    ColorConfig* prColor = NULL;
    if (prConfig != NULL) {
        prColor = &prConfig->color;
    }
    return prColor;
}

CursorPos* appletmenuconfig_get_position(AppletMenuConfig* prConfig) {
    CursorPos* prPos = NULL;
    if (prConfig != NULL) {
        prPos = &prConfig->top;
    }
    return prPos;
}

Dimension* appletmenuconfig_get_size(AppletMenuConfig* prConfig) {
    Dimension* prSize = NULL;
    if (prConfig != NULL) {
        prSize = &prConfig->size;
    }
    return prSize;
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
