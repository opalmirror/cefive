/* 
 * File:   optionspanel.h
 * Author: Sir Gee of Five
 *
 * Created on September 26, 2010, 10:49 AM
 */

#ifndef OPTIONSPANEL_H
#define	OPTIONSPANEL_H

#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include <string.h>
#include "appletconfig.h"
#include "cefiveconfig.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "hexpad.h"
#include "panelconfig.h"

/** Indicates success. */
#define OPTIONSPANEL_SUCCESS (0)

/** Indicates failure. */
#define OPTIONSPANEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define OPTIONSPANEL_NULLPTR (-2)

/** Indicates a memory error. */
#define OPTIONSPANEL_MEMORY (-3)

/** Pause Row Label */
#define OPTIONSPANEL_L_PAUSE    "Pause Game during UI: "

/** Panel Color Row Label */
#define OPTIONSPANEL_L_PANELC   "Panel Color       (bg/fg): "

/** Cursor Color Row Label */
#define OPTIONSPANEL_L_CURSORC  "Cursor Color      (bg/fg): "

/** Titlebar Color Row Label */
#define OPTIONSPANEL_L_TITLEC   "Titlebar Color    (bg/fg): "

/** Status / Menu Color Row Label */
#define OPTIONSPANEL_L_STATUSC  "Status/Menu Color (bg/fg): "

/** Edit / Hilite Color Row Label */
#define OPTIONSPANEL_L_EDITC    "Hilite Color      (bg/fg): "

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _OptionsPanel {
        /** Cursor Position struct */
        CursorPos cursor;
        /** cefive Configuration struct pointer */
        CEFiveConfig* prCEConfig;
        /** Hex Pad struct */
        HexPad hexPad;
        /** Indicates that an Options Panel needs to be redrawn. */
        int dirty;
        /** Indicates that an Options Panel is currently editing a value. */
        int editing;
    }
    /** The OptionsPanel struct is used to represent an Options Panel Applet. */
    OptionsPanel;

    /** Respond to the Circle button being pressed.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_button_circle(OptionsPanel* prPanel);
    
    /** Respond to the Cross (X) button being pressed.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_button_cross(OptionsPanel* prPanel);
    
    /** Respond to the Left Trigger being pressed.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_button_ltrigger(OptionsPanel* prPanel);
    
    /** Respond to the Right Trigger being pressed.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_button_rtrigger(OptionsPanel* prPanel);
    
    /** Respond to the Square button being pressed.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_button_square(OptionsPanel* prPanel);
    
    /** Respond to the Triangle button being pressed.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_button_triangle(OptionsPanel* prPanel);
    
    /** Respond to the D-Pad being pressed down.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_dpad_down(OptionsPanel* prPanel);
    
    /** Respond to the D-Pad being pressed left.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_dpad_left(OptionsPanel* prPanel);
    
    /** Respond to the D-Pad being pressed right.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_dpad_right(OptionsPanel* prPanel);
    
    /** Respond to the D-Pad being pressed up.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_dpad_up(OptionsPanel* prPanel);

    /** Initialize an Options Panel Applet.
     * 
     * @param prPanel Pointer to the OptionsPanel struct to initialize.
     * @param prConfig Pointer to a CEFiveConfig struct representing the 
     * cefive Configuration.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_init(OptionsPanel* prPanel, CEFiveConfig* prConfig);

    /** Draw an Options Panel Applet on the Debug Screen.
     * 
     * @param prPanel Pointer to an OptionsPanel struct representing the Options
     * Panel Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int optionspanel_redraw(OptionsPanel* prPanel);

#ifdef	__cplusplus
}
#endif

#endif	/* OPTIONSPANEL_H */
