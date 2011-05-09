#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <psputils.h>
#include <psputilsforkernel.h>
#include "colorconfig.h"
#include "dimension.h"
#include "cursorpos.h"
#include "addresscolumn.h"
#include "bytecolumn.h"
#include "asciicolumn.h"
#include "appletconfig.h"
#include "hexeditor.h"

static int commitSelectedByte(HexEditor* prHex);
static int drawAddressColumn(HexEditor* prHex, int iRow);
static int drawAsciiColumn(HexEditor* prHex, int iRow);
static int drawCursor(HexEditor* prHex);
static int drawDataColumns(HexEditor* prHex, int iRow);
static int drawTable(HexEditor* prHex);
static int drawTableRow(HexEditor* prHex, int iRow);
static int editSelectedChar(HexEditor* prHex);
static SceUInt32 getPhysicalOffset(HexEditor* prHex);
static SceUInt32 getRowOffset(HexEditor* prHex, int iRow);
static SceUInt32 getSelectedByteOffset(HexEditor* prHex);
static SceUChar8 getSelectedByteValue(HexEditor* prHex);
static SceChar8 getSelectedCharValue(HexEditor* prHex);
static SceUInt32 getSelectedRowAddress(HexEditor* prHex);
static SceUInt32 getSelectedRowOffset(HexEditor* prHex);

static int commitSelectedByte(HexEditor* prHex) {
    ByteColumn *prCol = NULL;
    SceUChar8 value = 0;
    SceUChar8* pucDest = NULL;
    SceUInt32 offset = 0;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prCol = &(prHex->rEditRow.rByte);
    value = (SceUChar8)(bytecolumn_commit(prCol) & 0xFF);
    offset = getSelectedByteOffset(prHex);
    pucDest = (SceUChar8*)offset;
    *pucDest = value;
    sceKernelDcacheWritebackInvalidateRange(pucDest, 1);
    sceKernelIcacheInvalidateRange(pucDest, 1);
    prHex->editing = 0;
    prHex->dirty = 1;
    return HEXEDITOR_SUCCESS;
}

static int commitSelectedChar(HexEditor* prHex) {
    AsciiColumn* prCol = NULL;
    SceChar8 value = (SceChar8)0;
    SceChar8* pcDest = NULL;
    SceUInt32 offset = 0;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prCol = &(prHex->rEditRow.rAscii);
    value = asciicolumn_commit(prCol);
    offset = getSelectedByteOffset(prHex);
    pcDest = (SceChar8*)offset;
    *pcDest = value;
    sceKernelDcacheWritebackInvalidateRange(pcDest, 1);
    sceKernelIcacheInvalidateRange(pcDest, 1);
    prHex->editing = 0;
    prHex->dirty = 1;
    return HEXEDITOR_SUCCESS;
}

