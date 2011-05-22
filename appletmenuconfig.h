/* 
 * File:   appletmenuconfig.h
 * Author: Sir Gee of Five
 *
 * Created on May 21, 2011, 11:03 AM
 */

#ifndef APPLETMENUCONFIG_H
#define	APPLETMENUCONFIG_H

#include "colorconfig.h"
#include "dimension.h"
#include "cursorpos.h"

/** Indicates success. */
#define APPLETMENUCONFIG_SUCCESS (0)

/** Indicates failure. */
#define APPLETMENUCONFIG_FAILURE (-1)

/** Indicates a NULL pointer. */
#define APPLETMENUCONFIG_NULLPTR (-2)

/** Indicates a memory error. */
#define APPLETMENUCONFIG_MEMORY (-3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _AppletMenuConfig {
        /** ColorConfig struct representing the base color. */
        ColorConfig color;
        /** ColorConfig struct representing the cursor color. */
        ColorConfig cursor;
        /** Dimension struct representing the size of the menu. */
        Dimension size;
        /** CursorPos struct representing the top left corner of the menu. */
        CursorPos top;
    } 
    /** The AppletMenuConfig struct is used to represent the Applet Menu
     * Configuration.
     */
    AppletMenuConfig;

    ColorConfig* appletmenuconfig_get_cursorcolor(AppletMenuConfig* prConfig);
    
    ColorConfig* appletmenuconfig_get_panelcolor(AppletMenuConfig* prConfig);
    
    /** Return a pointer to a CursorPos struct representing the position of
     * an Applet Menu on the Debug screen.
     * 
     * @param prConfig Pointer to an AppletMenuConfig struct representing the
     * Applet Menu Configuration.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* appletmenuconfig_get_position(AppletMenuConfig* prConfig);
    
    /** Return a pointer to a Dimension struct representing the size of an
     * Applet Menu on the Debug screen.
     * 
     * @param prConfig Pointer to an AppletMenuConfig struct representing the
     * Applet Menu Configuration.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* appletmenuconfig_get_size(AppletMenuConfig* prConfig);
    
    /** Initialize an Applet Menu Configuration.
     * 
     * @param prConfig Pointer to the AppletMenuConfig struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenuconfig_init(AppletMenuConfig* prConfig);
    
#ifdef	__cplusplus
}
#endif

#endif	/* APPLETMENUCONFIG_H */

