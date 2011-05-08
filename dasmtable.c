#include <psptypes.h>
#include <pspdebug.h>
#include "dasmtable.h"
#include "cursorpos.h"
#include "dasmmodel.h"
#include "panelconfig.h"
#include "colorconfig.h"

static int render_row(DasmTable* prTable, int row);

int dasmtable_cursor_down(DasmTable* prTable) {
    int row = 0;
    int rows = 0;
    int nrow = 0;
    
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Get the selected Row index. */
    row = dasmtable_get_selrow(prTable);
    if (row < 0) {
        return DASMTABLE_FAILURE;
    }
    /* Get the number of Rows in the Table. */
    rows = dasmtable_get_rows(prTable);
    if (rows < 0) {
        return DASMTABLE_FAILURE;
    }
    /* Compute the next Row index. */
    nrow = row + 1;
    /* If the next Row index is past the end of the Table */
    if (nrow >= rows) {
        /* Scroll the Table down by 1 row. */
        if (dasmtable_scroll_down(prTable, 1) != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
        /* Reset the Row index. */
        nrow = row;
    }
    
    if (nrow != row) {
        if (dasmtable_set_selrow(prTable, nrow) != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
    }
    return DASMTABLE_SUCCESS;
}

int dasmtable_cursor_up(DasmTable* prTable) {
    int row = 0;
    int nrow = 0;
    
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Get the selected Row index. */
    row = dasmtable_get_selrow(prTable);
    if (row < 0) {
        return DASMTABLE_FAILURE;
    }
    /* Compute the next Row index. */
    nrow = row - 1;
    /* If the previous Row is past the beginning of the Table. */
    if (nrow < 0) {
        /* Scroll the Table up by 1 row. */
        if (dasmtable_scroll_up(prTable, 1) != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
        /* Reset the Row index. */
        nrow = row;
    }
    
    if (nrow != row) {
        if (dasmtable_set_selrow(prTable, nrow) != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
    }
    return DASMTABLE_SUCCESS;
}

ColorConfig* dasmtable_get_cursorcolor(DasmTable* prTable) {
    ColorConfig* prColor = NULL;
    PanelConfig* prCfg = NULL;
    
    if (prTable != NULL) {
        prCfg = dasmtable_get_panelconfig(prTable);
        if (prCfg != NULL) {
            prColor = panelconfig_get_cursorcolor(prCfg);
        }
    }
    return prColor;
}

CursorPos* dasmtable_get_cursorpos(DasmTable* prTable) {
    CursorPos* prPos = NULL;
    if (prTable != NULL) {
        prPos = &prTable->rCursor;
    }
    return prPos;
}

DasmModel* dasmtable_get_model(DasmTable* prTable) {
    DasmModel* prModel = NULL;
    if (prTable != NULL) {
        prModel = &prTable->rModel;
    }
    return prModel;
}

ColorConfig* dasmtable_get_panelcolor(DasmTable* prTable) {
    ColorConfig* prColor = NULL;
    PanelConfig* prCfg = NULL;
    
    if (prTable != NULL) {
        prCfg = dasmtable_get_panelconfig(prTable);
        if (prCfg != NULL) {
            prColor = panelconfig_get_panelcolor(prCfg);
        }
    }
    return prColor;
}

PanelConfig* dasmtable_get_panelconfig(DasmTable* prTable) {
    PanelConfig* prCfg = NULL;
    if (prTable != NULL) {
        prCfg = &prTable->rPanelCfg;
    }
    return prCfg;
}

CursorPos* dasmtable_get_position(DasmTable* prTable) {
    PanelConfig* prCfg = NULL;
    CursorPos* prPos = NULL;
    
    if (prTable != NULL) {
        prCfg = dasmtable_get_panelconfig(prTable);
        if (prCfg != NULL) {
            prPos = panelconfig_get_position(prCfg);
        }
    }
    return prPos;
}

int dasmtable_get_rows(DasmTable* prTable) {
    DasmModel* prModel = NULL;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prModel = dasmtable_get_model(prTable);
    if (prModel == NULL) {
        return DASMTABLE_FAILURE;
    }
    return prModel->iRows;
}

int dasmtable_get_selcol(DasmTable* prTable) {
    CursorPos* prCursor = NULL;
    int col = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prCursor = dasmtable_get_cursorpos(prTable);
    if (prCursor == NULL) {
        return DASMTABLE_FAILURE;
    }
    col = prCursor->x;
    return col;
}

int dasmtable_get_selrow(DasmTable* prTable) {
    CursorPos* prCursor = NULL;
    int row = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prCursor = dasmtable_get_cursorpos(prTable);
    if (prCursor == NULL) {
        return DASMTABLE_FAILURE;
    }
    row = prCursor->y;
    return row;
}

int dasmtable_init(DasmTable* prTable) {
    CursorPos* prCursor = NULL;
    DasmModel* prModel = NULL;
    PanelConfig* prPcfg = NULL;
    int r = 0;
    /* Reject NULL pointers. */
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Initialize the DasmModel */
    prModel = dasmtable_get_model(prTable);
    if (prModel == NULL) {
        return DASMTABLE_FAILURE;
    }
    r = dasmmodel_init(prModel);
    if (r != DASMMODEL_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    
    /* Initialize the CursorPos. */
    prCursor = dasmtable_get_cursorpos(prTable);
    if (prCursor == NULL) {
        return DASMTABLE_FAILURE;
    }
    r = cursorpos_init(prCursor);
    if (r != CURSORPOS_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    
    /* Initialize the PanelConfig. */
    prPcfg = dasmtable_get_panelconfig(prTable);
    if (prPcfg == NULL) {
        return DASMTABLE_FAILURE;
    }
    r = panelconfig_init(prPcfg);
    if (r != PANELCONFIG_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    
    /* Assign the default Row Format. */
    if (dasmtable_set_rowformat(prTable, DASMTABLE_DEFFMT) != DASMTABLE_SUCCESS)
    {
        return DASMTABLE_FAILURE;
    }
    
    /* Invalidate the Table. */
    if (dasmtable_invalidate(prTable) != DASMTABLE_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    
    return DASMTABLE_SUCCESS;
}

int dasmtable_invalidate(DasmTable* prTable) {
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prTable->iPanelDirty = 1;
    return DASMTABLE_SUCCESS;
}

int dasmtable_redraw(DasmTable* prTable) {
    int rows = 0;
    int row = 0;
    int r = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Don't redraw a panel that isn't dirty. */
    if (prTable->iPanelDirty == 0) {
        return DASMTABLE_SUCCESS;
    }
    rows = dasmtable_get_rows(prTable);
    if (rows < 0) {
        return DASMTABLE_FAILURE;
    }
    for (row = 0; row < rows; row++) {
        r = render_row(prTable, row);
        if (r != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
    }
    prTable->iPanelDirty = 0;
    return DASMTABLE_SUCCESS;
}

int dasmtable_scroll_down(DasmTable* prTable, const unsigned int rows) {
    SceUInt32 address = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    address = dasmtable_tell(prTable);
    address += rows * 4;
    if (dasmtable_seek(prTable, address) != DASMTABLE_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    return DASMTABLE_SUCCESS;
}

int dasmtable_scroll_up(DasmTable* prTable, const unsigned int rows) {
    SceUInt32 address = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    address = dasmtable_tell(prTable);
    address -= rows * 4;
    if (dasmtable_seek(prTable, address) != DASMTABLE_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    return DASMTABLE_SUCCESS;
}

int dasmtable_seek(DasmTable* prTable, SceUInt32 address) {
    DasmModel* prModel = NULL;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prModel = dasmtable_get_model(prTable);
    if (prModel == NULL) {
        return DASMTABLE_FAILURE;
    }
    if (dasmmodel_set_address(prModel, address) != DASMMODEL_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    if (dasmtable_invalidate(prTable) != DASMTABLE_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    return DASMTABLE_SUCCESS;
}

int dasmtable_set_rowformat(DasmTable* prTable, const char* sFormat) {
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    strcpy(prTable->sRowFmt, sFormat);
    return DASMTABLE_SUCCESS;
}

int dasmtable_set_selrow(DasmTable* prTable, const unsigned int row) {
    CursorPos* prCursor = NULL;
    int oldRow = 0;
    int rows = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Get the number of Rows in the Table. */
    rows = dasmtable_get_rows(prTable);
    if (rows < 0) {
        return DASMTABLE_FAILURE;
    }
    /* Reject illegal Row indexes. */
    if ((row < 0) || (row >= rows)) {
        return DASMTABLE_INVPOS;
    }
    /* Get the current Cursor Position. */
    prCursor = dasmtable_get_cursorpos(prTable);
    if (prCursor == NULL) {
        return DASMTABLE_FAILURE;
    }
    /* Get the old selected Row. */
    oldRow = prCursor->y;
    if (oldRow >= 0) {
        /* If the old selected row is not the current row */
        if (oldRow != row) {
            /* Assign the Cursor Position. */
            prCursor->y = row;
            /* Redraw the old Row. */
            if (render_row(prTable, oldRow) != DASMTABLE_SUCCESS) {
                return DASMTABLE_FAILURE;
            }
            /* Redraw the current Row. */
            if (render_row(prTable, row) != DASMTABLE_SUCCESS) {
                return DASMTABLE_FAILURE;
            }
        }
    }
    return DASMTABLE_SUCCESS;
}

SceUInt32 dasmtable_tell(DasmTable* prTable) {
    DasmModel* prModel = NULL;
    SceUInt32 addr = 0;
    if (prTable != NULL) {
        prModel = dasmtable_get_model(prTable);
        if (prModel != NULL) {
            addr = dasmmodel_get_address(prModel);
        }
    }
    return addr;
}

/** Render the specified row of a Disassembler Table.
 * 
 * @param prTable Pointer to a DasmTable struct representing the Disassembler
 * Table.
 * @param row int containing the index of the row to render.
 * @return 0 indicates success, less than 0 indicates failure.
 */
static int render_row(DasmTable* prTable, int row) {
    CursorPos* prTop = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prColor = NULL;
    DasmModel* prModel = NULL;
    int rowY = 0;
    int r = 0;
    char sAddress[DASMTABLE_FMTLEN + 1];
    char sValue[DASMTABLE_FMTLEN + 1];
    char sAssembly[DASMTABLE_FMTLEN + 1];
    
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Get the Top Left Corner of the Table */
    prTop = dasmtable_get_position(prTable);
    if (prTop == NULL) {
        return DASMTABLE_FAILURE;
    }
    /* Compute the Y coordinate of the specified row. */
    rowY = prTop->y + row;
    pspDebugScreenSetXY(prTop->x, rowY);
    
    /* Get the Current Cursor Position */
    prCursor = dasmtable_get_cursorpos(prTable);
    if (prCursor == NULL) {
        return DASMTABLE_FAILURE;
    }
    
    /* Assign the color */
    prColor = dasmtable_get_panelcolor(prTable);
    if (row == prCursor->y) {
        prColor = dasmtable_get_cursorcolor(prTable);
    }
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    
    /* Get the Model */
    prModel = dasmtable_get_model(prTable);
    if (prModel == NULL) {
        return DASMTABLE_FAILURE;
    }
    
    /* Get the display values from the Model. */
    if (dasmmodel_get_value(sAddress, prModel, row, 0) != DASMMODEL_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    if (dasmmodel_get_value(sValue, prModel, row, 1) != DASMMODEL_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    if (dasmmodel_get_value(sAssembly, prModel, row, 2) != DASMMODEL_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    
    /* Render the Row to the debug screen. */
    pspDebugScreenKprintf(prTable->sRowFmt, sAddress, sValue, sAssembly);
    
    return DASMTABLE_SUCCESS;
}
