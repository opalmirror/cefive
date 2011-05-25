/* 
 * File:   appletconfig.h
 * Author: Sir Gee of Five
 *
 * Created on October 3, 2010, 1:35 PM
 */

#ifndef _APPLETCONFIG_H
#define	_APPLETCONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "panelconfig.h"
#include "cursorpos.h"
#include "dimension.h"

/** Indicates success.*/
#define APPLETCONFIG_SUCCESS        (0)

/** Indicates failure. */
#define APPLETCONFIG_FAILURE        (-1)

/** Indicates a memory error. */
#define APPLETCONFIG_MEMORY         (-2)

/** Indicates a NULL pointer. */
#define APPLETCONFIG_NULLPTR        (-3)

/** Indicates an I/O Error. */
#define APPLETCONFIG_IOERROR        (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _AppletConfig {
        /** Panel Configuration struct */
        PanelConfig rPanel;
        /** Status Bar Color Configuration struct */
        ColorConfig rStatus;
        /** Title Bar Color Configuration struct */
        ColorConfig rTitlebar;
    }
    /** The Applet Configuration struct contains settings for an Applet.
     * The struct contains members for the Panel Configuration as well as
     * Color Configuration settings for the Title Bar and Status Bar.
     */
    AppletConfig;

    /** Copy an AppletConfig struct.
     * 
     * @param prDest Pointer to the destination AppletConfig struct.
     * @param prSrc Pointer to the source AppletConfig struct.
     * @return 0 is returned on success, less than 0 is returned on failure.
     */
    int appletconfig_copy(AppletConfig* prDest, AppletConfig* prSrc);
    
    /** Return a pointer to the ColorConfig struct representing the cursor
     * color configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletconfig_get_cursorcolor(AppletConfig* prConfig);
    
    /** Return a pointer to the ColorConfig struct representing the editor
     * color configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletconfig_get_editcolor(AppletConfig* prConfig);
    
    /** Return a pointer to the ColorConfig struct representing the Panel
     * color configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletconfig_get_panelcolor(AppletConfig* prConfig);
    
    /** Return a pointer to the PanelConfig struct representing the Panel
     * Configuration
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a PanelConfig struct or NULL is returned.
     */
    PanelConfig* appletconfig_get_panelconfig(AppletConfig* prConfig);
    
    /** Return a pointer to a CursorPos struct representing the position of
     * the Applet on the Debug screen.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* appletconfig_get_position(AppletConfig* prConfig);
    
    /** Return a pointer to a Dimension struct representing the size of the
     * Applet on the Debug screen.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* appletconfig_get_size(AppletConfig* prConfig);
    
    /** Return a pointer to the ColorConfig struct representing the Status Bar
     * color configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletconfig_get_statuscolor(AppletConfig* prConfig);
    
    /** Return a pointer to the ColorConfig struct representing the Title Bar
     * color configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletconfig_get_titlecolor(AppletConfig* prConfig);
    
    /** Initialize an AppletConfig struct.
     * 
     * @param prConfig Pointer to the AppletConfig struct to initialize.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_init(AppletConfig* prConfig);
    
    /** Read an AppletConfig struct from a file descriptor.
     * 
     * @param prConfig Pointer to the AppletConfig struct to read into.
     * @param fd SceUID indicating a valid file descriptor.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_read(AppletConfig* prConfig, SceUID fd);

    /** Assign the Cursor Color Configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the 
     * Applet Configuration.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_cursorcolor(AppletConfig* prConfig, 
            const u32 background, const u32 text);
    
    /** Assign the Editor Color Configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the 
     * Applet Configuration.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_editcolor(AppletConfig* prConfig, 
            const u32 background, const u32 text);
    
    /** Assign the Panel Color Configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the 
     * Applet Configuration.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_panelcolor(AppletConfig* prConfig, 
            const u32 background, const u32 text);
    
    /** Assign the position of an Applet on the Debug Screen.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @param x int indicating the column of the left edge of the Applet.
     * @param y int indicating the row of the top edge of the Applet.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_position(AppletConfig* prConfig, 
            const int x, const int y);
    
    /** Assign the size of an Applet on the Debug Screen.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @param width int indicating the width in columns.
     * @param height int indicating the height in rows.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_size(AppletConfig* prConfig,
            const int width, const int height);
    
    /** Assign the Status Bar Color Configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the 
     * Applet Configuration.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_statuscolor(AppletConfig* prConfig, 
            const u32 background, const u32 text);
    
    /** Assign the Title Bar Color Configuration.
     * 
     * @param prConfig Pointer to an AppletConfig struct representing the 
     * Applet Configuration.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_set_titlecolor(AppletConfig* prConfig, 
            const u32 background, const u32 text);
    
    /** Write an AppletConfig struct to a file descriptor.
     * 
     * @param prConfig Pointer to the AppletConfig struct to write.
     * @param fd SceUID indicating a valid file descriptor.
     * @return 0 indicates success, &lt0; indicates failure.
     */
    int appletconfig_write(AppletConfig* prConfig, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif	/* _APPLETCONFIG_H */

