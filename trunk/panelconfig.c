#include <stdlib.h>
#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "colorconfig.h"
#include "dimension.h"
#include "cursorpos.h"
#include "panelconfig.h"

int panelconfig_copy(PanelConfig* prDest, PanelConfig* prSrc) {
    int r = 0;
    if (prDest == NULL || prSrc == NULL) {
        return PANELCONFIG_NULLPTR;
    }
    r = colorconfig_copy(&prDest->rColor, &prSrc->rColor);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_copy(&prDest->rCursor, &prSrc->rCursor);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_copy(&prDest->rEdit, &prSrc->rEdit);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = cursorpos_copy(&prDest->rTop, &prSrc->rTop);
    if (r != CURSORPOS_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = dimension_copy(&prDest->rSize, &prSrc->rSize);
    if (r != DIMENSION_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    return PANELCONFIG_SUCCESS;
}

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

int panelconfig_init(PanelConfig* prCfg) {
    int r = 0;
    if (prCfg == NULL) {
        return PANELCONFIG_NULLPTR;
    }
    r = colorconfig_init(&prCfg->rColor);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_init(&prCfg->rCursor);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_init(&prCfg->rEdit);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = cursorpos_init(&prCfg->rTop);
    if (r != CURSORPOS_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = dimension_init(&prCfg->rSize);
    if (r != DIMENSION_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    return PANELCONFIG_SUCCESS;
}

int panelconfig_read(PanelConfig* prCfg, SceUID fd) {
    int r = 0;
    if (prCfg == NULL) {
        return PANELCONFIG_NULLPTR;
    }
    if (fd < 0) {
        return PANELCONFIG_IOERROR;
    }
    r = colorconfig_read(&prCfg->rColor, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_read(&prCfg->rCursor, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_read(&prCfg->rEdit, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = cursorpos_read(&prCfg->rTop, fd);
    if (r != CURSORPOS_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = dimension_read(&prCfg->rSize, fd);
    if (r != DIMENSION_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    return PANELCONFIG_SUCCESS;
}

int panelconfig_write(PanelConfig* prCfg, SceUID fd) {
    int r = 0;
    if (prCfg == NULL) {
        return PANELCONFIG_NULLPTR;
    }
    if (fd < 0) {
        return PANELCONFIG_IOERROR;
    }
    r = colorconfig_write(&prCfg->rColor, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_write(&prCfg->rCursor, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = colorconfig_write(&prCfg->rEdit, fd);
    if (r != COLORCONFIG_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = cursorpos_write(&prCfg->rTop, fd);
    if (r != CURSORPOS_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    r = dimension_write(&prCfg->rSize, fd);
    if (r != DIMENSION_SUCCESS) {
        return PANELCONFIG_FAILURE;
    }
    return PANELCONFIG_SUCCESS;
}
