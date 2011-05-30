/* 
 * File:   cefiveconfig.h
 * Author: Sir Gee of Five
 *
 * Created on September 26, 2010, 2:36 PM
 */

#ifndef _CEFIVECONFIG_H
#define	_CEFIVECONFIG_H

#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "panelconfig.h"
#include "appletconfig.h"

/** Indicates success. */
#define CEFIVECONFIG_SUCCESS    (0)

/** Indicates failure. */
#define CEFIVECONFIG_FAILURE    (-1)

/** Indicates a memory error. */
#define CEFIVECONFIG_MEMORY     (-2)

/** Indicates a NULL pointer. */
#define CEFIVECONFIG_NULLPTR    (-3)

/** Indicates an I/O error. */
#define CEFIVECONFIG_IOERROR    (-4)

#define CEFIVE_DIR_MAX      (64)
#define CEFIVE_GAMEID_LEN   (10)
#define CEFIVE_PATH_MAX     (256)

#define CEFIVE_DEF_BG       ((u32)0xFFC0C0C0)
#define CEFIVE_DEF_TEXT     ((u32)0xFF000000)
#define CEFIVE_CURSOR_BG    ((u32)0xFFFF0000)
#define CEFIVE_CURSOR_TEXT  ((u32)0xFFFFFFFF)
#define CEFIVE_EDIT_BG      ((u32)0xFF808080)
#define CEFIVE_EDIT_TEXT    ((u32)0xFF00FFFF)
#define CEFIVE_STATUS_BG    ((u32)0xFF909090)
#define CEFIVE_STATUS_TEXT  ((u32)0xFF000000)
#define CEFIVE_TITLE_BG     ((u32)0xFF0000A0)
#define CEFIVE_TITLE_TEXT   ((u32)0xFFFFFFFF)


#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct _CEFiveConfig {
        char plugins_dir[CEFIVE_DIR_MAX];
        char cefive_dir[CEFIVE_DIR_MAX];
        char screenshot_path[CEFIVE_PATH_MAX];
        char cheatfile_path[CEFIVE_PATH_MAX];
        char game_id[CEFIVE_GAMEID_LEN + 1];
        int pause_during_ui;
        AppletConfig rAppletConfig;
    }CEFiveConfig;

    /** Copy a CeFive Configuration.
     * 
     * @param prDest Pointer to a CEFiveConfig struct representing the 
     * destination.
     * @param prSrc Pointer to a CEFiveConfig struct representing the source.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int cefiveconfig_copy(CEFiveConfig* prDest, CEFiveConfig* prSrc);

    /** Return a pointer to an AppletConfig struct representing the CeFive
     * Applet Configuration.
     * 
     * @param prConfig Pointer to a CEFiveConfig struct representing the CeFive
     * Configuration.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* cefiveconfig_get_appletconfig(CEFiveConfig* prConfig);
    
    int cefiveconfig_get_cheatfilename(char* sDest, CEFiveConfig* prConfig);
    
    ColorConfig* cefiveconfig_get_cursorcolor(CEFiveConfig* prConfig);
    
    ColorConfig* cefiveconfig_get_editcolor(CEFiveConfig* prConfig);
    
    ColorConfig* cefiveconfig_get_panelcolor(CEFiveConfig* prConfig);
    
    PanelConfig* cefiveconfig_get_panelconfig(CEFiveConfig* prConfig);
    
    ColorConfig* cefiveconfig_get_statuscolor(CEFiveConfig* prConfig);
    
    ColorConfig* cefiveconfig_get_titlecolor(CEFiveConfig* prConfig);
    
    int cefiveconfig_init(CEFiveConfig* prConfig);
    
    int cefiveconfig_load(CEFiveConfig* prConfig, const char* sFilename);
    
    int cefiveconfig_read(CEFiveConfig* prConfig, SceUID fd);
    
    int cefiveconfig_save(CEFiveConfig* prConfig, const char* sFilename);
    
    /** Assign the CEFive directory.
     * 
     * @param prConfig Pointer to a CEFiveConfig struct representing the 
     * CEFive Configuration.
     * @param sDir string containing the directory to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveconfig_set_cefivedir(CEFiveConfig* prConfig, const char* sDir);
    
    /** Assign the CFW plugins directory.
     * 
     * @param prConfig Pointer to a CEFiveConfig struct representing the 
     * CEFive Configuration.
     * @param sDir string containing the directory to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveconfig_set_plugindir(CEFiveConfig* prConfig, const char* sDir);
    
    int cefiveconfig_write(CEFiveConfig* prConfig, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif	/* _CEFIVECONFIG_H */
