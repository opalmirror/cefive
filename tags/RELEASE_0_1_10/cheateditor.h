/* File: cheateditor.h
 *       The Cheat Editor for the Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */

#ifndef CHEATEDITOR_H
#define CHEATEDITOR_H

#include <stdio.h>
#include <string.h>
#include <pspdebug.h>
#include <pspkerneltypes.h>
#include "block.h"
#include "cefiveconfig.h"
#include "cheat.h"
#include "cheatengine.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "dwordeditor.h"
#include "mips.h"

#define CHEATEDITOR_SUCCESS (0)

#define CHEATEDITOR_FAILURE (-1)

#define CHEATEDITOR_NULLPTR (-2)

#define CHEATEDITOR_MEMORY (-3)

#define CHEATEDITOR_INVIDX (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ECEViewMode {
        CEVM_None, /** No View Mode */
        CEVM_Value, /** View as a Value */
        CEVM_Assembly /** View as MIPS32 Assembly Language */
    }
    /** The ECEViewMode enumeration is used to indicate a view mode for
     * individual Blocks in a Cheat.
     */
    ECEViewMode;


    typedef struct _CheatEditorConfig {
        /** Panel Color Configuration */
        ColorConfig color;
        /** Cursor Color Configuration */
        ColorConfig cursor;
        /** Editor Color Configuration */
        ColorConfig edit;
        /** Top Left Corner of Panel */
        CursorPos top;
        /** Panel size */
        Dimension size;
    }
    /** The CheatEditorConfig struct represents a Cheat Editor Configuration. */
    CheatEditorConfig;

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
    }
    CheatEditor;

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

#endif /* CHEATEDITOR_H */
