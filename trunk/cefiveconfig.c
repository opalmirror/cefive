#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include <stdlib.h>
#include <string.h>
#include "appletconfig.h"
#include "panelconfig.h"
#include "colorconfig.h"
#include "dimension.h"
#include "cursorpos.h"
#include "cefiveconfig.h"
#include "cefive.h"

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
int cefiveconfig_copy(CEFiveConfig* prDest, CEFiveConfig* prSrc) {
    int r = 0;
    if (prDest == NULL || prSrc == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    strcpy(prDest->plugins_dir, prSrc->plugins_dir);
    strcpy(prDest->cefive_dir, prSrc->cefive_dir);
    strcpy(prDest->screenshot_path, prSrc->screenshot_path);
    strcpy(prDest->cheatfile_path, prSrc->cheatfile_path);
    strcpy(prDest->game_id, prSrc->game_id);
    prDest->pause_during_ui = prSrc->pause_during_ui;
    r = appletconfig_copy(&prDest->rAppletConfig, &prSrc->rAppletConfig);
    if (r != APPLETCONFIG_SUCCESS) {
        return CEFIVECONFIG_FAILURE;
    }
    return CEFIVECONFIG_SUCCESS;
}

/* Return a pointer to the member AppletConfig struct from a specified
 * CEFiveConfig struct.
 * 
 * Parameters:
 *   prConfig   Pointer to a CEFiveConfig struct holding cefive settings.
 * 
 * Returns:
 *   A pointer to an AppletConfig struct.
 */
AppletConfig* cefiveconfig_get_appletconfig(CEFiveConfig* prConfig) {
    if (prConfig != NULL) {
        return &prConfig->rAppletConfig;
    }
    return NULL;
}

/* Copy the full path to the current cheat file into the specified destination
 * string.
 * 
 * Parameters:
 *   sDest      A char pointer to act as the destination.  The path will be
 *              copied into this string.
 *   prConfig   Pointer to a CEFiveConfig struct holding cefive settings.
 * 
 * Returns:
 *   CEFIVECONFIG_NULLPTR if the specified CEFiveConfig pointer is NULL.
 *   CEFIVECONFIG_SUCCESS if the path was copied to the destination.
 */
int cefiveconfig_get_cheatfilename(char* sDest, CEFiveConfig* prConfig) {
    char* sPlugins = NULL;
    char* sCefive = NULL;
    char* sGameId = NULL;
    if (prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    sPlugins = prConfig->plugins_dir;
    sCefive = prConfig->cefive_dir;
    sGameId = prConfig->game_id;
    sprintf(sDest, "ms0:/%s/%s/%s.txt", sPlugins, sCefive, sGameId);
    return CEFIVECONFIG_SUCCESS;
}

ColorConfig* cefiveconfig_get_cursorcolor(CEFiveConfig* prConfig) {
    PanelConfig* prPanel = cefiveconfig_get_panelconfig(prConfig);
    if (prPanel != NULL) {
        return panelconfig_get_cursorcolor(prPanel);
    }
    return NULL;
}

ColorConfig* cefiveconfig_get_editcolor(CEFiveConfig* prConfig) {
    PanelConfig* prPanel = cefiveconfig_get_panelconfig(prConfig);
    if (prPanel != NULL) {
        return panelconfig_get_editcolor(prPanel);
    }
    return NULL;
}

ColorConfig* cefiveconfig_get_panelcolor(CEFiveConfig* prConfig) {
    PanelConfig* prPanel = cefiveconfig_get_panelconfig(prConfig);
    if (prPanel != NULL) {
        return panelconfig_get_panelcolor(prPanel);
    }
    return NULL;
}

/* Return a pointer to the member PanelConfig struct containing Panel
 * Configuration settings from the specified CEFiveConfig struct.
 * 
 * Parameters:
 *   prConfig   Pointer to a CEFiveConfig struct containing cefive settings.
 * 
 * Returns:
 *   Pointer to a PanelConfig struct.
 */
PanelConfig* cefiveconfig_get_panelconfig(CEFiveConfig* prConfig) {
    AppletConfig* prApp = cefiveconfig_get_appletconfig(prConfig);
    if (prApp != NULL) {
        return appletconfig_get_panelconfig(prApp);
    }
    return NULL;
}

ColorConfig* cefiveconfig_get_statuscolor(CEFiveConfig* prConfig) {
    AppletConfig* prApp = cefiveconfig_get_appletconfig(prConfig);
    if (prApp != NULL) {
        return appletconfig_get_statuscolor(prApp);
    }
    return NULL;
}

ColorConfig* cefiveconfig_get_titlecolor(CEFiveConfig* prConfig) {
    AppletConfig* prApp = cefiveconfig_get_appletconfig(prConfig);
    if (prApp != NULL) {
        return appletconfig_get_titlecolor(prApp);
    }
    return NULL;
}

int cefiveconfig_init(CEFiveConfig* prConfig) {
    AppletConfig* prApp = NULL;
    PanelConfig* prPanel = NULL;
    ColorConfig* prColor = NULL;
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;
    int r = 0;

    if (prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    memset(prConfig->cefive_dir, 0, CEFIVE_PATH_MAX);
    memset(prConfig->cheatfile_path, 0, CEFIVE_PATH_MAX);
    memset(prConfig->game_id, 0, CEFIVE_GAMEID_LEN + 1);
    memset(prConfig->plugins_dir, 0, CEFIVE_PATH_MAX);
    memset(prConfig->screenshot_path, 0, CEFIVE_PATH_MAX);
    prConfig->pause_during_ui = 0;
    prApp = &prConfig->rAppletConfig;
    r = appletconfig_init(prApp);
    if (r != APPLETCONFIG_SUCCESS) {
        return CEFIVECONFIG_FAILURE;
    }
    prPanel = &prApp->rPanel;
    prColor = &prPanel->rColor;
    colorconfig_setcolor(prColor, CEFIVE_DEF_BG, CEFIVE_DEF_TEXT);
    prColor = &prPanel->rCursor;
    colorconfig_setcolor(prColor, CEFIVE_CURSOR_BG, CEFIVE_CURSOR_TEXT);
    prColor = &prPanel->rEdit;
    colorconfig_setcolor(prColor, CEFIVE_EDIT_BG, CEFIVE_EDIT_TEXT);
    prPos = &prPanel->rTop;
    prPos->x = 0;
    prPos->y = 0;
    prSize = &prPanel->rSize;
    prSize->height = 34;
    prSize->width = 70;
    prColor = &prApp->rStatus;
    colorconfig_setcolor(prColor, CEFIVE_STATUS_BG, CEFIVE_STATUS_TEXT);
    prColor = &prApp->rTitlebar;
    colorconfig_setcolor(prColor, CEFIVE_TITLE_BG, CEFIVE_TITLE_TEXT);

    return CEFIVECONFIG_SUCCESS;
}

int cefiveconfig_load(CEFiveConfig* prConfig, const char* sFilename) {
    SceUID fd = -1;
    int r = 0;
    
    if (prConfig == NULL || sFilename == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    fd = sceIoOpen(sFilename, PSP_O_RDONLY, 0664);
    if (fd < 0) {
        return CEFIVECONFIG_IOERROR;
    }
    r = cefiveconfig_read(prConfig, fd);
    if (r != CEFIVECONFIG_SUCCESS) {
        sceIoClose(fd);
        return CEFIVECONFIG_FAILURE;
    }
    sceIoClose(fd);
    return CEFIVECONFIG_SUCCESS;
}

int cefiveconfig_read(CEFiveConfig* prConfig, SceUID fd) {
    int r = 0;
    if (prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    if (fd < 0) {
        return CEFIVECONFIG_IOERROR;
    }
    r = sceIoRead(fd, &prConfig->pause_during_ui, sizeof(int));
    if (r != sizeof(int)) {
        return CEFIVECONFIG_IOERROR;
    }
    r = appletconfig_read(&prConfig->rAppletConfig, fd);
    if (r != APPLETCONFIG_SUCCESS) {
        return CEFIVECONFIG_FAILURE;
    }
    return CEFIVECONFIG_SUCCESS;
}

int cefiveconfig_save(CEFiveConfig* prConfig, const char* sFilename) {
    SceUID fd = -1;
    int r = 0;
    
    if (prConfig == NULL || sFilename == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    fd = sceIoOpen(sFilename, PSP_O_WRONLY|PSP_O_CREAT, 0664);
    if (fd < 0) {
        return CEFIVECONFIG_IOERROR;
    }
    r = cefiveconfig_write(prConfig, fd);
    if (r != CEFIVECONFIG_SUCCESS) {
        sceIoClose(fd);
        return CEFIVECONFIG_FAILURE;
    }
    sceIoClose(fd);
    return CEFIVECONFIG_SUCCESS;
}

int cefiveconfig_set_cefivedir(CEFiveConfig* prConfig, const char* sDir) {
    if (prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    strncpy(prConfig->cefive_dir, sDir, CEFIVE_DIR_MAX);
    return CEFIVECONFIG_SUCCESS;
}

int cefiveconfig_set_plugindir(CEFiveConfig* prConfig, const char* sDir) {
    if (prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    strncpy(prConfig->plugins_dir, sDir, CEFIVE_DIR_MAX);
    return CEFIVECONFIG_SUCCESS;
}

int cefiveconfig_write(CEFiveConfig* prConfig, SceUID fd) {
    int r = 0;
    if (prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    if (fd < 0) {
        return CEFIVECONFIG_IOERROR;
    }
    r = sceIoWrite(fd, &prConfig->pause_during_ui, sizeof(int));
    if (r != sizeof(int)) {
        return CEFIVECONFIG_IOERROR;
    }
    r = appletconfig_write(&prConfig->rAppletConfig, fd);
    if (r != APPLETCONFIG_SUCCESS) {
        return CEFIVECONFIG_FAILURE;
    }
    return CEFIVECONFIG_SUCCESS;
}
