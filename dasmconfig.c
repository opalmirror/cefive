#include "dasmconfig.h"
#include "appletconfig.h"

AppletConfig* dasmconfig_get_appletconfig(DasmConfig* prCfg) {
    AppletConfig* prApp = NULL;
    if (prCfg != NULL) {
        prApp = prCfg->prApCfg;
    }
    return prApp;
}

SceUInt32 dasmconfig_get_baseaddr(DasmConfig* prCfg) {
    SceUInt32 base = 0;
    if (prCfg != NULL) {
        base = prCfg->baseAddress;
    }
    return base;
}

ColorConfig* dasmconfig_get_cursorcolor(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = NULL;
    if (prCfg != NULL) {
        prApp = dasmconfig_get_appletconfig(prCfg);
        if (prApp != NULL) {
            prColor = appletconfig_get_cursorcolor(prApp);
        }
    }
    return prColor;
}

ColorConfig* dasmconfig_get_editcolor(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = NULL;
    if (prCfg != NULL) {
        prApp = dasmconfig_get_appletconfig(prCfg);
        if (prApp != NULL) {
            prColor = appletconfig_get_editcolor(prApp);
        }
    }
    return prColor;
}

ColorConfig* dasmconfig_get_panelcolor(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = NULL;
    if (prCfg != NULL) {
        prApp = dasmconfig_get_appletconfig(prCfg);
        if (prApp != NULL) {
            prColor = appletconfig_get_panelcolor(prApp);
        }
    }
    return prColor;
}

ColorConfig* dasmconfig_get_pointercolor(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    if (prCfg != NULL) {
        prColor = &prCfg->rPtrColor;
    }
    return prColor;
}

ColorConfig* dasmconfig_get_statuscolor(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = NULL;
    if (prCfg != NULL) {
        prApp = dasmconfig_get_appletconfig(prCfg);
        if (prApp != NULL) {
            prColor = appletconfig_get_statuscolor(prApp);
        }
    }
    return prColor;
}

ColorConfig* dasmconfig_get_titlecolor(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = NULL;
    if (prCfg != NULL) {
        prApp = dasmconfig_get_appletconfig(prCfg);
        if (prApp != NULL) {
            prColor = appletconfig_get_titlecolor(prApp);
        }
    }
    return prColor;
}

int dasmconfig_init(DasmConfig* prCfg) {
    ColorConfig* prColor = NULL;
    if (prCfg == NULL) {
        return DASMCONFIG_NULLPTR;
    }
    
    dasmconfig_set_appletconfig(prCfg, NULL);
    dasmconfig_set_baseaddr(prCfg, 0);
    prColor = dasmconfig_get_pointercolor(prCfg);
    colorconfig_init(prColor);
    
    return DASMCONFIG_SUCCESS;
}

int dasmconfig_set_appletconfig(DasmConfig* prCfg, AppletConfig* prApp) {
    if (prCfg == NULL) {
        return DASMCONFIG_NULLPTR;
    }
    prCfg->prApCfg = prApp;
    return DASMCONFIG_SUCCESS;
}

int dasmconfig_set_baseaddr(DasmConfig* prCfg, SceUInt32 addr) {
    if (prCfg == NULL) {
        return DASMCONFIG_NULLPTR;
    }
    prCfg->baseAddress = addr;
    return DASMCONFIG_SUCCESS;
}
