/* 
 * File:   bytecolumn.h
 * Author: Sir Gee of Five
 *
 * Created on September 25, 2010, 11:25 AM
 */

#ifndef _BYTECOLUMN_H
#define	_BYTECOLUMN_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "colorconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _ByteColumn {
        ColorConfig     color;          // Display Color
        ColorConfig     edit;           // Editing Color
        ColorConfig     editdigit;      // Editing Digit Color
        unsigned int    value;          // Value
        int             digit;          // Currently Selected Digit
        unsigned int    increments[2];  // Digit Increment Amounts
        unsigned int    min;            // Minimum Value
        unsigned int    max;            // Maximum Value
        int             prefix;         // Whether to display 0x prefix
        int             inedit;         // Whether the column is being edited.
    }ByteColumn;

    unsigned int bytecolumn_commit(ByteColumn *);
    void bytecolumn_decrement(ByteColumn *);
    void bytecolumn_edit(ByteColumn *, unsigned int value);
    void bytecolumn_increment(ByteColumn *);
    void bytecolumn_init(ByteColumn *);

    /* bytecolumn_nextdigit
     *  Activate the next digit of an ByteColumn.  If the next digit cannot
     *  be activated, do nothing.
     * Parameters:
     *  prCol - Pointer to the ByteColumn struct.
     */
    void bytecolumn_nextdigit(ByteColumn *);

    /* bytecolumn_prevdigit
     *  Activate the previous digit of an ByteColumn.  If the previous
     *  digit cannot be activated, do nothing.
     * Parameters:
     *  prCol - Pointer to the ByteColumn struct.
     */
    void bytecolumn_prevdigit(ByteColumn *);

    /* bytecolumn_redraw
     *  Draw an address column at the current screen position on the current
     *  debug screen.
     * Parameters:
     *  prCol - Pointer to an ByteColumn struct, the ByteColumn to draw.
     */
    void bytecolumn_redraw(ByteColumn *prCol);

#ifdef	__cplusplus
}
#endif

#endif	/* _BYTECOLUMN_H */

