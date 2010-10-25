/* 
 * File:   dwordcolumn.h
 * Author: Sir Gee of Five
 *
 * Created on September 25, 2010, 11:20 AM
 */

#ifndef _DWORDCOLUMN_H
#define	_DWORDCOLUMN_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "colorconfig.h"

    #define DWORDCOLUMN_SUCCESS     (0)
    #define DWORDCOLUMN_FAILURE     (-1)
    #define DWORDCOLUMN_MEMORY      (-2)
    #define DWORDCOLUMN_NULLPTR     (-3)

    typedef struct _DwordColumn {
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
    }DwordColumn;

    unsigned int dwordcolumn_commit(DwordColumn *);
    void dwordcolumn_decrement(DwordColumn *);
    void dwordcolumn_edit(DwordColumn *, unsigned int value);
    void dwordcolumn_increment(DwordColumn *);
    void dwordcolumn_init(DwordColumn *);

    /* dwordcolumn_nextdigit
     *  Activate the next digit of an DwordColumn.  If the next digit cannot
     *  be activated, do nothing.
     * Parameters:
     *  prCol - Pointer to the DwordColumn struct.
     */
    void dwordcolumn_nextdigit(DwordColumn *);

    /* dwordcolumn_prevdigit
     *  Activate the previous digit of an DwordColumn.  If the previous
     *  digit cannot be activated, do nothing.
     * Parameters:
     *  prCol - Pointer to the DwordColumn struct.
     */
    void dwordcolumn_prevdigit(DwordColumn *);

    /* dwordcolumn_redraw
     *  Draw an address column at the current screen position on the current
     *  debug screen.
     * Parameters:
     *  prCol - Pointer to an DwordColumn struct, the DwordColumn to draw.
     */
    void dwordcolumn_redraw(DwordColumn *prCol);

    int dwordcolumn_setvalue(DwordColumn* prCol, unsigned int value);

#ifdef	__cplusplus
}
#endif

#endif	/* _DWORDCOLUMN_H */

