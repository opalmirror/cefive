#include "gdasm.h"
#include "dasmconfig.h"
#include "cursorpos.h"
#include "colorconfig.h"
#include "geelog.h"
#include "dasmtable.h"
#include "appletconfig.h"
#include "panelconfig.h"
#include "dimension.h"

int gdasm_circle_button(Gdasm* prDasm) {
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    
    return GDASM_SUCCESS;
}

int gdasm_cross_button(Gdasm* prDasm) {
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    
    return GDASM_SUCCESS;
}

int gdasm_dpad_down(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_dpad_down: Invalid Disassemble Table pointer.");
        return GDASM_FAILURE;
    }
    if (dasmtable_cursor_down(prTable) != DASMTABLE_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_dpad_down: dasmtable_cursor_down failed.");
        return GDASM_FAILURE;
    }
    return GDASM_SUCCESS;
}

int gdasm_dpad_left(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR, "gdasm_dpad_left: Invalid Disassembler "
                "Table pointer.");
        return GDASM_FAILURE;
    }
    /*
    if (dasmtable_cursor_left(prTable) != DASMTABLE_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_cursor_left: dasmtable_cursor_left failed.");
        return GDASM_FAILURE;
    }
    */
    return GDASM_SUCCESS;
}

int gdasm_dpad_right(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR, "gdasm_dpad_right: Invalid Disassembler "
                "Table pointer.");
        return GDASM_FAILURE;
    }
    /*
    if (dasmtable_cursor_right(prTable) != DASMTABLE_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_dpad_right: dasmtable_cursor_right failed.");
        return GDASM_FAILURE;
    }
    */
    return GDASM_SUCCESS;
}

int gdasm_dpad_up(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR, "gdasm_dpad_up: Invalid Disassembler "
                "Table pointer.");
        return GDASM_FAILURE;
    }
    if (dasmtable_cursor_up(prTable) != DASMTABLE_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_dpad_up: dasmtable_cursor_up failed.");
        return GDASM_FAILURE;
    }
    
    return GDASM_SUCCESS;
}

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
        } else {
            gdasm_log(prDasm, LOG_WARN, 
                    "gdasm_get_cursorcolor: Invalid Disassembler Configuration "
                    "pointer.");
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
        } else {
            gdasm_log(prDasm, LOG_WARN,
                    "gdasm_get_cursorcolumn: Invalid Cursor Position pointer.");
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
        } else {
            gdasm_log(prDasm, LOG_WARN,
                    "gdasm_get_cursorrow: Invalid Cursor Position pointer.");
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
        } else {
            gdasm_log(prDasm, LOG_WARN, 
                    "gdasm_get_cursorcolor: Invalid Disassembler Configuration "
                    "pointer.");
        }
    }
    return prColor;
}

DasmTable* gdasm_get_table(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    if (prDasm != NULL) {
        prTable = &prDasm->rTable;
    }
    return prTable;
}

PanelConfig* gdasm_get_tablepanelconfig(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    PanelConfig* prConfig = NULL;
    
    if (prDasm != NULL) {
        prTable = gdasm_get_table(prDasm);
        if (prTable != NULL) {
            prConfig = dasmtable_get_panelconfig(prTable);
        }
    }
    return prConfig;
}

CursorPos* gdasm_get_tableposition(Gdasm* prDasm) {
    CursorPos* prPos = NULL;
    PanelConfig* prCfg = NULL;
    
    if (prDasm != NULL) {
        prCfg = gdasm_get_tablepanelconfig(prDasm);
        if (prCfg != NULL) {
            prPos = panelconfig_get_position(prCfg);
        }
    }
    return prPos;
}

Dimension* gdasm_get_tablesize(Gdasm* prDasm) {
    Dimension* prSize = NULL;
    PanelConfig* prCfg = NULL;
    
    if (prDasm != NULL) {
        prCfg = gdasm_get_tablepanelconfig(prDasm);
        if (prCfg != NULL) {
            prSize = panelconfig_get_size(prCfg);
        }
    }
    return prSize;
}

int gdasm_init(Gdasm* prDasm, GeeLog* prLog) {
    DasmConfig* prCfg = NULL;
    CursorPos* prPos = NULL;
    DasmTable* prTable = NULL;
    char sMsg[GEELOG_LINELEN + 1];
    int r = 0;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    if (prLog != NULL) {
        gdasm_set_logger(prDasm, prLog);
    }
    /* Initialize the DasmConfig member. */
    prCfg = gdasm_get_config(prDasm);
    if (prCfg == NULL) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_init: Invalid Disassembler Configuration pointer.");
        return GDASM_FAILURE;
    }
    r = dasmconfig_init(prCfg);
    if (r != DASMCONFIG_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_init: Failed to initialize Disassembler Configuration.");
        return GDASM_FAILURE;
    }
    /* Initialize the CursorPos member. */
    prPos = gdasm_get_cursorpos(prDasm);
    if (prPos == NULL) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_init: Invalid Cursor Position pointer.");
        return GDASM_FAILURE;
    }
    r = cursorpos_init(prPos);
    if (r != CURSORPOS_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_init: Failed to intialize Cursor Position.");
        return GDASM_FAILURE;
    }
    /* Initialize the Disassembler Table */
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_init: Invalid Disassembler Table pointer.");
        return GDASM_FAILURE;
    }
    r = dasmtable_init(prTable);
    if (r != DASMTABLE_SUCCESS) {
        sprintf(sMsg, "gdasm_init: dasmtable_init failed (%d).", r);
        gdasm_log(prDasm, LOG_ERROR, sMsg);
        return GDASM_FAILURE;
    }
    gdasm_log(prDasm, LOG_DEBUG, "gdasm_init: Disassembler Initialized.");
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
    r = geelog_log(rLevel, sMsg);
    if (r != GEELOG_SUCCESS) {
        return GDASM_FAILURE;
    }
    return GDASM_SUCCESS;
}

