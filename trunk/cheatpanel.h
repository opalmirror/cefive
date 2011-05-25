/* cheatpanel.h
 *  The Cheat Panel Applet for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef _CHEATPANEL_H
#define _CHEATPANEL_H

#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "cheat.h"
#include "block.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "cheatengine.h"
#include "appletconfig.h"

/** Indicates success. */
#define CHEATPANEL_SUCCESS (0)

/** Indicates failure. */
#define CHEATPANEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define CHEATPANEL_NULLPTR (-2)

/** Indicates a memory error. */
#define CHEATPANEL_MEMORY (-3)

/** Indicates an invalid Cheat Engine. */
#define CHEATPANEL_INVENGINE (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CheatPanelConfig {
        /** Selected Cheat Color Settings */
        ColorConfig selectedcolor;
        /** Constant Cheat Color Settings */
        ColorConfig constantcolor;
        /** The number of rows in the Cheat Table. */
        int table_height;
        /** The starting row of the Cheat Table. */
        int top_row;
    }
    /** Cheat Panel Configuration Settings */
    CheatPanelConfig;

    typedef struct _CheatPanel {
        /** Cheat Panel Configuration settings. */
        CheatPanelConfig config;
        /** Pointer to the Cheat Engine. */
        CheatEngine* prEngine;
        /** Cheat Panel Applet Configuration settings. */
        AppletConfig* prApCfg;
        /** Cheat Panel Cursor Position. */
        CursorPos cursor;
        /** Starting row of the current Page. */
        int page_position;
        /** Indicates that the panel needs to be redrawn. */
        int dirty;
    }
    /** Cheat Panel Applet struct */
    CheatPanel;

    /** Called when the Circle button is released in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat 
     * Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_circle_button(CheatPanel* prPanel);

    /** Called when the Cross button is released in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat 
     * Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_cross_button(CheatPanel* prPanel);

    /** Called when the D-Pad is pressed Down in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_dpad_down(CheatPanel* prPanel);
    
    /** Called when the D-Pad is pressed Left in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_dpad_left(CheatPanel* prPanel);
    
    /** Called when the D-Pad is pressed Right in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_dpad_right(CheatPanel* prPanel);
    
    /** Called when the D-Pad is pressed Up in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_dpad_up(CheatPanel* prPanel);

    /** Return a pointer to an AppletConfig struct representing the Applet
     * Configuration.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* cheatpanel_get_appletconfig(CheatPanel* prPanel);
    
    /** Return a pointer to a CheatEngine struct representing the Cheat Engine.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a CheatEngine struct or NULL is returned.
     */
    CheatEngine* cheatpanel_get_cheatengine(CheatPanel* prPanel);
    
    /** Return a pointer to a CheatPanelConfig struct representing the Cheat
     * Panel Configuration.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a CheatPanelConfig struct or NULL is returned.
     */
    CheatPanelConfig* cheatpanel_get_config(CheatPanel* prPanel);
    
    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration for a Cheat that is constant.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* cheatpanel_get_constantcolor(CheatPanel* prPanel);

    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration for the Cursor.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* cheatpanel_get_cursorcolor(CheatPanel* prPanel);

    /** Return a pointer to a CursorPos struct representing the current Cursor
     * Position.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* cheatpanel_get_cursorpos(CheatPanel* prPanel);
    
    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration for the Panel.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* cheatpanel_get_panelcolor(CheatPanel* prPanel);

    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration for a Cheat that is selected.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* cheatpanel_get_selectedcolor(CheatPanel* prPanel);
    
    /** Return the index of the currently selected Cheat within the Cheat
     * Engine.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return An int containing the index of the currently selected Cheat is
     * returned.
     */
    int cheatpanel_get_selectedindex(CheatPanel* prPanel);

    /** Initialize a Cheat Panel Applet.
     * 
     * @param prPanel Pointer to the CheatPanel struct to initialize.
     * @param prEngine Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @return 0 indicates success, &lt;0 indicates failure. 
     */
    int cheatpanel_init(CheatPanel* prPanel, CheatEngine* prEngine);

    /** Indicate that a Cheat Panel needs to be redrawn.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_invalidate(CheatPanel* prPanel);
    
    /** Redraw a Cheat Panel on the Debug Screen.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_redraw(CheatPanel* prPanel);

    /** Scroll the Cheat list down by a single line.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_scroll_down(CheatPanel* prPanel);
    
    /** Scroll the Cheat list up by a single line.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_scroll_up(CheatPanel* prPanel);

    /** Assign the Color Configuration values for Cheats that are constant.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @param bg u32 indicating the background color.
     * @param fg u32 indicating the text color.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_set_constantcolor(CheatPanel* prPanel, u32 bg, u32 fg);
    
    /** Assign the Color Configuration values for the Cursor.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @param bg u32 indicating the background color.
     * @param fg u32 indicating the text color.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_set_cursorcolor(CheatPanel* prPanel, u32 bg, u32 fg);
    
    /** Assign the current Cursor Position.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @param x int indicating the horizontal cursor position.
     * @param y int indicating the vertical cursor position.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_set_cursorpos(CheatPanel* prPanel, int x, int y);
    
    /** Assign the Color Configuration values for Cheats that are selected.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @param bg u32 indicating the background color.
     * @param fg u32 indicating the text color.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_set_selectedcolor(CheatPanel* prPanel, u32 bg, u32 fg);
    
    /** Assign the table height.  The Table height controls the number of
     * Cheats visible on the page.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @param height int indicating the table height in rows.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_set_tableheight(CheatPanel* prPanel, int height);

    /** Called when the user presses and releases the Square button.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_square_button(CheatPanel* prPanel);
    
    /** Called when the user presses and releases the Triangle button.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cheatpanel_triangle_button(CheatPanel* prPanel);

#ifdef	__cplusplus
}
#endif

#endif /* _CHEATPANEL_H */
