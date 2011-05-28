/* 
 * File:   panelconfig.h
 * Author: Sir Gee of Five
 *
 * Created on October 3, 2010, 1:31 PM
 */

#ifndef PANELCONFIG_H
#define	PANELCONFIG_H

#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"

/** Indicates success. */
#define PANELCONFIG_SUCCESS     (0)

/** Indicates failure. */
#define PANELCONFIG_FAILURE     (-1)

/** Indicates a memory error. */
#define PANELCONFIG_MEMORY      (-2)

/** Indicates a NULL pointer. */
#define PANELCONFIG_NULLPTR     (-3)

/** Indicates an I/O Error. */
#define PANELCONFIG_IOERROR     (-4)
    
#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _PanelConfig {
        /** Normal color configuration. */
        ColorConfig rColor;
        /** Cursor color configuration. */
        ColorConfig rCursor;
        /** Edit color configuration. */
        ColorConfig rEdit;
        /** Cursor position of the top-left corner of the Panel. */
        CursorPos rTop;
        /** Dimension of the Panel expressed in rows and columns. */
        Dimension rSize;
    }
    /** The Panel Configuration structure holds Panel configuration settings.
     * The settings specify color and position of the Panel.
     */
    PanelConfig;

    /** Copy a PanelConfig struct.
     * 
     * @param prDest Pointer to a PanelConfig struct to act as the destination.
     * @param prSrc Pointer to a PanelConfig struct to act as the source.
     * @return PANELCONFIG_NULLPTR is returned if either parameter is NULL.
     * PANELCONFIG_SUCCESS is returned if the PanelConfig is copied.
     */
    int panelconfig_copy(PanelConfig* prDest, PanelConfig* prSrc);
    
    /** Return a pointer to a ColorConfig struct representing the cursor
     * color configuration.
     * 
     * @param prCfg Pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * @return A Pointer to the cursor ColorConfig struct is returned or NULL
     * is returned.
     */
    ColorConfig* panelconfig_get_cursorcolor(PanelConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct representing the editor
     * color configuration.
     * 
     * @param prCfg Pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * @return A Pointer to the cursor ColorConfig struct is returned or NULL
     * is returned.
     */
    ColorConfig* panelconfig_get_editcolor(PanelConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct representing the Panel
     * color configuration.
     * 
     * @param prCfg Pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * @return A Pointer to the cursor ColorConfig struct is returned or NULL
     * is returned.
     */
    ColorConfig* panelconfig_get_panelcolor(PanelConfig* prCfg);
    
    /** Return a pointer to a CursorPos struct representing the position of
     * the Panel on the screen.
     * 
     * @param prCfg Pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * @return A pointer to the CursorPos struct representing the Panel 
     * position or NULL is returned.
     */
    CursorPos* panelconfig_get_position(PanelConfig* prCfg);
    
    /** Return a pointer to a Dimension struct representing the Panel
     * Size in rows and columns.
     * 
     * @param prCfg Pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * @return A pointer to the Dimension struct representing the Panel
     * size or NULL is returned.
     */
    Dimension* panelconfig_get_size(PanelConfig* prCfg);
    
    /** Initialize a PanelConfig struct.
     * 
     * @param prCfg Pointer to the PanelConfig struct to initialize.
     * @return PANELCONFIG_NULLPTR is returned if the parameter prCfg is NULL.
     * PANELCONFIG_FAILURE is returned if the PanelConfig could not be 
     * initialized, otherwise PANELCONFIG_SUCCESS is returned.
     */
    int panelconfig_init(PanelConfig* prCfg);
    
    /** Read a PanelConfig struct from the specified file descriptor.
     * 
     * @param prCfg Pointer to a PanelConfig struct to read into.
     * @param fd SceUID containing a valid file descriptor.
     * @return PANELCONFIG_NULLPTR is returned if the parameter prCfg is NULL.
     * PANELCONFIG_FAILURE is returned if the PanelConfig could not be read.
     * PANELCONFIG_IOERROR is returned if an I/O error occured during reading.
     * PANELCONFIG_SUCCESS is returned if the PanelConfig was read.
     */
    int panelconfig_read(PanelConfig* prCfg, SceUID fd);
    
    /** Write a PanelConfig struct to the specified file descriptor.
     * 
     * @param prCfg Pointer to the PanelConfig struct to write.
     * @param fd SceUID containing a valid file descriptor.
     * @return PANELCONFIG_NULLPTR is returned if the parameter prCfg is NULL.
     * PANELCONFIG_FAILURE is returned if the PanelConfig could not be written.
     * PANELCONFIG_IOERROR is returned if an I/O error occured during writing.
     * PANELCONFIG_SUCCESS is returned if the PanelConfig was written.
     */
    int panelconfig_write(PanelConfig* prCfg, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif	/* PANELCONFIG_H */

