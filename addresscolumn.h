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

#include "colorconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _AddressColumn {
        ColorConfig     color;          // Display Color
        ColorConfig     edit;           // Editing Color
        ColorConfig     editdigit;      // Editing Digit Color
        unsigned int    value;          // Value
        int             digit;          // Currently Selected Digit
        unsigned int    increments[8];  // Digit Increment Amounts
        unsigned int    min;            // Minimum Value
        unsigned int    max;            // Maximum Value
        int             prefix;         // Whether to display 0x prefix
        int             inedit;         // Whether the column is being edited.
        int             dirty;          // Whether the column needs drawing.
    }AddressColumn;

    unsigned int addresscolumn_commit(AddressColumn *);
    void addresscolumn_decrement(AddressColumn *);
    void addresscolumn_edit(AddressColumn *, unsigned int value);
    int addresscolumn_getincrement(AddressColumn* prCol);
    void addresscolumn_increment(AddressColumn *);
    void addresscolumn_init(AddressColumn *);

    /* addresscolumn_nextdigit
     *  Activate the next digit of an AddressColumn.  If the next digit cannot
     *  be activated, do nothing.
     * Parameters:
     *  prCol - Pointer to the AddressColumn struct.
     */
    void addresscolumn_nextdigit(AddressColumn *);

    /* addresscolumn_prevdigit
     *  Activate the previous digit of an AddressColumn.  If the previous
     *  digit cannot be activated, do nothing.
     * Parameters:
     *  prCol - Pointer to the AddressColumn struct.
     */
    void addresscolumn_prevdigit(AddressColumn *);

    /* addresscolumn_redraw
     *  Draw an address column at the current screen position on the current
     *  debug screen.
     * Parameters:
     *  prCol - Pointer to an AddressColumn struct, the AddressColumn to draw.
     */
    void addresscolumn_redraw(AddressColumn *prCol);

    void addresscolumn_setvalue(AddressColumn* prCol, unsigned int value);

#ifdef	__cplusplus
}
#endif

#endif	/* _ADDRESSCOLUMN_H */

