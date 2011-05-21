/* 
 * File:   dasmtable.h
 * Author: Sir Gee of Five
 *
 * Created on May 2, 2011, 6:09 PM
 */

#ifndef DASMTABLE_H
#define	DASMTABLE_H

#include <psptypes.h>
#include "cursorpos.h"
#include "dasmmodel.h"
#include "panelconfig.h"
#include "colorconfig.h"

/** Indicates success. */
#define DASMTABLE_SUCCESS (0)

/** Indicates failure. */
#define DASMTABLE_FAILURE (-1)

/** Indicates a NULL pointer. */
#define DASMTABLE_NULLPTR (-2)

/** Indicates a memory error. */
#define DASMTABLE_MEMORY (-3)

/** Indicates an Index Out of Bounds error. */
#define DASMTABLE_INDEX (-4)

/** Indicate an invalid cursor position. */
#define DASMTABLE_INVPOS (-5)

/** Indicate an invalid Model pointer. */
#define DASMTABLE_INVMODEL (-6)

/** Indicate a Model Failure. */
#define DASMTABLE_MODELFAIL (-7)

/** Indicates an invalid Cursor pointer. */
#define DASMTABLE_INVCURSOR (-8)

/** Indicates a Cursor failure. */
#define DASMTABLE_CURSORFAIL (-9)

/** Indicates an invalid PanelConfig pointer. */
#define DASMTABLE_INVPCFG (-10)

/** Indicates a PanelConfig failure. */
#define DASMTABLE_PCFGFAIL (-11)

/** Indicates a failure to assign Row Format. */
#define DASMTABLE_RFMTFAIL (-12)

/** The maximum number of rows in the Table. */
#define DASMTABLE_MAXROWS (30)

/** The maximum length of the format string. */
#define DASMTABLE_FMTLEN (80)

/** The default Row Format */
#define DASMTABLE_DEFFMT "%10s %8s  %48s"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DasmTable {
        /** The Model that represents the Data in the Table. */
        DasmModel rModel;
        /** CursorPos struct representing the Cursor Position. */
        CursorPos rCursor;
        /** Panel Configuration struct. */
        PanelConfig rPanelCfg;
        /** Minimum Possible Page Address. */
        SceUInt32 uiMinPage;
        /** Maximum Possible Page Address. */
        SceUInt32 uiMaxPage;
        /** String denoting the format of a Row. */
        char sRowFmt[DASMTABLE_FMTLEN + 1];
        /** Indicates whether the entire Table needs to be redrawn. */
        int iPanelDirty;
    }
    /** The DasmTable struct is used to represent a Disassembler table.
     */
    DasmTable;

    /** Move the cursor of a Disassembler Table down by one row.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_cursor_down(DasmTable* prTable);
    
    /** Move the cursor of a Disassembler Table left by one column.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_cursor_left(DasmTable* prTable);
    
    /** Move the cursor of a Disassembler Table right by one column.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_cursor_right(DasmTable* prTable);
    
    /** Move the cursor of a Disassembler Table up by one row.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_cursor_up(DasmTable* prTable);
    
    /** Return a pointer to a ColorConfig struct representing the Cursor
     * Color Configuration for a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmtable_get_cursorcolor(DasmTable* prTable);
    
    /** Return a pointer to a CursorPos struct representing the Cursor Position
     * of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* dasmtable_get_cursorpos(DasmTable* prTable);

    /** Return a pointer to the DasmModel struct that represents the Model
     * for a Disassembler Table.
     * 
     * @param prTable Pointer to the DasmTable struct representing the
     * Disassembler Table.
     * @return A pointer to a DasmModel struct or NULL is returned.
     */
    DasmModel* dasmtable_get_model(DasmTable* prTable);
    
    /** Return a pointer to a ColorConfig struct representing the Panel
     * Color Configuration for a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmtable_get_panelcolor(DasmTable* prTable);
    
    /** Return a pointer to a PanelConfig struct representing the Panel
     * Configuration of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return A pointer to a PanelConfig struct or NULL is returned.
     */
    PanelConfig* dasmtable_get_panelconfig(DasmTable* prTable);
    
    /** Return a pointer to a CursorPos struct representing the Top-Left
     * corner of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* dasmtable_get_position(DasmTable* prTable);
    
    /** Return the index of the currently selected Column of a Disassembler
     * Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return An int containing the currently selected index or a negative
     * value indicating a failure is returned.
     */
    int dasmtable_get_selcol(DasmTable* prTable);
    
    /** Return the index of the currently selected Row of a Disassembler
     * Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return An int containing the currently selected index or a negative
     * value indicating a failure is returned.
     */
    int dasmtable_get_selrow(DasmTable* prTable);
    
    /** Indicate that the entire Disassembler Table needs to be redrawn.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_invalidate(DasmTable* prTable);
    
    /** Return the number of visible Rows in a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return int containing the number of visible rows, or less than 0 if
     * an error occured.
     */
    int dasmtable_get_rows(DasmTable* prTable);
    
    /** Initialize a DasmTable struct to represent a Disassembler Table.
     * 
     * @param prTable Pointer to the DasmTable struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_init(DasmTable* prTable);

    /** Render a Disassembler Table on the debug screen.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table to render.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_redraw(DasmTable* prTable);

    /** Scroll a Disassembler Table forward by the specified number of Rows.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @param rows int containing the number of rows to scroll.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_scroll_down(DasmTable* prTable, const unsigned int rows);
    
    /** Scroll a Disassembler Table backward by the specified number of Rows.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @param rows int containing the number of rows to scroll.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_scroll_up(DasmTable* prTable, const unsigned int rows);
    
    /** Position a Disassembler Table to where the first row of the table
     * is the specified address.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table.
     * @param address SceUInt32 containing the address to seek.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_seek(DasmTable* prTable, SceUInt32 address);
    
    /** Assign a string containing a printf style formatting string to act
     * as the formatting for each Row in the Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @param sFormat String containing the format string to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_set_rowformat(DasmTable* prTable, const char* sFormat);
    
    /** Assign the selected Column index of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @param col const unsigned int containing the index to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_set_selcol(DasmTable* prTable, const unsigned int col);
    
    /** Assign the selected Row index of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @param row const unsigned int containing the index to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_set_selrow(DasmTable* prTable, const unsigned int row);
    
    /** Return a 32-bit unsigned integer containing the address of the first
     * row of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return SceUInt32 containing the page address or 0 is returned.
     */
    SceUInt32 dasmtable_tell(DasmTable* prTable);
    
#ifdef	__cplusplus
}
#endif

#endif	/* DASMTABLE_H */
