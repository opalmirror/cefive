/* 
 * File:   dasmrow.h
 * Author: george
 *
 * Created on May 1, 2011, 2:35 PM
 */

#ifndef DASMROW_H
#define	DASMROW_H

#include <psptypes.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "addresscolumn.h"
#include "dwordcolumn.h"
#include "textcolumn.h"
#include "colorconfig.h"

/** Indicates success. */
#define DASMROW_SUCCESS         (0)
    
/** Indicates failure. */
#define DASMROW_FAILURE         (-1)
    
/** Indicates a NULL pointer. */
#define DASMROW_NULLPTR         (-2)
    
/** Indicates a memory error. */
#define DASMROW_MEMORY          (-3)
    
    typedef struct _DasmRow {
        /** The Address Column. */
        AddressColumn rAddress;
        /** The Value Column. */
        DwordColumn rValue;
        /** The Assembly Column.*/
        TextColumn rAssembly;
    } 
    /** The DasmRow struct represents a single row of the Disassembler.
     * The row is comprised of an Address Column, a Value Column and an Assembly
     * Column.
     */
    DasmRow;

    /** Return the 32-bit unsigned integer address of a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @return SceUInt32 containing the address or 0.
     */
    SceUInt32 dasmrow_get_address(DasmRow* prRow);
    
    /** Return a pointer to an AddressColumn struct representing the Address
     * Column of a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Row.
     * @return A pointer to an AddressColumn struct or NULL is returned.
     */
    AddressColumn* dasmrow_get_addresscolumn(DasmRow* prRow);
    
    /** Return a pointer to a TextColumn struct representing the Assembly
     * Column of a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Row.
     * @return A pointer to a TextColumn struct or NULL is returned.
     */
    TextColumn* dasmrow_get_assemblycolumn(DasmRow* prRow);

    /** Return a 32-bit unsigned integer bitmask to use when displaying
     * addresses in an Address Column.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @return SceUInt32 containing the bitmask or 0 is returned.
     */
    SceUInt32 dasmrow_get_displaymask(DasmRow* prRow);
    
    /** Return a 32-bit unsigned integer containing the current Value of a
     * Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @return SceUInt32 containing the current value or 0.
     */
    SceUInt32 dasmrow_get_value(DasmRow* prRow);
    
    /** Return a pointer to a DwordColumn struct representing the Value Column
     * of a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Row.
     * @return A pointer to a DwordColumn struct or NULL is returned.
     */
    DwordColumn* dasmrow_get_valuecolumn(DasmRow* prRow);
    
    /** Initialize a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Row.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmrow_init(DasmRow* prRow);
    
    /** Redraw a Dissasembler Row on the screen.
     * Begins drawing at the current screen position.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Row.
     * @return 0 indicates success, less that 0 indicates failure.
     */
    int dasmrow_redraw(DasmRow* prRow);
    
    /** Assign a 32-bit unsigned integer address to a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @param address SceUInt32 containing the address to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmrow_set_address(DasmRow* prRow, SceUInt32 address);
    
    /** Assign the Assembly Language to display in a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @param sAssm String containing the Assembly Language to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmrow_set_assembly(DasmRow* prRow, const char* sAssm);
    
    /** Assign a Color Configuration to a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @param prColor Pointer to a ColorConfig struct representing the 
     * Color Configuration to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmrow_set_color(DasmRow* prRow, ColorConfig* prColor);
    
    /** Assign a 32-bit unsigned integer value to use as a bitmask when
     * displaying addresses in an Address Column.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @param mask SceUInt32 containing the display bitmask to use.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmrow_set_displaymask(DasmRow* prRow, SceUInt32 mask);

    /** Assign a 32-bit unsigned integer value to a Disassembler Row.
     * 
     * @param prRow Pointer to a DasmRow struct representing the Disassembler
     * Row.
     * @param value SceUInt32 containing the value to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmrow_set_value(DasmRow* prRow, SceUInt32 value);
    
#ifdef	__cplusplus
}
#endif

#endif	/* DASMROW_H */

