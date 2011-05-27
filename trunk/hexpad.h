/* 
 * File:   hexpad.h
 * Author: Sir Gee of Five
 *
 * Created on May 26, 2011, 1:25 PM
 */

#ifndef HEXPAD_H
#define	HEXPAD_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "panelconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "colorconfig.h"

/** Indicates success. */
#define HEXPAD_SUCCESS (0)

/** Indicates failure. */
#define HEXPAD_FAILURE (-1)

/** Indicates a NULL pointer. */
#define HEXPAD_NULLPTR (-2)

/** Indicates a memory error. */
#define HEXPAD_MEMORY (-3)

/** Indicates an invalid index. */
#define HEXPAD_INVIDX (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _HexPad {
        /** Panel Configuration struct */
        PanelConfig panelConfig;
        /** Cursor Position struct */
        CursorPos cursorPos;
        /** Individual digit values */
        u8 byteval[8];
        /** Currently selected digit */
        int digit;
        /** Dialog Visible indicator */
        int visible;
    }
    /** The HexPad struct represents a Hex Pad. */
    HexPad;

    /** Respond to the circle button being pressed.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_button_circle(HexPad* prPad);
    
    /** Respond to the cross (X) button being pressed.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_button_cross(HexPad* prPad);
    
    /** Respond to the square button being pressed.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_button_square(HexPad* prPad);
    
    /** Respond to the triangle button being pressed.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_button_triangle(HexPad* prPad);
    
    /** Move the Hex Pad cursor down by 1 row.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_cursor_down(HexPad* prPad);
    
    /** Move the Hex Pad cursor left by 1 column.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_cursor_left(HexPad* prPad);
    
    /** Move the Hex Pad cursor right by 1 column.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_cursor_right(HexPad* prPad);
    
    /** Move the Hex Pad cursor up by 1 column.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_cursor_up(HexPad* prPad);
    
    /** Return a pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return A pointer to a PanelConfig struct or NULL is returned.
     */
    PanelConfig* hexpad_get_config(HexPad* prPad);
    
    /** Return a pointer to a ColorConfig struct representing the Cursor Color
     * Configuration.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* hexpad_get_cursorcolor(HexPad* prPad);
    
    /** Return a pointer to a CursorPos struct representing the Cursor Position.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* hexpad_get_cursorpos(HexPad* prPad);
    
    /** Return a pointer to a ColorConfig struct representing the Panel Color
     * Configuration.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* hexpad_get_panelcolor(HexPad* prPad);
    
    /** Return a pointer to a CursorPos struct representing the position of
     * the Hex Pad on the Debug Screen.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* hexpad_get_position(HexPad* prPad);
    
    /** Return a pointer to a Dimension struct representing the size of the
     * Hex Pad on the Debug Screen.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* hexpad_get_size(HexPad* prPad);
    
    SceUInt32 hexpad_get_value(HexPad* prPad);
    
    int hexpad_init(HexPad* prPad);
    
    /** Select the next digit as active.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_next_digit(HexPad* prPad);
    
    /** Select the previous digit as active.
     * 
     * @param prPad Pointer to a HexPad struct representing the Hex Pad.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int hexpad_prev_digit(HexPad* prPad);
    
    int hexpad_redraw(HexPad* prPad);
    
    int hexpad_set_cursorcolor(HexPad* prPad, 
            const u32 background, const u32 text);
    
    int hexpad_set_cursordigit(HexPad* prPad, const int digit);
    
    int hexpad_set_cursorpos(HexPad* prPad, const int x, const int y);
    
    int hexpad_set_panelcolor(HexPad* prPad, 
            const u32 background, const u32 text);
    
    int hexpad_set_position(HexPad* prPad, const int x, const int y);
    
    int hexpad_set_value(HexPad* prPad, const SceUInt32 value);

#ifdef	__cplusplus
}
#endif

#endif	/* HEXPAD_H */