static int drawAddressColumn(HexEditor* prHex, int iRow) {
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prApCfg = prHex->prApCfg;
    SceUInt32 poff = getPhysicalOffset(prHex);
    SceUInt32 base = prHex->config.base_address;
    SceUInt32 vbase = base + poff;
    int bpl = prHex->config.bytes_per_line;
    SceUInt32 vaddr = vbase + (iRow * bpl);
    prColor = &prHex->config.address;
    int y = prHex->cursor.y;
    if (iRow == y) {
        prColor = &prApCfg->rPanel.rCursor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("0x%08X", vaddr);

    return HEXEDITOR_SUCCESS;
}

static int drawAsciiColumn(HexEditor* prHex, int iRow) {
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prApCfg = prHex->prApCfg;
    int i;
    char abuf[17];
    int bpl = prHex->config.bytes_per_line;
    SceChar8 *pVal = (SceChar8*)(prHex->offset + (iRow * bpl));
    for (i = 0; i < bpl; i++) {
        if (pVal[i] >= 0x20) {
            abuf[i] = pVal[i];
        } else {
            abuf[i] = '.';
        }
    }
    abuf[bpl] = (char)0;
    prColor = &prHex->config.ascii;
    if (iRow == prHex->cursor.y) {
        prColor = &prApCfg->rPanel.rColor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf("%16s", abuf);
    return HEXEDITOR_SUCCESS;
}

static int drawDataColumns(HexEditor* prHex, int iRow) {
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prApCfg = prHex->prApCfg;
    int bpl = prHex->config.bytes_per_line;
    SceUChar8 *pVal = (SceUChar8*)(prHex->offset + (iRow * bpl));
    prColor = &prHex->config.datacolumns;
    if (iRow == prHex->cursor.y) {
        prColor = &prApCfg->rPanel.rColor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    if (bpl == 16) {
        pspDebugScreenKprintf("%02X%02X%02X%02X ",
                pVal[0], pVal[1], pVal[2], pVal[3]);
        pspDebugScreenKprintf("%02X%02X%02X%02X ",
                pVal[4], pVal[5], pVal[6], pVal[7]);
        pspDebugScreenKprintf("%02X%02X%02X%02X ",
                pVal[8], pVal[9], pVal[10], pVal[11]);
        pspDebugScreenKprintf("%02X%02X%02X%02X",
                pVal[12], pVal[13], pVal[14], pVal[15]);
    }
    if (bpl == 8) {
        pspDebugScreenKprintf("%02X %02X %02X %02X %02X %02X %02X %02X",
                pVal[0], pVal[1], pVal[2], pVal[3],
                pVal[4], pVal[5], pVal[6], pVal[7]);
    }
    return HEXEDITOR_SUCCESS;
}

static int drawCursor(HexEditor* prHex) {
    AddressColumn* prCol = NULL;
    ByteColumn* prBcol = NULL;
    AsciiColumn* prAcol = NULL;
    SceChar8* pcVal = NULL;
    SceUChar8* pucVal = NULL;
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prEdit = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prApCfg = prHex->prApCfg;
    int x = prHex->cursor.x;
    int y = prHex->cursor.y;
    int tt = prHex->config.table_pos.y;
    int iDcols = prHex->config.bytes_per_line;
    SceUInt32 poff = hexEditorTell(prHex);
    int iRow = prHex->cursor.y;
    SceUInt32 roff = getRowOffset(prHex, iRow);
    int iCol = prHex->cursor.x;
    int iDcol = 0;
    int iDx = 0;
    int iDigit = 0;

    if (x == 0) {
        prCol = &(prHex->rEditRow.rAddress);
        prColor = &prApCfg->rPanel.rCursor;
        colorconfig_setcolor(&prCol->color, prColor->background, prColor->text);
        prEdit = &prApCfg->rPanel.rEdit;
        colorconfig_setcolor(&prCol->edit, prEdit->background, prColor->text);
        colorconfig_setcolor(&prCol->editdigit, prEdit->background,
                prEdit->text);
        prCol->prefix = 1;
        if (prCol->inedit == 0) {
            addresscolumn_setvalue(prCol, getSelectedRowAddress(prHex));
        }
        pspDebugScreenSetXY(x, tt + y);
        addresscolumn_redraw(prCol);
    }
    if (x > 0 && x <= iDcols) {
        prBcol = &(prHex->rEditRow.rByte);
        prColor = &prApCfg->rPanel.rCursor;
        colorconfig_setcolor(&prBcol->color, prColor->background,
                prColor->text);
        prBcol->prefix = 0;
        iDcol = x - 1;
        if (prBcol->inedit == 0) {
            pucVal = (SceUChar8*) (poff + roff + iDcol);
            prBcol->value = (unsigned int)*pucVal;
        }
        if (iDcol >= 0 && iDcol < 4) {
            iDx = 11 + (iDcol * 2);
        }
        if (iDcol >= 4 && iDcol < 8) {
            iDigit = iDcol - 4;
            iDx = 20 + (iDigit * 2);
        }
        if (iDcol >= 8 && iDcol < 12) {
            iDigit = iDcol - 8;
            iDx = 29 + (iDigit * 2);
        }
        if (iDcol >= 12 && iDcol < 16) {
            iDigit = iDcol - 12;
            iDx = 38 + (iDigit * 2);
        }
        pspDebugScreenSetXY(iDx, tt + y);
        bytecolumn_redraw(prBcol);
    }
    if (x > iDcols && x <= (iDcols * 2)) {
        prAcol = &(prHex->rEditRow.rAscii);
        prColor = &prApCfg->rPanel.rCursor;
        colorconfig_setcolor(&prAcol->color, prColor->background,
                prColor->text);
        iDcol = x - iDcols - 1;
        pcVal = (SceChar8*) (poff + roff + iDcol);
        if (prAcol->in_edit == 0) {
            asciicolumn_setvalue(prAcol, *pcVal);
        }
        iDx = 47 + iDcol;
        pspDebugScreenSetXY(iDx, tt + y);
        asciicolumn_redraw(prAcol);
    }
    return HEXEDITOR_SUCCESS;
}

static int drawTable(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    int i = 0;

    for (i = 0; i < prHex->config.table_size.height; i++) {
        drawTableRow(prHex, i);
    }
    return HEXEDITOR_SUCCESS;
}

static int drawTableRow(HexEditor* prHex, int iRow) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    int tt = prHex->config.table_pos.y;
    int tl = prHex->config.table_pos.x;
    pspDebugScreenSetXY(tl, tt + iRow);
    drawAddressColumn(prHex, iRow);
    pspDebugScreenPuts(" ");
    drawDataColumns(prHex, iRow);
    pspDebugScreenPuts(" ");
    drawAsciiColumn(prHex, iRow);
    return HEXEDITOR_SUCCESS;
}

static int editSelectedChar(HexEditor* prHex) {
    AsciiColumn* prCol = NULL;
    char value = (char)0;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    prCol = &(prHex->rEditRow.rAscii);
    value = getSelectedCharValue(prHex);
    asciicolumn_edit(prCol, value);
    prHex->editing = 1;
    prHex->dirty = 1;

    return HEXEDITOR_SUCCESS;
}

static SceUInt32 getPhysicalAddress(HexEditor* prHex) {
    SceUInt32 addr = 0;
    if (prHex != NULL) {
        addr = prHex->config.base_address + getPhysicalOffset(prHex);
    }
    return addr;
}

static SceUInt32 getPhysicalOffset(HexEditor* prHex) {
    SceUInt32 poff = 0;
    if (prHex != NULL) {
        poff = prHex->offset - prHex->config.min_offset;
    }
    return poff;
}

static SceUInt32 getRowOffset(HexEditor* prHex, int iRow) {
    SceUInt32 offset = 0;
    int bpr = 0;

    if (prHex != NULL) {
        bpr = prHex->config.bytes_per_line;
        offset = iRow * bpr;
    }
    return offset;
}

static SceUInt32 getSelectedByteOffset(HexEditor* prHex) {
    SceUInt32 offset = 0;
    SceUInt32 roff = 0;
    int x = 0;
    int col = 0;
    int bpl = 0;
    if (prHex != NULL) {
        roff = getSelectedRowOffset(prHex);
        bpl = prHex->config.bytes_per_line;
        x = prHex->cursor.x;

        if (x > 0 && x <= bpl) {
            col = x - 1;
            offset = roff + col;
        }
        if (x > bpl && x <= bpl * 2) {
            col = x - bpl - 1;
            offset = roff + col;
        }
    }
    return offset;
}

static SceUChar8 getSelectedByteValue(HexEditor* prHex) {
    SceUChar8 value = 0;
    SceUInt32 offset = 0;
    SceUChar8 *pVal = NULL;
    if (prHex != NULL) {
        offset = getSelectedByteOffset(prHex);
        if (offset != 0) {
            pVal = (SceUChar8*)(offset);
            value = *pVal;
        }
    }
    return value;
}

static SceChar8 getSelectedCharValue(HexEditor* prHex) {
    SceChar8 value = (SceChar8)0;
    SceUInt32 offset = 0;
    SceChar8* pVal = NULL;
    if (prHex != NULL) {
        offset = getSelectedByteOffset(prHex);
        if (offset != 0) {
            pVal = (SceChar8*)offset;
            value = *pVal;
        }
    }
    return value;
}

static SceUInt32 getSelectedRowAddress(HexEditor* prHex) {
    SceUInt32 addr = 0;
    SceUInt32 base = 0;
    SceUInt32 offset = 0;
    int delta = 0;
    if (prHex != NULL) {
        base = prHex->config.base_address;
        offset = getSelectedRowOffset(prHex);
        delta = offset - prHex->config.min_offset;
        addr = base + delta;
    }
    return addr;
}

static SceUInt32 getSelectedRowOffset(HexEditor* prHex) {
    SceUInt32 offset = 0;
    int bpl = 0;
    int y = 0;
    if (prHex != NULL) {
        bpl = prHex->config.bytes_per_line;
        y = prHex->cursor.y;
        offset = prHex->offset + (y * bpl);
    }
    return offset;
}

int hexeditorCircleButton(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }

    return HEXEDITOR_SUCCESS;
}

int hexeditorCrossButton(HexEditor* prHex) {
    AddressColumn *prAddr = NULL;
    ByteColumn *prByte = NULL;
    SceUInt32 addr = 0;
    SceUChar8 bval = 0;
    int x = 0;
    int bpl = 0;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    x = prHex->cursor.x;
    bpl = prHex->config.bytes_per_line;
    if (prHex->editing == 0) {
        if (x == 0) {
            prAddr = &(prHex->rEditRow.rAddress);
            addr = getSelectedRowAddress(prHex);
            addresscolumn_edit(prAddr, addr);
            prHex->editing = 1;
            prHex->dirty = 1;
        }
        if (x > 0 && x <= bpl) {
            prByte = &(prHex->rEditRow.rByte);
            bval = getSelectedByteValue(prHex);
            bytecolumn_edit(prByte, bval);
            prHex->editing = 1;
            prHex->dirty = 1;
        }
        if (x > bpl && x <= bpl * 2) {
            editSelectedChar(prHex);
        }
    } else {
        if (x == 0) {
            prAddr = &(prHex->rEditRow.rAddress);
            addresscolumn_commit(prAddr);
            prHex->editing = 0;
            prHex->dirty = 1;
        }
        if (x > 0 && x <= bpl) {
            commitSelectedByte(prHex);
        }
        if (x > bpl && x <= bpl * 2) {
            commitSelectedChar(prHex);
        }
    }

    return HEXEDITOR_SUCCESS;
}

int hexeditorCursorDown(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    int y = prHex->cursor.y;
    y++;
    if (y >= prHex->config.table_size.height) {
        y = prHex->config.table_size.height - 1;
        hexeditorScrollDown(prHex);
    }
    if (y != prHex->cursor.y) {
        prHex->cursor.y = y;
        prHex->dirty = 1;
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorCursorLeft(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    int x = prHex->cursor.x;
    int bpl = prHex->config.bytes_per_line;
    int cols = (bpl * 2) + 1;
    x--;
    if (x < 0) {
        x = cols;
    }
    if (x != prHex->cursor.x) {
        prHex->cursor.x = x;
        prHex->dirty = 1;
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorCursorRight(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    int x = prHex->cursor.x;
    int bpl = prHex->config.bytes_per_line;
    int cols = (bpl * 2) + 1;
    x++;
    if (x >= cols) {
        x = 0;
    }
    if (x != prHex->cursor.x) {
        prHex->cursor.x = x;
        prHex->dirty = 1;
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorCursorUp(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    int y = prHex->cursor.y;
    y--;
    if (y < 0) {
        hexeditorScrollUp(prHex);
        y = 0;
    }
    if (y != prHex->cursor.y) {
        prHex->cursor.y = y;
        prHex->dirty = 1;
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorDpadDown(HexEditor* prHex) {
    int col = 0;
    int bpl = 0;
    int incr = 0;
    SceUInt32 addr = 0;
    SceUInt32 off;
    AddressColumn* prAddr = NULL;
    ByteColumn* prByte = NULL;
    AsciiColumn* prAscii = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    if (prHex->editing == 0) {
        hexeditorCursorDown(prHex);
    } else {
        bpl = prHex->config.bytes_per_line;
        col = prHex->cursor.x;
        if (col == 0) {
            prAddr = &(prHex->rEditRow.rAddress);
            addresscolumn_decrement(prAddr);
            addr = getSelectedRowAddress(prHex);
            if (addr != prAddr->value) {
                incr = addresscolumn_getincrement(prAddr);
                off = hexEditorTell(prHex);
                hexeditorSeek(prHex, off - incr);
            }
            prHex->dirty = 1;
        }
        if (col > 0 && col <= bpl) {
            prByte = &(prHex->rEditRow.rByte);
            bytecolumn_decrement(prByte);
            prHex->dirty = 1;
        }
        if (col > bpl && col <= bpl * 2) {
            prAscii = &(prHex->rEditRow.rAscii);
            asciicolumn_cursor_down(prAscii);
            prHex->dirty = 1;
        }
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorDpadLeft(HexEditor* prHex) {
    int col = 0;
    int bpl = 0;
    AddressColumn* prAddr = NULL;
    ByteColumn* prByte = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    if (prHex->editing == 0) {
        hexeditorCursorLeft(prHex);
    } else {
        bpl = prHex->config.bytes_per_line;
        col = prHex->cursor.x;
        if (col == 0) {
            prAddr = &(prHex->rEditRow.rAddress);
            addresscolumn_prevdigit(prAddr);
            prHex->dirty = 1;
        }
        if (col > 0 && col <= bpl) {
            prByte = &(prHex->rEditRow.rByte);
            bytecolumn_prevdigit(prByte);
            prHex->dirty = 1;
        }
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorDpadRight(HexEditor* prHex) {
    int col = 0;
    int bpl = 0;
    AddressColumn* prAddr = NULL;
    ByteColumn* prByte = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    if (prHex->editing == 0) {
        hexeditorCursorRight(prHex);
    } else {
        bpl = prHex->config.bytes_per_line;
        col = prHex->cursor.x;
        if (col == 0) {
            prAddr = &(prHex->rEditRow.rAddress);
            addresscolumn_nextdigit(prAddr);
            prHex->dirty = 1;
        }
        if (col > 0 && col <= bpl) {
            prByte = &(prHex->rEditRow.rByte);
            bytecolumn_nextdigit(prByte);
            prHex->dirty = 1;
        }
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorDpadUp(HexEditor* prHex) {
    int col = 0;
    int bpl = 0;
    int incr = 0;
    SceUInt32 off = 0;
    SceUInt32 addr = 0;
    AddressColumn* prAddr = NULL;
    ByteColumn* prByte = NULL;
    AsciiColumn* prAscii = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    if (prHex->editing == 0) {
        hexeditorCursorUp(prHex);
    } else {
        bpl = prHex->config.bytes_per_line;
        col = prHex->cursor.x;
        if (col == 0) {
            prAddr = &(prHex->rEditRow.rAddress);
            addresscolumn_increment(prAddr);
            off = hexEditorTell(prHex);
            addr = getSelectedRowAddress(prHex);
            if (addr != prAddr->value) {
                incr = addresscolumn_getincrement(prAddr);
                hexeditorSeek(prHex, off + incr);
            }
            prHex->dirty = 1;
        }
        if (col > 0 && col <= bpl) {
            prByte = &(prHex->rEditRow.rByte);
            bytecolumn_increment(prByte);
            prHex->dirty = 1;
        }
        if (col > bpl && col <= bpl * 2) {
            prAscii = &(prHex->rEditRow.rAscii);
            asciicolumn_cursor_up(prAscii);
            prHex->dirty = 1;
        }
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorInit(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    HexEditorConfig *prCfg = &prHex->config;
    ColorConfig *prCc = &prCfg->color;
    prCc->background = HEXEDITOR_COLORBG;
    prCc->text = HEXEDITOR_COLORTXT;
    prCc = &prCfg->address;
    prCc->background = HEXEDITOR_COLORBG;
    prCc->text = HEXEDITOR_COLORTXT;
    prCc = &prCfg->datacolumns;
    prCc->background = HEXEDITOR_COLORBG;
    prCc->text = HEXEDITOR_COLORTXT;
    prCc = &prCfg->ascii;
    prCc->background = HEXEDITOR_COLORBG;
    prCc->text = HEXEDITOR_COLORTXT;
    prCc = &prCfg->cursor;
    prCc->background = HEXEDITOR_COLORTXT;
    prCc->text = HEXEDITOR_COLORBG;
    prCc = &prCfg->edit;
    prCc->background = HEXEDITOR_COLORTXT;
    prCc->text = HEXEDITOR_COLORBG;
    Dimension *prDim = &prCfg->table_size;
    prDim->height = 20;
    prDim->width = 33;
    CursorPos *prPos = &prCfg->table_pos;
    prPos->x = 0;
    prPos->y = 2;
    prPos = &prCfg->position;
    prPos->x = 0;
    prPos->y = 1;
    prCfg->min_offset = HEXEDITOR_DEFMINOFF;
    prCfg->max_offset = HEXEDITOR_DEFMAXOFF;
    prCfg->base_address = HEXEDITOR_DEFBASE;
    prCfg->bytes_per_line = HEXEDITOR_DEFBPL;
    prPos = &prHex->cursor;
    prPos->x = 0;
    prPos->y = 0;
    prHex->offset = prCfg->min_offset;

    AddressColumn* prAdc = &(prHex->rEditRow.rAddress);
    addresscolumn_init(prAdc);

    ByteColumn* prByc = &(prHex->rEditRow.rByte);
    bytecolumn_init(prByc);

    AsciiColumn* prAsc = &(prHex->rEditRow.rAscii);
    asciicolumn_init(prAsc);
    
    return HEXEDITOR_SUCCESS;
}

int hexeditorPageDown(HexEditor* prHex) {
    SceUInt32 offset = 0;
    int lpp = 0;
    int bpl = 0;
    int psize = 0;
    SceUInt32 endp = 0;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }

    offset = hexEditorTell(prHex);
    lpp = prHex->config.table_size.height;
    bpl = prHex->config.bytes_per_line;
    psize = lpp * bpl;
    endp = prHex->config.max_offset - psize;
    if (offset + psize >= endp) {
        offset = endp;
    } else {
        offset += psize;
    }
    hexeditorSeek(prHex, offset);

    return HEXEDITOR_SUCCESS;
}

int hexeditorPageUp(HexEditor* prHex) {
    SceUInt32 offset = 0;
    int lpp = 0;
    int bpl = 0;
    int psize = 0;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    offset = hexEditorTell(prHex);
    lpp = prHex->config.table_size.height;
    bpl = prHex->config.bytes_per_line;
    psize = lpp * bpl;
    if (offset - psize < prHex->config.min_offset) {
        offset = prHex->config.min_offset;
    } else {
        offset -= psize;
    }
    hexeditorSeek(prHex, offset);

    return HEXEDITOR_SUCCESS;
}

int hexeditorRedraw(HexEditor* prHex) {
    AppletConfig* prApCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    if (prHex->dirty == 0) {
        return;
    }
    prApCfg = prHex->prApCfg;
    prColor = &prApCfg->rPanel.rColor;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(prHex->config.position.x, prHex->config.position.y);
    pspDebugScreenKprintf("%-67s", "Hex Editor");
    drawTable(prHex);
    drawCursor(prHex);
    prHex->dirty = 0;
    return HEXEDITOR_SUCCESS;
}

int hexeditorScrollDown(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    SceUInt32 offset = prHex->offset;
    int lpp = prHex->config.table_size.height;
    int bpl = prHex->config.bytes_per_line;
    int lpa = prHex->config.max_offset - (lpp * bpl);
    offset += bpl;
    if (offset >= lpa) {
        offset = lpa;
    }
    if (offset != prHex->offset) {
        return hexeditorSeek(prHex, offset);
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorScrollUp(HexEditor* prHex) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    SceUInt32 offset = prHex->offset;
    int bpl = prHex->config.bytes_per_line;
    offset -= bpl;
    if (offset < prHex->config.min_offset) {
        offset = prHex->config.min_offset;
    }
    if (offset != prHex->offset) {
        return hexeditorSeek(prHex, offset);
    }
    return HEXEDITOR_SUCCESS;
}

int hexeditorSeek(HexEditor* prHex, SceUInt32 offset) {
    if (prHex == NULL) {
        return HEXEDITOR_MEMORY;
    }
    if (offset < prHex->config.min_offset ||
            offset > prHex->config.max_offset) {
        return HEXEDITOR_BADADDR;
    }
    prHex->offset = offset;
    prHex->dirty = 1;
    return HEXEDITOR_SUCCESS;
}

SceUInt32 hexEditorTell(HexEditor* prHex) {
    SceUInt32 pos = 0;
    if (prHex != NULL) {
        pos = prHex->offset;
    }
    return pos;
}
