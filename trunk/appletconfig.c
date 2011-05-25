#include "appletconfig.h"

int appletconfig_copy(AppletConfig* prDest, AppletConfig* prSrc) {
    int r = 0;
    if (prDest == NULL || prSrc == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    r = panelconfig_copy(&prDest->rPanel, &prSrc->rPanel);
    if (r != PANELCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_copy(&prDest->rStatus, &prSrc->rStatus);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_copy(&prDest->rTitlebar, &prSrc->rTitlebar);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

ColorConfig* appletconfig_get_cursorcolor(AppletConfig* prConfig) {
    ColorConfig* prColor = NULL;
    PanelConfig* prPan = appletconfig_get_panelconfig(prConfig);
    if (prPan != NULL) {
        prColor = panelconfig_get_cursorcolor(prPan);
    }
    return prColor;
}

ColorConfig* appletconfig_get_editcolor(AppletConfig* prConfig) {
    ColorConfig* prColor = NULL;
    PanelConfig* prPan = appletconfig_get_panelconfig(prConfig);
    if (prPan != NULL) {
        prColor = panelconfig_get_editcolor(prPan);
    }
    return prColor;
}

ColorConfig* appletconfig_get_panelcolor(AppletConfig* prConfig) {
    ColorConfig* prColor = NULL;
    PanelConfig* prPan = appletconfig_get_panelconfig(prConfig);
    if (prPan != NULL) {
        prColor = panelconfig_get_panelcolor(prPan);
    }
    return prColor;
}

PanelConfig* appletconfig_get_panelconfig(AppletConfig* prConfig) {
    PanelConfig* prPan = NULL;
    if (prConfig != NULL) {
        prPan = &prConfig->rPanel;
    }
    return prPan;
}

CursorPos* appletconfig_get_position(AppletConfig* prConfig) {
    CursorPos* prPos = NULL;
    PanelConfig* prCfg = NULL;
    if (prConfig != NULL) {
        prCfg = appletconfig_get_panelconfig(prConfig);
        prPos = panelconfig_get_position(prCfg);
    }
    return prPos;
}

Dimension* appletconfig_get_size(AppletConfig* prConfig) {
    Dimension* prSize = NULL;
    PanelConfig* prCfg = NULL;
    if (prConfig != NULL) {
        prCfg = appletconfig_get_panelconfig(prConfig);
        prSize = panelconfig_get_size(prCfg);
    }
    return prSize;
}

ColorConfig* appletconfig_get_statuscolor(AppletConfig* prConfig) {
    ColorConfig* prColor = NULL;
    if (prConfig != NULL) {
        prColor = &prConfig->rStatus;
    }
    return prColor;
}

ColorConfig* appletconfig_get_titlecolor(AppletConfig* prConfig) {
    ColorConfig* prColor = NULL;
    if (prConfig != NULL) {
        prColor = &prConfig->rTitlebar;
    }
    return prColor;
}

int appletconfig_init(AppletConfig* prConfig) {
    int r = 0;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    r = panelconfig_init(&prConfig->rPanel);
    if (r != PANELCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_init(&prConfig->rStatus);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_init(&prConfig->rTitlebar);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_read(AppletConfig* prConfig, SceUID fd) {
    int r = 0;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    if (fd < 0) {
        return APPLETCONFIG_IOERROR;
    }
    r = panelconfig_read(&prConfig->rPanel, fd);
    if (r != PANELCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_read(&prConfig->rStatus, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_read(&prConfig->rTitlebar, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_cursorcolor(AppletConfig* prConfig, 
            const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prColor = appletconfig_get_cursorcolor(prConfig);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_editcolor(AppletConfig* prConfig, 
            const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prColor = appletconfig_get_editcolor(prConfig);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_panelcolor(AppletConfig* prConfig, 
            const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prColor = appletconfig_get_panelcolor(prConfig);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_position(AppletConfig* prConfig, 
            const int x, const int y) {
    CursorPos *prPos = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prPos = appletconfig_get_position(prConfig);
    if (cursorpos_set(prPos, x, y) != CURSORPOS_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_size(AppletConfig* prConfig,
            const int width, const int height) {
    Dimension* prSize = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prSize = appletconfig_get_size(prConfig);
    if (dimension_set(prSize, width, height) != DIMENSION_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_statuscolor(AppletConfig* prConfig, 
            const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prColor = appletconfig_get_statuscolor(prConfig);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_set_titlecolor(AppletConfig* prConfig, 
            const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    prColor = appletconfig_get_titlecolor(prConfig);
    if (colorconfig_setcolor(prColor, background, text) != COLORCONFIG_SUCCESS)
    {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}

int appletconfig_write(AppletConfig* prConfig, SceUID fd) {
    int r = 0;
    if (prConfig == NULL) {
        return APPLETCONFIG_NULLPTR;
    }
    if (fd < 0) {
        return APPLETCONFIG_IOERROR;
    }
    r = panelconfig_write(&prConfig->rPanel, fd);
    if (r != PANELCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_write(&prConfig->rStatus, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    r = colorconfig_write(&prConfig->rTitlebar, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return APPLETCONFIG_FAILURE;
    }
    return APPLETCONFIG_SUCCESS;
}
