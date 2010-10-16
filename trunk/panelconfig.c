#include <stdlib.h>
#include "colorconfig.h"
#include "dimension.h"
#include "cursorpos.h"
#include "panelconfig.h"

ColorConfig* panelconfig_get_cursorcolor(PanelConfig* prCfg) {
    ColorConfig* prColor = NULL;
    if (prCfg != NULL) {
        prColor = &prCfg->rCursor;
    }
    return prColor;
}

ColorConfig* panelconfig_get_editcolor(PanelConfig* prCfg) {
    ColorConfig* prColor = NULL;
    if (prCfg != NULL) {
        prColor = &prCfg->rEdit;
    }
    return prColor;
}

ColorConfig* panelconfig_get_panelcolor(PanelConfig* prCfg) {
    ColorConfig* prColor = NULL;
    if (prCfg != NULL) {
        prColor = &prCfg->rColor;
    }
    return prColor;
}

CursorPos* panelconfig_get_position(PanelConfig* prCfg) {
    CursorPos* prPos = NULL;
    if (prCfg != NULL) {
        prPos = &prCfg->rTop;
    }
    return prPos;
}

Dimension* panelconfig_get_size(PanelConfig* prCfg) {
    Dimension* prDim = NULL;
    if (prCfg != NULL) {
        prDim = &prCfg->rSize;
    }
    return prDim;
}
