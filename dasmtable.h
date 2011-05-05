/* 
 * File:   dasmtable.h
 * Author: Sir Gee of Five
 *
 * Created on May 2, 2011, 6:09 PM
 */

#ifndef DASMTABLE_H
#define	DASMTABLE_H

#include <psptypes.h>
#include "dasmrow.h"
#include "cursorpos.h"

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

/** The maximum number of rows in the Table. */
#define DASMTABLE_MAXROWS (30)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DasmTable {
        /** An array of DasmRow structs representing the rows of the table. */
        DasmRow arRow[DASMTABLE_MAXROWS];
        /** The number of rows visible in the table. */
        int rows;
        /** CursorPos struct representing the Cursor Position. */
        CursorPos rCursor;
        /** Display Address Bitmask. */
        SceUInt32 uiDispmask;
        /** Address of the first row in the Table. */
        SceUInt32 uiPageAddress;
        /** Minimum Possible Page Address. */
        SceUInt32 uiMinPage;
        /** Maximum Possible Page Address. */
        SceUInt32 uiMaxPage;
    }
    /** The DasmTable struct is used to represent a Disassembler table.
     */
    DasmTable;

    /** Return a pointer to a CursorPos struct representing the Cursor Position
     * of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* dasmtable_get_cursorpos(DasmTable* prTable);
    
    /** Return a 32-bit unsigned integer containing a bitmask to apply when
     * displaying Addresses in a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table.
     * @return The bitmask to apply or 0 is returned.
     */
    SceUInt32 dasmtable_get_displaymask(DasmTable* prTable);
    
    /** Assign a pointer to the specified Disassembler Row in a table.
     * 
     * @param prRow Pointer to a DasmRow struct to assign.
     * @param prTable Pointer to a DasmTable struct representing the Table.
     * @param row int containing the index of the Row to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_get_row(DasmRow* prRow, DasmTable* prTable, int row);
    
    /** Return the number of visible rows in a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table.
     * @return an int containing the number of visible rows in the Table or a
     * negative value if an error occured.
     */
    int dasmtable_get_rows(DasmTable* prTable);
    
    /** Initialize a DasmTable struct to represent a Disassembler Table.
     * 
     * @param prTable Pointer to the DasmTable struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_init(DasmTable* prTable);

    /** Position a Disassembler Table to where the first row of the table
     * is the specified address.
     * 
     * @param prTable Pointer to a DasmTable struct representing the 
     * Disassembler Table.
     * @param address SceUInt32 containing the address to seek.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_seek(DasmTable* prTable, SceUInt32 address);
    
    /** Assign the cursor position of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @param col int containing the column position.
     * @param row int containing the row position.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_set_cursor(DasmTable* prTable, int col, int row);
    
    /** Assign the number of visible rows in a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing a Disassembler
     * Table.
     * @param rows int containing the number of visible rows.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_set_rows(DasmTable* prTable, int rows);

    /** Return a 32-bit unsigned integer containing the address of the first
     * row of a Disassembler Table.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return SceUInt32 containing the page address or 0 is returned.
     */
    SceUInt32 dasmtable_tell(DasmTable* prTable);
    
    /** Refresh the individual cells of a Disassembler Table rereading the
     * values from memory.
     * 
     * @param prTable Pointer to a DasmTable struct representing the
     * Disassembler Table.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmtable_update(DasmTable* prTable);
    
#ifdef	__cplusplus
}
#endif

#endif	/* DASMTABLE_H */
