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

#define CEFIVECONFIG_SUCCESS    (0)
#define CEFIVECONFIG_FAILURE    (-1)
#define CEFIVECONFIG_MEMORY     (-2)
#define CEFIVECONFIG_NULLPTR    (-3)
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

    /* Copy the specified CEFiveConfig struct to a destination CEFiveConfig struct.
     * 
     * Parameters:
     *   prDest     Pointer to a CEFiveConfig struct to act as the destination.
     *   prSrc      Pointer to a CEFiveConfig struct to act as the source.
     * 
     * Returns:
     *   CEFIVECONFIG_NULLPTR is returned if either parameter is NULL.
     *   CEFIVECONFIG_FAILURE is returned if the structure failed to copy.
     *   CEFIVECONFIG_SUCCESS is returned if the structure was copied.
     */
    int cefiveconfig_copy(CEFiveConfig* prDest, CEFiveConfig* prSrc);

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
    int cefiveconfig_write(CEFiveConfig* prConfig, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif	/* _CEFIVECONFIG_H */
