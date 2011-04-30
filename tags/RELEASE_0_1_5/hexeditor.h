/* 
 * File:   hexeditor.h
 * Author: Sir Gee of Five
 *
 * Created on September 21, 2010, 10:16 AM
 */

#ifndef _HEXEDITOR_H
#define	_HEXEDITOR_H

#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "dwordeditor.h"
#include "addresscolumn.h"
#include "bytecolumn.h"
#include "asciicolumn.h"
#include "appletconfig.h"

#define HEXEDITOR_SUCCESS   (0)
#define HEXEDITOR_FAILED    (-1)
#define HEXEDITOR_MEMORY    (-2)
#define HEXEDITOR_BADADDR   (-3)

#define HEXEDITOR_COLORBG   ((u32)0xFF000000)
#define HEXEDITOR_COLORTXT  ((u32)0xFFE0E0E0)
#define HEXEDITOR_DEFMINOFF 0x48800000
#define HEXEDITOR_DEFMAXOFF 0x49FFFFFF
#define HEXEDITOR_DEFBASE   0x08800000
#define HEXEDITOR_DEFBPL    16

typedef struct _HexEditorRow {
    AddressColumn rAddress;
    ByteColumn rByte;
    AsciiColumn rAscii;
}HexEditorRow;

typedef struct _HexEditorConfig {
    ColorConfig color;
    ColorConfig cursor;
    ColorConfig edit;
    ColorConfig address;
    ColorConfig ascii;
    ColorConfig datacolumns;
    Dimension table_size;
    CursorPos table_pos;
    CursorPos position;
    SceUInt32 min_offset;
    SceUInt32 max_offset;
    SceUInt32 base_address;
    int bytes_per_line;
}HexEditorConfig;

typedef struct _HexEditor {
    HexEditorConfig config;
    AppletConfig* prApCfg;
    CursorPos cursor;
    SceUInt32 offset;
    DwordEditor address_editor;
    int dirty;
    int editing;
    HexEditorRow rEditRow;
}HexEditor;

int hexeditorCrossButton(HexEditor* prHex);
int hexeditorCursorDown(HexEditor* prHex);
int hexeditorCursorLeft(HexEditor* prHex);
int hexeditorCursorRight(HexEditor* prHex);
int hexeditorCursorUp(HexEditor* prHex);
int hexeditorDpadDown(HexEditor* prHex);
int hexeditorDpadLeft(HexEditor* prHex);
int hexeditorDpadRight(HexEditor* prHex);
int hexeditorDpadUp(HexEditor* prHex);
int hexeditorDraw(HexEditor* prHex);
int hexeditorInit(HexEditor* prHex);
int hexeditorPageDown(HexEditor* prHex);
int hexeditorPageUp(HexEditor* prHex);
int hexeditorScrollDown(HexEditor* prHex);
int hexeditorScrollUp(HexEditor* prHex);
int hexeditorRedraw(HexEditor* prHex);
int hexeditorSeek(HexEditor* prHex, SceUInt32 offset);
SceUInt32 hexEditorTell(HexEditor* prHex);

#endif	/* _HEXEDITOR_H */

