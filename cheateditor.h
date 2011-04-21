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
    }
    /* Cheat Editor Block View Mode Enumeration.
     * The Enumeration is used to specify how values are shown during
     * display of Cheats in the Cheat Editor.
     */
    ECEViewMode;


    typedef struct _CheatEditorConfig {
        ColorConfig color;
        ColorConfig cursor;
        ColorConfig edit;
        CursorPos top;
        Dimension size;
    }
    /* Cheat Editor Configuration Structure.
     * 
     * Members:
     *   color  ColorConfig     The default color configuration.
     *   cursor ColorConfig     The cursor color configuration.
     *   edit   ColorConfig     The editor color configuration.
     *   top    CursorPos       The top left corner position on the screen.
     *   size   Dimension       The size of the panel expressed in rows and
     *                          columns.
     */    
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
    /* Cheat Editor Structure.
     * 
     * Members:
     *   config         CheatEditorConfig struct with configuration settings.
     *   prCeConfig     Pointer to CEFiveConfig struct with Cheat Engine
     *                  configuration settings.
     *   prEngine       Pointer to CheatEngine struct representing the Cheat
     *                  Engine.
     *   cheat          Pointer to the first element of an array of Cheat 
     *                  structures that represents the currently loaded Cheats.
     *   block          Pointer to the first element of an array of Block
     *                  structures that represents the currently loaded Blocks.
     *   cheat_count    The number of active elements in the Cheat array.
     *   selected_cheat The index of the currently selected Cheat in the Cheat
     *                  Editor.
     *   top_row        The screen row to begin drawing the Cheat Editor at.
     *   view_mode      ECEViewMode value indicating the current value viewing
     *                  mode in the Cheat Editor.
     *   table_height   The number of table rows visible in the Cheat Editor.
     *   table_top      The screen row of the top of the table on the screen.
     *   table_y        The current first visible row of the table in the Cheat
     *                  Editor.
     *   editing        An integer indicating whether the Cheat Editor is
     *                  currently editing.  1 = True, 0 = False.
     *   rAddrEdit      A DwordEditor struct containing the Address Editor for
     *                  the Cheat Editor.
     *   rValEdit       A DwordEditor struct containing the Value Editor for the
     *                  Cheat Editor.
     *   cursor         A CursorPos struct containing the current panel cursor
     *                  position.
     *   dirty          An integer indicating whether the Cheat Editor needs
     *                  to be redrawn.
     */
    CheatEditor;

    /* Called when the Circle Button is pressed within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorCircleButton(CheatEditor *);

    /* Called when the Cross Button is pressed within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorCrossButton(CheatEditor *);

    /* Move the cursor down one row if possible within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorCursorDown(CheatEditor *);

    /* Move the cursor left one column if possible within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorCursorLeft(CheatEditor *);

    /* Move the cursor right one column if possible within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorCursorRight(CheatEditor *);

    /* Move the cursor up one row if possible within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorCursorUp(CheatEditor *);

    /* Called when the D-Pad Down is pressed.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorDpadDown(CheatEditor *);

    /* Called when the D-Pad Left is pressed.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorDpadLeft(CheatEditor *);

    /* Called when the D-Pad Right is pressed.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorDpadRight(CheatEditor *);

    /* Called when the D-Pad Up is pressed.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorDpadUp(CheatEditor *);

    /* Initialize a Cheat Editor, assigning the specified Cheat array, and the
     * specified Block array.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct to initialize.
     *   parCheat   Pointer to the first element of a Cheat array.
     *   parBlock   Pointer to the first element of a Block array.
     */
    void cheateditorInit(CheatEditor *, Cheat *, Block *);

    /* Redraw a Cheat Editor Panel.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the Cheat Editor
     *              Panel to redraw.
     */
    void cheateditorRedraw(CheatEditor *);

    /* Scroll the specified Cheat Editor Panel down by one row if possible.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the Cheat Editor
     *              Panel to scroll.
     */
    void cheateditorScrollDown(CheatEditor *);

    /* Scroll the specified Cheat Editor Panel up by one row if possible.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the Cheat Editor
     *              Panel to scroll.
     */
    void cheateditorScrollUp(CheatEditor *);

    /* Select the specified Cheat within a CheatEditor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the Cheat Editor
     *              Panel to select from.
     *   index      Integer index specifying which cheat (0-based) to select.
     */
    void cheateditorSelectCheat(CheatEditor *prPanel, int index);

    /* Called when the Square Button is pressed within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorSquareButton(CheatEditor *);

    /* Called when the Triangle Button is pressed within the Cheat Editor.
     * 
     * Parameters:
     *   prPanel    Pointer to a CheatEditor struct containing the CheatEditor
     *              panel.
     */
    void cheateditorTriangleButton(CheatEditor *);

#ifdef	__cplusplus
}
#endif

#endif /* _CHEATEDITOR_H */
