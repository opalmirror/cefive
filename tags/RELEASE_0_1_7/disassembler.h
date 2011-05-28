/* disassembler.h
 *  The Disassembler Applet for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "appletconfig.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "gameinfo.h"
#include "geelog.h"
#include "memviewpanel.h"
#include "mips.h"

/** Indicates success. */
#define DISASSEMBLER_SUCCESS    (0)

/** Indicates failure. */
#define DISASSEMBLER_FAILURE    (-1)

/** Indicates a memory error. */
#define DISASSEMBLER_MEMORY     (-2)

/** Indicates a NULL pointer. */
#define DISASSEMBLER_NULLPTR    (-3)
    
/** Indicates an address that is out of bounds. */
#define DISASSEMBLER_BADADDR    (-4)

/** Default background color. */
#define DISASSEMBLER_DEFBGCOLOR ((u32)0xFFD0D0D0)

/** Default foreground color. */
#define DISASSEMBLER_DEFFGCOLOR ((u32)0xFF000000)

/** The number of elements in the Jump Stack. */
#define DISASSEMBLER_JSLEN      10

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Disassembler {
        /** Memory View Panel struct */
        MemViewPanel memViewPanel;
        /** Applet Configuration struct pointer */
        AppletConfig* prApCfg;
        /** Game Info struct pointer */
        GameInfo* prGameInfo;
        /** Editing indicator */
        int editing;
        /** Indicates that Disassembler needs redrawing. */
        int dirty;
    } 
    /** The Disassembler struct represents the CeFive Disassembler Applet. */
    Disassembler;

    /** Respond to the user pressing the Circle button.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_button_circle(Disassembler* prDasm);
    
    /** Respond to the user pressing the Cross (X) button.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_button_cross(Disassembler* prDasm);
    
    /** Respond to the user pressing the Left Trigger.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_button_ltrigger(Disassembler* prDasm);
    
    /** Respond to the user pressing the Right Trigger.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_button_rtrigger(Disassembler* prDasm);
    
    /** Respond to the user pressing the Square button.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_button_square(Disassembler* prDasm);

    /** Respond to the user pressing the Triangle button.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_button_triangle(Disassembler* prDasm);

    /** Respond to the user pressing the D-Pad down.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_dpad_down(Disassembler* prDasm);
    
    /** Respond to the user pressing the D-Pad left.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_dpad_left(Disassembler* prDasm);
    
    /** Respond to the user pressing the D-Pad right.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_dpad_right(Disassembler* prDasm);
    
    /** Respond to the user pressing the D-Pad up.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt; 0 indicates failure.
     */
    int disassembler_dpad_up(Disassembler* prDasm);

    /** Return a pointer to an AppletConfig struct representing the Applet
     * Configuration.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* disassembler_get_appletconfig(Disassembler* prDasm);

    /** Return a pointer to a ColorConfig struct representing the Cursor
     * Color Configuration.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* disassembler_get_cursorcolor(Disassembler* prDasm);
    
    /** Return a pointer to a MemViewPanel struct representing the Memory View
     * Panel.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a MemViewPanel struct or NULL is returned.
     */
    MemViewPanel* disassembler_get_memview(Disassembler* prDasm);
    
    /** Return a pointer to a ColorConfig struct representing the Panel Color
     * Configuration.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* disassembler_get_panelcolor(Disassembler* prDasm);
    
    /** Return a pointer to a CursorPos struct representing the position of
     * the Disassembler Applet on the Debug Screen.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* disassembler_get_position(Disassembler* prDasm);

    /** Return a pointer to a Dimension struct representing the size of the
     * Disassembler Applet on the Debug Screen.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* disassembler_get_size(Disassembler* prDasm);
    
    /** Return a pointer to a ColorConfig struct representing the Status Color
     * Configuration.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* disassembler_get_statuscolor(Disassembler* prDasm);
    
    /** Initialize a Disassembler Applet.
     * 
     * @param prDasm Pointer to the Disassembler struct to initialize.
     * @param prApCfg Pointer to an AppletConfig struct representing the Applet
     * Configuration.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int disassembler_init(Disassembler* prDasm, AppletConfig* prApCfg);
    
    /** Return an indication of whether the Disassembler is currently editing.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates not editing, 1 indicates editing.
     */
    int disassembler_is_editing(Disassembler* prDasm);
    
    /** Scroll the Disassembler Applet down by a Page.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int disassembler_page_down(Disassembler* prDasm);
    
    /** Scroll the Disassembler Applet up by a Page.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int disassembler_page_up(Disassembler* prDasm);

    /** Redraw the Disassembler Applet on the Debug Screen.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int disassembler_redraw(Disassembler* prDasm);

    /** Reposition the Disassembler to the indicated offset.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @param offset SceUInt32 containing the address to reposition to.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int disassembler_seek(Disassembler* prDasm, SceUInt32 offset);
    
    /** Return the current address of a Disassembler Applet.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return SceUInt32 containing the current address.
     */
    SceUInt32 disassembler_tell(Disassembler* prDasm);

#ifdef	__cplusplus
}
#endif

#endif
