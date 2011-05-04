#include "gdasm.h"
#include "dasmconfig.h"
#include "cursorpos.h"
#include "colorconfig.h"
#include "geelog.h"

DasmConfig* gdasm_get_config(Gdasm* prDasm) {
    DasmConfig* prCfg = NULL;
    if (prDasm != NULL) {
        prCfg = &prDasm->rConfig;
    }
    return prCfg;
}

ColorConfig* gdasm_get_cursorcolor(Gdasm* prDasm) {
    DasmConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    if (prDasm != NULL) {
        prCfg = gdasm_get_config(prDasm);
        if (prCfg != NULL) {
            prColor = dasmconfig_get_cursorcolor(prCfg);
        }
    }
    return prColor;
}

int gdasm_get_cursorcolumn(Gdasm* prDasm) {
    CursorPos* prPos = NULL;
    int col = GDASM_FAILURE;
    if (prDasm != NULL) {
        prPos = gdasm_get_cursorpos(prDasm);
        if (prPos != NULL) {
            col = prPos->x;
        }
    }
    return col;
}

CursorPos* gdasm_get_cursorpos(Gdasm* prDasm) {
    CursorPos* prPos = NULL;
    if (prDasm != NULL) {
        prPos = &prDasm->rCursor;
    }
    return prPos;
}

int gdasm_get_cursorrow(Gdasm* prDasm) {
    CursorPos* prPos = NULL;
    int row = GDASM_FAILURE;
    if (prDasm != NULL) {
        prPos = gdasm_get_cursorpos(prDasm);
        if (prPos != NULL) {
            row = prPos->y;
        }
    }
    return row;
}

ColorConfig* gdasm_get_panelcolor(Gdasm* prDasm) {
    DasmConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    if (prDasm != NULL) {
        prCfg = gdasm_get_config(prDasm);
        if (prCfg != NULL) {
            prColor = dasmconfig_get_panelcolor(prCfg);
        }
    }
    return prColor;
}

int gdasm_init(Gdasm* prDasm) {
    DasmConfig* prCfg = NULL;
    CursorPos* prPos = NULL;
    int r = 0;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    /* Initialize the DasmConfig member. */
    prCfg = gdasm_get_config(prDasm);
    if (prCfg == NULL) {
        return GDASM_FAILURE;
    }
    r = dasmconfig_init(prCfg);
    if (r != DASMCONFIG_SUCCESS) {
        return GDASM_FAILURE;
    }
    /* Initialize the CursorPos member. */
    prPos = gdasm_get_cursorpos(prDasm);
    if (prPos == NULL) {
        return GDASM_FAILURE;
    }
    r = cursorpos_init(prPos);
    if (r != CURSORPOS_SUCCESS) {
        return GDASM_FAILURE;
    }
    
    return GDASM_SUCCESS;
}

int gdasm_invalidate(Gdasm* prDasm) {
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prDasm->dirty = 1;
    return GDASM_SUCCESS;
}

int gdasm_log(Gdasm* prDasm, ELogLevel rLevel, const char* sMsg) {
    GeeLog* prLog = NULL;
    int r = 0;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prLog = prDasm->prLog;
    if (prLog == NULL) {
        return GDASM_FAILURE;
    }
    r = geelog_log(prLog, rLevel, sMsg);
    if (r != GEELOG_SUCCESS) {
        return GDASM_FAILURE;
    }
    return GDASM_SUCCESS;
}

int gdasm_set_logger(Gdasm* prDasm, GeeLog* prLog) {
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prDasm->prLog = prLog;
    return GDASM_SUCCESS;
}
