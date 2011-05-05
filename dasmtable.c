#include <psptypes.h>
#include "dasmtable.h"
#include "dasmrow.h"
#include "cursorpos.h"

CursorPos* dasmtable_get_cursorpos(DasmTable* prTable) {
    CursorPos* prPos = NULL;
    if (prTable != NULL) {
        prPos = &prTable->rCursor;
    }
    return prPos;
}

SceUInt32 dasmtable_get_displaymask(DasmTable* prTable) {
    if (prTable == NULL) {
        return 0;
    }
    return prTable->uiDispmask;
}

int dasmtable_get_row(DasmRow* prRow, DasmTable* prTable, int row) {
    if (prTable == NULL) {
        prRow = NULL;
        return DASMTABLE_NULLPTR;
    }
    if ((row < 0) || (row >= prTable->rows)) {
        prRow = NULL;
        return DASMTABLE_INDEX;
    }
    prRow = &prTable->arRow[row];
    return DASMTABLE_SUCCESS;
}

int dasmtable_get_rows(DasmTable* prTable) {
    if (prTable == NULL) {
        return DASMTABLE_FAILURE;
    }
    return prTable->rows;
}

int dasmtable_init(DasmTable* prTable) {
    DasmRow* prRow = NULL;
    CursorPos* prCursor = NULL;
    int i = 0;
    int r = 0;
    /* Reject NULL pointers. */
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    /* Initialize the row array. */
    for (i = 0; i < DASMTABLE_MAXROWS; i++) {
        r = dasmtable_get_row(prRow, prTable, i);
        if (r != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
        r = dasmrow_init(prRow);
        if (r != DASMROW_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
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
    
    return DASMTABLE_SUCCESS;
}

int dasmtable_seek(DasmTable* prTable, SceUInt32 address) {
    DasmRow* prRow = NULL;
    int r = 0;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    r = dasmtable_get_row(prRow, prTable, 0);
    if (r != DASMTABLE_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    r = dasmrow_set_address(prRow, address);
    if (r != DASMROW_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    r = dasmtable_update(prTable);
    if (r != DASMTABLE_SUCCESS) {
        return DASMTABLE_FAILURE;
    }
    return DASMTABLE_SUCCESS;
}

int dasmtable_set_cursor(DasmTable* prTable, int col, int row) {
    CursorPos* prCursor = NULL;
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prCursor = dasmtable_get_cursorpos(prTable);
    if (prCursor == NULL) {
        return DASMTABLE_FAILURE;
    }
    if ((row < 0) || (row >= prTable->rows)) {
        return DASMTABLE_INVPOS;
    }
    if ((col < 0) || (col >= 2)) {
        return DASMTABLE_INVPOS;
    }
    prCursor->x = col;
    prCursor->y = row;
    return DASMTABLE_SUCCESS;
}

int dasmtable_set_rows(DasmTable* prTable, int rows) {
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    prTable->rows = rows;
    return DASMTABLE_SUCCESS;
}

SceUInt32 dasmtable_tell(DasmTable* prTable) {
    DasmRow* prRow = NULL;
    SceUInt32 addr = 0;
    int r = 0;
    
    if (prTable != NULL) {
        r = dasmtable_get_row(prRow, prTable, 0);
        if (r == DASMTABLE_SUCCESS) {
            addr = dasmrow_get_address(prRow);
        }
    }
    return addr;
}

int dasmtable_update(DasmTable* prTable) {
    DasmRow* prRow = NULL;
    int iRow = 0;
    int iRows = 0;
    int r = 0;
    SceUInt32 page = 0;
    SceUInt32 addr = 0;
    
    if (prTable == NULL) {
        return DASMTABLE_NULLPTR;
    }
    iRows = dasmtable_get_rows(prTable);
    if (iRows < 0) {
        return DASMTABLE_FAILURE;
    }
    page = dasmtable_tell(prTable);
    if (page == 0) {
        return DASMTABLE_FAILURE;
    }
    for (iRow = 0; iRow < iRows; iRow++) {
        r = dasmtable_get_row(prRow, prTable, iRow);
        if (r != DASMTABLE_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
        addr = page + (iRow * 4);
        r = dasmrow_set_address(prRow, addr);
        if (r != DASMROW_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
        r = dasmrow_update(prRow);
        if (r != DASMROW_SUCCESS) {
            return DASMTABLE_FAILURE;
        }
    }
    return DASMTABLE_SUCCESS;
}
