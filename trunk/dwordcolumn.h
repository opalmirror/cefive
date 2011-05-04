/* 
 * File:   dwordcolumn.h
 * Author: Sir Gee of Five
 *
 * Created on September 25, 2010, 11:20 AM
 */

#ifndef _DWORDCOLUMN_H
#define	_DWORDCOLUMN_H

#include "colorconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

/** Indicates success. */
#define DWORDCOLUMN_SUCCESS     (0)

/** Indicates failure. */
#define DWORDCOLUMN_FAILURE     (-1)

/** Indicates a memory error. */
#define DWORDCOLUMN_MEMORY      (-2)

/** Indicates a NULL pointer. */
#define DWORDCOLUMN_NULLPTR     (-3)

    typedef struct _DwordColumn {
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
    }
    /** The Dword Column is used to display and edit 32-bit integer values.
     * The DwordColumn struct is used to represent a Dword Column.
     */
    DwordColumn;

    /** Return the current editing value of a Dword Column and set the 
     * Column to displaying.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return an unsigned int containing the edited value.
     */
    unsigned int dwordcolumn_commit(DwordColumn* prCol);
    
    /** Decrement a Dword Column value by the configured increment amount
     * for the currently selected digit.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_decrement(DwordColumn* prCol);
    
    /** Assign the specified value as the editing value of a Dword Column and
     * set the Column to editing.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param value unsigned int containing the value to edit.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_edit(DwordColumn* prCol, unsigned int value);

    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration of a Dword Column while a digit is being edited.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return A pointer to a ColorConfig struct or NULL is returned. 
     */
    ColorConfig* dwordcolumn_get_digitcolor(DwordColumn* prCol);
    
    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration of a Dword Column while the value is being displayed.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return A pointer to a ColorConfig struct or NULL is returned. 
     */
    ColorConfig* dwordcolumn_get_displaycolor(DwordColumn *prCol);
    
    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration of a Dword Column while a value is being edited.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return A pointer to a ColorConfig struct or NULL is returned. 
     */
    ColorConfig* dwordcolumn_get_editcolor(DwordColumn *prCol);
    
    /** Return the current increment amount of a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return unsigned int containing the current increment amount. 
     */
    unsigned int dwordcolumn_get_increment(DwordColumn* prCol);
    
    /** Increment a Dword Column value by the configured increment amount
     * for the currently selected digit.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_increment(DwordColumn* prCol);
    
    /** Initialize a Dword Column.
     * 
     * @param prCol Pointer to the DwordColumn struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_init(DwordColumn* prCol);

    /** Indicate that a Dword Column needs redrawing.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_invalidate(DwordColumn* prCol);
    
    /** Return an indication of whether a Dword Column is editing.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 1 indicates that the column is editing, 0 indicates that the
     * column is not editing. 
     */
    int dwordcolumn_is_editing(DwordColumn* prCol);
    
    /** Return an indication of whether a Dword Column is displayed with a '0x'
     * prefix.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 1 indicates that the column is prefixed, 0 indicates that the
     * column is not prefixed. 
     */
    int dwordcolumn_is_prefixed(DwordColumn* prCol);
    
    /** Select the next digit to be active in a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_nextdigit(DwordColumn* prCol);

    /** Select the previous digit to be active in a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_prevdigit(DwordColumn* prCol);

    /** Redraw a Dword Column on the screen.  Will only redraw if invalidated.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_redraw(DwordColumn* prCol);

    /** Assign the selected editing digit in a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param digit int containing the digit (0-7) to select.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_set_digit(DwordColumn* prCol, int digit);
    
    /** Indicate whether a Dword Column is editing.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param editing int containing a 0 to indicate that the column is not 
     * editing or a 1 to indicate that the column is editing.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_set_editing(DwordColumn* prCol, int editing);
    
    /** Assign the increment amount for the specified digit in a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param digit int containing the digit (0-7) to set the increment for.
     * @param amount unsigned int containing the increment amount.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_set_increment(DwordColumn* prCol, int digit,
            unsigned int amount);

    /** Assign the maximum value of a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param max unsigned int containing the value to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_set_max(DwordColumn* prCol, unsigned int max);
    
    /** Assign the minimum value of a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param min unsigned int containing the value to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_set_min(DwordColumn* prCol, unsigned int min);
    
    /** Indicate whether a Dword Column value is displayed using a '0x' prefix.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param prefixed int containing a 0 to indicate that column is not
     * prefixed or a 1 to indicate that the column is prefixed.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_set_prefixed(DwordColumn* prCol, int prefixed);
    
    /** Assign the value of a Dword Column.
     * 
     * @param prCol Pointer to a DwordColumn struct representing the Dword
     * Column.
     * @param value insigned integer containing the 32-bit value to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int dwordcolumn_setvalue(DwordColumn* prCol, unsigned int value);

#ifdef	__cplusplus
}
#endif

#endif	/* _DWORDCOLUMN_H */

