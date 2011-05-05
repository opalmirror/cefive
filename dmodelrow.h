/* 
 * File:   dmodelrow.h
 * Author: Sir Gee of Five
 *
 * Created on May 5, 2011, 10:38 AM
 */

#ifndef DMODELROW_H
#define	DMODELROW_H

#include <psptypes.h>

/** Indicates success. */
#define DMODELROW_SUCCESS (0)

/** Indicates failure. */
#define DMODELROW_FAILURE (-1)

/** Indicates a NULL pointer. */
#define DMODELROW_NULLPTR (-2)

/** The maximum length of the Assembly column. */
#define DMODELROW_ASSMLEN (80)

/** The default Kernel Address of a Row. */
#define DMODELROW_DEFADDR ((SceUInt32) 0)

/** The default Value of a Row. */
#define DMODELROW_DEFVALUE ((SceUInt32) 0)

/** The default Assembly Language of a Row. */
#define DMODELROW_DEFASSM "nop"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DmodelRow {
        /** The Kernel Address of the Row. */
        SceUInt32 uiAddress;
        /** The value of the Row. */
        SceUInt32 uiValue;
        /** The Assembly Language for the Row. */
        char sAssembly[DMODELROW_ASSMLEN + 1];
    }
    /** The DmodelRow struct represents a single Disassembler Model Row. */
    DmodelRow;

    /** Initialize a Disassembler Model Row.
     * 
     * @param prRow Pointer to the DmodelRow struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dmodelrow_init(DmodelRow* prRow);

    /** Assign the Kernel Address of a Disassembler Model Row.
     * 
     * @param prRow Pointer to a DmodelRow struct representing the Disassembler
     * Model Row.
     * @param address SceUInt32 containing the Kernel Address to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dmodelrow_set_address(DmodelRow* prRow, const SceUInt32 address);
    
    /** Assign the Assembly Language of a Disassembler Model Row.
     * 
     * @param prRow Pointer to a DmodelRow struct representing the Disassembler
     * Model Row.
     * @param sAssm String containing the Assembly language to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dmodelrow_set_assembly(DmodelRow* prRow, const char* sAssm);
    
    /** Assign the Value of a Disassembler Model Row.
     * 
     * @param prRow Pointer to a DmodelRow struct representing the Disassembler
     * Model Row.
     * @param value SceUInt32 containing the Value to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dmodelrow_set_value(DmodelRow* prRow, const SceUInt32 value);

#ifdef	__cplusplus
}
#endif

#endif	/* DMODELROW_H */

