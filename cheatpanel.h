/* cheatpanel.h
 *  The Cheat Panel Applet for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef _CHEATPANEL_H
#define _CHEATPANEL_H

#include <pspkerneltypes.h>
#include "cheat.h"
#include "block.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "cheatengine.h"
#include "appletconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CheatPanelConfig {
        /** Normal Color Settings */
        ColorConfig color;
        /** Cursor Column Color Settings */
        ColorConfig cursorcolor;
        /** Cursor Row Color Settings */
        ColorConfig cursorrowcolor;
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
    }
    /** Cheat Panel Applet struct */
    CheatPanel;

    /** Called when the Circle button is released in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat Panel.
     */
    void cheatpanelCircleButton(CheatPanel* prPanel);

    /** Called when the Cross button is released in the Cheat Panel Applet.
     * 
     * @param prPanel Pointer to a CheatPanel struct representing the Cheat Panel.
     */
    void cheatpanelCrossButton(CheatPanel* prPanel);
    void cheatpanelDpadDown(CheatPanel* prPanel);
    void cheatpanelDpadLeft(CheatPanel* prPanel);
    void cheatpanelDpadRight(CheatPanel* prPanel);
    void cheatpanelDpadUp(CheatPanel* prPanel);
    int cheatpanelGetSelectedIndex(CheatPanel* prPanel);
    void cheatpanelInit(CheatPanel* prPanel, CheatEngine* prEngine);
    void cheatpanelRedraw(CheatPanel* prPanel);
    void cheatpanelScrollDown(CheatPanel* prPanel);
    void cheatpanelScrollUp(CheatPanel* prPanel);
    void cheatpanelSquareButton(CheatPanel* prPanel);
    void cheatpanelTriangleButton(CheatPanel* prPanel);

#ifdef	__cplusplus
}
#endif

#endif /* _CHEATPANEL_H */
