/* 
 * File:   addresscolumn.h
 *  The Address Column for CEFive.
 *  The Address Column is used to display an editable address on the debug
 *  screen.  The Address Column takes care of updating and redrawing it's
 *  value and is driven through the methods in the module.
 * 
 * Author: Sir Gee of Five
 *
 * Created on September 24, 2010, 1:13 PM
 */

#ifndef _ADDRESSCOLUMN_H
#define	_ADDRESSCOLUMN_H

#include <psptypes.h>
#include "colorconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
/** Indicates success. */
#define ADDRESSCOLUMN_SUCCESS 0

/** Indicates failure. */
#define ADDRESSCOLUMN_FAILURE (-1)
    
/** Indicates a NULL pointer. */
#define ADDRESSCOLUMN_NULLPTR (-2)

    typedef struct _AddressColumn {
        /** Display Color Configuration */
        ColorConfig     color;
        /** Editing Color Configuration */
        ColorConfig     edit;
        /** Editing Digit Color Configuration */
        ColorConfig     editdigit;
        /** Currently Displayed Value */
        unsigned int    value;
        /** Index of the currently selected digit. */
        int             digit;
        /** Array of individual digit increment amounts. */
        unsigned int    increments[8];
        /** Minimum value */
        unsigned int    min;
        /** Maximum value */
        unsigned int    max;
        /** Whether to display a '0x' prefix. */
        int             prefix;
        /** Whether the value is currently being edited. */
        int             inedit;
        /** Indicator to denote that the control needs redrawing. */
        int             dirty;
        /** 32-bit unsigned integer bitmask to use when displaying addresses. */
        SceUInt32 uiDispMask;
    }
    /** The AddressColumn is a User Interface element used to display and edit
     * 32-bit unsigned integer addresses.
     */
    AddressColumn;

    /** Return the current value of an address column and set the AddressColumn
     * to displaying.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the 
     * Address Column.
     * @return unsigned int containing the current value of the Address Column.
     */
    unsigned int addresscolumn_commit(AddressColumn* prCol);

    /** Decrement the currently selected digit in an Address Column by the
     * configured amount.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column to decrement.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_decrement(AddressColumn* prCol);
    
    /** Assign the specified value to an AddressColumn and begin editing.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column to edit.
     * @param value a 32-bit unsigned integer value to assign as the
     * Address.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_edit(AddressColumn* prCol, unsigned int value);
    
    /** Return the increment amount for the currently selected digit.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return an int containing the amount that the value will be incremented
     * or decremented when incremented or decremented.
     */
    int addresscolumn_getincrement(AddressColumn* prCol);
    
    /** Return a pointer to a ColorConfig struct representing the Color 
     * Configuration for the digit that is currently being edited.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* addresscolumn_get_digitcolor(AddressColumn* prCol);
    
    /** Return a pointer to a ColorConfig struct representing the Color 
     * Configuration for the Address Column when displayed.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* addresscolumn_get_displaycolor(AddressColumn* prCol);
    
    /** Return a 32-bit unsigned integer bitmask to use when displaying
     * addresses in an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return SceUInt32 containing the display bitmask.
     */
    SceUInt32 addresscolumn_get_displaymask(AddressColumn* prCol);
    
    /** Return a pointer to a ColorConfig struct representing the Color 
     * Configuration for the Address Column when editing the address.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* addresscolumn_get_editcolor(AddressColumn* prCol);
    
    /** Increment the currently selected digit in an Address Column by the
     * configured amount.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column to increment.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_increment(AddressColumn* prCol);
    
    /** Initialize an AddressColumn struct.
     * 
     * @param prCol Pointer to the AddressColumn struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_init(AddressColumn* prCol);

    /** Indicate that an Address Column needs to be redrawn.
     * 
     * @param prCol Pointer to the AddressColumn struct to invalidate.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_invalidate(AddressColumn* prCol);
    
    /** Indicate whether an Address Column is currently in edit mode.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return 0 indicates that the AddressColumn is not in edit mode,
     * 1 indicates that the AddressColumn is in edit mode and less than 0
     * indicates a failure.
     */
    int addresscolumn_is_editing(AddressColumn* prCol);
    
    /** Indicate whether an Address Column displays an address with a '0x'
     * prefix.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return 0 indicates that addresses are not prefixed, 1 indicates that
     * addresses are prefixed, and less than 0 indicates a failure.
     */
    int addresscolumn_is_prefixed(AddressColumn* prCol);
    
    /** Activate the next digit of an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_nextdigit(AddressColumn *prCol);

    /** Activate the previous digit of an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_prevdigit(AddressColumn *prCol);

    /** Draw an Address Column at the current screen position on the current
     * Debug screen.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column to draw.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_redraw(AddressColumn *prCol);

    /** Assign the currently selected digit of an Address Column.
     * 
     * @param prCol Pointer to the AddressColumn struct representing the
     * Address Column.
     * @param digit int column of the digit to select.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setdigit(AddressColumn* prCol, int digit);
    
    /** Assign a 32-bit unsigned integer value to use as a bitmask when
     * displaying addresses in an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @param mask SceUInt32 containing the bitmask to use.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_set_displaymask(AddressColumn* prCol, SceUInt32 mask);
    
    /** Indicate whether an Address Column is currently editing.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @param editing int containing 0 to indicate not editing, or 1 to indicate
     * editing.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setediting(AddressColumn* prCol, int editing);
    
    /** Assign the increment amount of the specified digit of an Address
     * Column.  The increment amount is the amount that the value is incremented
     * or decremented each time the selected digit is incremented or
     * decremented.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @param digit int column of the digit to assign the amount for.
     * @param amt unsigned int amount to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setincrement(AddressColumn* prCol, int digit, 
            unsigned int amt);
    
    /** Assign the maximum value of an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @param max unsigned int containing the maximum value.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setmax(AddressColumn* prCol, unsigned int max);
    
    /** Assign the minimum value of an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @param min unsigned int containing the minimum value.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setmin(AddressColumn* prCol, unsigned int min);
    
    /** Indicate whether to display a '0x' prefix on an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column.
     * @param prefixed int containing 0 to indicate no prefix or 1 to indicate
     * a '0x' prefix.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setprefixed(AddressColumn* prCol, int prefixed);
    
    /** Assign a 32-bit unsigned integer address to an Address Column.
     * 
     * @param prCol Pointer to an AddressColumn struct representing the
     * Address Column to assign.
     * @param value unsigned int containing the 32-bit address to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int addresscolumn_setvalue(AddressColumn* prCol, unsigned int value);

#ifdef	__cplusplus
}
#endif

#endif	/* _ADDRESSCOLUMN_H */

