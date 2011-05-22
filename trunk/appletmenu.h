/* 
 * File:   appletmenu.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 9:39 AM
 */

#ifndef _APPLETMENU_H
#define	_APPLETMENU_H

#include <pspdebug.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "appletconfig.h"
#include "appletmenuconfig.h"
#include "menumodel.h"

/** Indicates success. */
#define APPLETMENU_SUCCESS (0)

/** Indicates failure. */
#define APPLETMENU_FAILURE (-1)

/** Indicates a NULL pointer. */
#define APPLETMENU_NULLPTR (-2)

/** Indicates a memory error. */
#define APPLETMENU_MEMORY (-3)

/** Indicates an invalid index. */
#define APPLETMENU_INVIDX (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _AppletMenu {
        /** AppletMenuConfig struct representing the configuration. */
        AppletMenuConfig config;
        /** Pointer to an AppletConfig struct representing the Applet
         * Configuration.
         */
        AppletConfig* prApCfg;
        /** CursorPos struct representing the Applet Menu cursor position. */
        CursorPos cursor;
        /** MenuModel struct representing the Menu Model. */
        MenuModel model;
        /** Indicates whether an Applet Menu is visible. */
        int visible;
        /** Indicates whether an Applet Menu needs to be redrawn. */
        int dirty;
    } 
    /** The AppletMenu struct represents an Applet Menu. */
    AppletMenu;

    /** Respond to the user pressing the Circle Button.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_circle_button(AppletMenu* prMenu);
    
    /** Respond to the user pressing the Cross Button.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_cross_button(AppletMenu* prMenu);
    
    /** Move the Applet Menu Cursor down by one row.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_cursor_down(AppletMenu* prMenu);
    
    /** Move the Applet Menu Cursor up by one row.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_cursor_up(AppletMenu* prMenu);
    
    /** Return a pointer to an AppletConfig struct representing the Applet
     * Configuration.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* appletmenu_get_appletconfig(AppletMenu* prMenu);
    
    /** Return a pointer to an AppletMenuConfig struct representing the Applet
     * Menu Configuration.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to an AppletMenuConfig struct or NULL is returned.
     */
    AppletMenuConfig* appletmenu_get_config(AppletMenu* prMenu);
    
    /** Return a pointer to a ColorConfig struct representing the Cursor Color
     * Configuration.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletmenu_get_cursorcolor(AppletMenu* prMenu);
    
    /** Return a pointer to a CursorPos struct representing the Cursor Position.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* appletmenu_get_cursorpos(AppletMenu* prMenu);
    
    /** Return a pointer to a MenuModel struct representing the Menu Model.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to a MenuModel struct or NULL is returned.
     */
    MenuModel* appletmenu_get_model(AppletMenu* prMenu);
    
    /** Return a pointer to a ColorConfig struct representing the Panel Color
     * Configuration.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* appletmenu_get_panelcolor(AppletMenu* prMenu);
    
    /** Return a pointer to a CursorPos struct representing the Panel Position
     * of an Applet Menu.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* appletmenu_get_position(AppletMenu* prMenu);
    
    /** Return the currently selected index of an Applet Menu.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return The currently selected index or -1 is returned.
     */
    int appletmenu_get_selectedindex(AppletMenu* prMenu);
    
    /** Return a pointer to a Dimension struct representing the size of an
     * Applet Menu.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* appletmenu_get_size(AppletMenu* prMenu);
    
    /** Initialize an Applet Menu.
     * 
     * @param prMenu Pointer to the AppletMenu struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_init(AppletMenu* prMenu);
    
    /** Indicate that an Applet Menu needs to be redrawn.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_invalidate(AppletMenu* prMenu);
    
    /** Indicate whether an Applet Menu is visible.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates that the Applet Menu is not visible.  1 indicates
     * that the Applet Menu is visible.
     */
    int appletmenu_is_visible(AppletMenu* prMenu);
    
    /** Redraw an Applet Menu on the Debug Screen.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_redraw(AppletMenu* prMenu);
    
    /** Assign the screen position of an Applet Menu.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @param x int indicating the left hand column.
     * @param y int indicating the top row.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_set_position(AppletMenu* prMenu, int x, int y);
    
    /** Assign the size of an Applet Menu.
     * 
     * @param prMenu Pointer to an AppletMenu struct representing the Applet
     * Menu.
     * @param width int containing the width in columns.
     * @param height int containing the height in rows.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int appletmenu_set_size(AppletMenu* prMenu, int width, int height);
    
    void appletmenuCircleButton(AppletMenu* prMenu);
    void appletmenuCrossButton(AppletMenu *prMenu);
    void appletmenuCursorDown(AppletMenu *prMenu);
    void appletmenuCursorUp(AppletMenu *prMenu);
    int appletmenuGetSelectedIndex(AppletMenu *prMenu);
    void appletmenuRedraw(AppletMenu *prMenu);

#ifdef	__cplusplus
}
#endif

#endif	/* _APPLETMENU_H */

