/* 
 * File:   dasmmodel.h
 * Author: Sir Gee of Five
 *
 * Created on May 5, 2011, 12:05 PM
 */

#ifndef DASMMODEL_H
#define	DASMMODEL_H

#include <psptypes.h>
#include "dmodelrow.h"

/** Indicates success. */
#define DASMMODEL_SUCCESS (0)

/** Indicates failure. */
#define DASMMODEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define DASMMODEL_NULLPTR (-2)

/** Indicates an invalid index or array index. */
#define DASMMODEL_INVIDX (-3)

/** Maximum number of Rows in the Model. */
#define DASMMODEL_MAXROWS (30)

/** The default number of rows in the Model. */
#define DASMMODEL_DEFROWS (20)

/** The default virtual address mask. */
#define DASMMODEL_DEFVAMASK ((SceUInt32) 0xBFFFFFFF)

/** The number of columns in the Model. */
#define DASMMODEL_COLS (3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DasmModel {
        /** Array of DmodelRow structs representing the Rows. */
        DmodelRow arRow[DASMMODEL_MAXROWS];
        /** The number of visible Rows. */
        int iRows;
        /** The Virtual Address Mask. */
        SceUInt32 uiAddrMask;
        /** Whether to prefix virtual addresses with '0x'. */
        int addrPrefix;
    }
    /** The DasmModel struct represents a Disassembler Model. */
    DasmModel;

    /** Return the current page address of the Model.
     * 
     * @param prModel Pointer to a DasmModel struct representing the 
     * Disassembler Model.
     * @return The current page address or 0 is returned.
     */
    SceUInt32 dasmmodel_get_address(DasmModel* prModel);

    /** Return a pointer to a DmodelRow struct representing the indicated Row
     * of a Disassember Model.
     * 
     * @param prModel Pointer to a DasmModel struct representing the
     * Disassembler Model.
     * @param index int containing the row to return.
     * @return A pointer to a DmodelRow struct or NULL is returned.
     */
    DmodelRow* dasmmodel_get_row(DasmModel* prModel, int index);
    
    /** Return a string representation of the indicated cell of a Disassembler
     * Model.
     * 
     * @param sDest char pointer representing the destination string.
     * @param prModel Pointer to a DasmModel struct representing the
     * Disassembler Model.
     * @param row int containing the row to return.
     * @param col int containing the column to return.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmmodel_get_value(char* sDest, DasmModel* prModel, int row, int col);
    
    /** Initialize a Disassembler Model.
     * 
     * @param prModel Pointer to the DasmModel struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmmodel_init(DasmModel* prModel);
    
    /** Assign the page address of a Disassembler Table.  As a result the
     * contents of the table will be automatically updated.
     * 
     * @param prModel Pointer to a DasmModel struct representing the
     * Disassembler Model.
     * @param address SceUInt32 containing the address to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dasmmodel_set_address(DasmModel* prModel, SceUInt32 address);


#ifdef	__cplusplus
}
#endif

#endif	/* DASMMODEL_H */