int gdasm_redraw(Gdasm* prDasm) {
    DasmTable* prTable = NULL;
    int r = 0;
    
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_redraw: Invalid Disassembler Table pointer.");
        return GDASM_FAILURE;
    }
    r = dasmtable_redraw(prTable);
    if (r != DASMTABLE_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_redraw: Failed to redraw Disassembler Table.");
        return GDASM_FAILURE;
    }
    return GDASM_SUCCESS;
}

int gdasm_seek(Gdasm* prDasm, SceUInt32 address) {
    DasmTable* prTable = NULL;
    int r = 0;
    
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prTable = gdasm_get_table(prDasm);
    if (prTable == NULL) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_seek: Invalid Disassembler Table pointer.");
        return GDASM_FAILURE;
    }
    r = dasmtable_seek(prTable, address);
    if (r != DASMTABLE_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_seek: seek failed on Disassembler Table.");
        return GDASM_FAILURE;
    }
    return GDASM_SUCCESS;
}

int gdasm_set_appletconfig(Gdasm* prDasm, AppletConfig* prAcfg) {
    DasmConfig* prDcfg = NULL;
    DasmTable* prTable = NULL;
    ColorConfig* prSclr = NULL;
    ColorConfig* prDclr = NULL;
    int r = 0;
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prDcfg = gdasm_get_config(prDasm);
    if (prDcfg == NULL) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_set_appletconfig: Invalid Disassembler Configuration "
                "Pointer.");
        return GDASM_FAILURE;
    }
    r = dasmconfig_set_appletconfig(prDcfg, prAcfg);
    if (r != DASMCONFIG_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_set_appletconfig: Failed to assign AppletConfig.");
        return GDASM_FAILURE;
    }
    if (prAcfg != NULL) {
        prTable = gdasm_get_table(prDasm);
        if (prTable == NULL) {
            gdasm_log(prDasm, LOG_ERROR,
                    "gdasm_set_appletconfig: Invalid Disassembler Table "
                    "Pointer.");
            return GDASM_FAILURE;
        }
        prSclr = appletconfig_get_panelcolor(prAcfg);
        if (prSclr != NULL) {
            prDclr = dasmtable_get_panelcolor(prTable);
            if (prDclr != NULL) {
                r = colorconfig_copy(prDclr, prSclr);
                if (r != COLORCONFIG_SUCCESS) {
                    gdasm_log(prDasm, LOG_WARN, 
                            "gdasm_set_appletconfig: Failed to copy Panel "
                            "Color Configuration.");
                }
            } else {
                gdasm_log(prDasm, LOG_WARN, "gdasm_set_appletconfig: Invalid "
                        "Destination Color Configuration pointer.");
            }
            
        } else {
            gdasm_log(prDasm, LOG_WARN, 
                    "gdasm_set_appletconfig: Invalid Panel Color Configuration "
                    "pointer.");
        }
        prSclr = appletconfig_get_cursorcolor(prAcfg);
        if (prSclr != NULL) {
            prDclr = dasmtable_get_cursorcolor(prTable);
            if (prDclr != NULL) {
                r = colorconfig_copy(prDclr, prSclr);
                if (r != COLORCONFIG_SUCCESS) {
                    gdasm_log(prDasm, LOG_WARN, 
                            "gdasm_set_appletconfig: Failed to copy Cursor "
                            "Color Configuration.");
                }
            } else {
                gdasm_log(prDasm, LOG_WARN, "gdasm_set_appletconfig: Invalid "
                        "Destination Color Configuration pointer.");
            }
        } else {
            gdasm_log(prDasm, LOG_WARN, 
                    "gdasm_set_appletconfig: Invalid Cursor Color "
                    "Configuration pointer.");
        }
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

int gdasm_set_tableposition(Gdasm* prDasm, int x, int y) {
    CursorPos* prPos = NULL;
    
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prPos = gdasm_get_tableposition(prDasm);
    if (prPos == NULL) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_set_tableposition: Invalid CursorPos pointer.");
        return GDASM_FAILURE;
    }
    if (cursorpos_set(prPos, x, y) != CURSORPOS_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR, 
                "gdasm_set_tableposition: cursorpos_set failed.");
        return GDASM_FAILURE;
    }
    
    return GDASM_SUCCESS;
}

int gdasm_set_tablesize(Gdasm* prDasm, int width, int height) {
    Dimension* prSize = NULL;
    
    if (prDasm == NULL) {
        return GDASM_NULLPTR;
    }
    prSize = gdasm_get_tablesize(prDasm);
    if (prSize == NULL) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_set_tablesize: Invalid Dimension pointer.");
        return GDASM_FAILURE;
    }
    if (dimension_set(prSize, width, height) != DIMENSION_SUCCESS) {
        gdasm_log(prDasm, LOG_ERROR,
                "gdasm_set_tablesize: dimension_set failed.");
        return GDASM_FAILURE;
    }
    
    return GDASM_SUCCESS;
}
