#include <stdlib.h>
#include "colorconfig.h"
#include "panelconfig.h"
#include "appletconfig.h"

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
