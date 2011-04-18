/* File: cheateditor.h
 *       The Cheat Editor for the Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */

#ifndef _CHEATEDITOR_H
#define _CHEATEDITOR_H

#include <pspkerneltypes.h>
#include "cheat.h"
#include "block.h"
#include "dwordeditor.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "cefiveconfig.h"
#include "cheatengine.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ECEViewMode {
        CEVM_None,
        CEVM_Value,
        CEVM_Assembly
    } ECEViewMode;

    typedef struct _CheatEditorConfig {
        ColorConfig color;
        ColorConfig cursor;
        ColorConfig edit;
        CursorPos top;
        Dimension size;
    } CheatEditorConfig;

    typedef struct _CheatEditor {
        CheatEditorConfig config;
        CEFiveConfig* prCeConfig;
        CheatEngine* prEngine;
        Cheat *cheat;
        Block *block;
        int cheat_count;
        int selected_cheat;
        int top_row;
        u32 background_color;
        u32 text_color;
        u32 cursor_rowcolor;
        u32 cursor_textcolor;
        ECEViewMode view_mode;
        int table_height;
        int table_top;
        int table_y;
        int editing;
        DwordEditor rAddrEdit;
        DwordEditor rValEdit;
        CursorPos cursor;
        int dirty;
    } CheatEditor;

    void cheateditorCircleButton(CheatEditor *);
    void cheateditorCrossButton(CheatEditor *);
    void cheateditorCursorDown(CheatEditor *);
    void cheateditorCursorLeft(CheatEditor *);
    void cheateditorCursorRight(CheatEditor *);
    void cheateditorCursorUp(CheatEditor *);
    void cheateditorDpadDown(CheatEditor *);
    void cheateditorDpadLeft(CheatEditor *);
    void cheateditorDpadRight(CheatEditor *);
    void cheateditorDpadUp(CheatEditor *);
    void cheateditorInit(CheatEditor *, Cheat *, Block *);
    void cheateditorRedraw(CheatEditor *);
    void cheateditorScrollDown(CheatEditor *);
    void cheateditorScrollUp(CheatEditor *);
    void cheateditorSelectCheat(CheatEditor *prPanel, int index);
    void cheateditorSquareButton(CheatEditor *);
    void cheateditorTriangleButton(CheatEditor *);

#ifdef	__cplusplus
}
#endif

#endif /* _CHEATEDITOR_H */
