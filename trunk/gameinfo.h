/* 
 * File:   gameinfo.h
 * Author: Sir Gee of Five
 *
 * Created on September 23, 2010, 4:35 PM
 */

#ifndef _GAMEINFO_H
#define	_GAMEINFO_H

#include <pspkerneltypes.h>
#include <pspmodulemgr.h>
#include <psploadcore.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "appletconfig.h"
#include "geelog.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define GAMEINFO_SUCCESS    (0)
#define GAMEINFO_FAILURE    (-1)
#define GAMEINFO_MEMORY     (-2)
#define GAMEINFO_NULLPTR    (-3)

#define GAMEINFO_MAXSTUBCT  64

typedef struct _GameInfoConfig {
    ColorConfig color;
    CursorPos position;
}GameInfoConfig;

typedef struct _GameInfo {
    GeeLog* prLog;
    GameInfoConfig config;
    AppletConfig* prApCfg;
    int module_count;
    int loaded;
    SceUID arUID[GAMEINFO_MAXSTUBCT];
    SceModule* prModule;
    SceLibraryEntryTable* prLibTable;
    SceLibraryStubTable* aprStubTable[GAMEINFO_MAXSTUBCT];
    int libEntryCount;
    int libStubCount;
    void* textEnd;
    char* sGameId;
}GameInfo;

/** Return a pointer to the ColorConfig struct that belongs to the specified
 * GameInfoConfig.
 * 
 * @param prCfg Pointer to a GameInfoConfig struct.
 * @return Pointer to the ColorConfig struct or NULL.
 */
ColorConfig* gameinfoconfig_get_colorconfig(GameInfoConfig* prCfg);

/** Return a pointer to the CursorPos struct that belongs to the specified
 * GameInfoConfig.
 * 
 * @param prCfg Pointer to a GameInfoConfig struct.
 * @return Pointer to the CursorPos struct or NULL.
 */
CursorPos* gameinfoconfig_get_cursorpos(GameInfoConfig* prCfg);

/** Initialize the specified GameInfoConfig struct.
 * 
 * @param prCfg Pointer to the GameInfoConfig struct to initialize.
 * @return GAMEINFO_NULLPTR is returned if the parameter prCfg is NULL.
 * GAMEINFO_FAILURE is returned if the GameInfoConfig struct could not be
 * initialized.  GAMEINFO_SUCCESS is returned if the GameInfoConfig struct
 * is initialized.
 */
int gameinfoconfig_init(GameInfoConfig* prCfg);

/** Assign a ColorConfig to the specified GameInfoConfig.
 * 
 * @param prCfg Pointer to the GameInfoConfig struct to assign to.
 * @param prColor Pointer to the ColorConfig struct to assign.
 * @return GAMEINFO_NULLPTR is returned if either parameter is NULL.  
 * GAMEINFO_FAILURE is returned if the ColorConfig could not be assigned.
 * GAMEINFO_SUCCESS is returned if the ColorConfig is assigned.
 */
int gameinfoconfig_set_colorconfig(GameInfoConfig* prCfg, ColorConfig* prColor);

/** Assign the current cursor position of a specified GameInfoConfig by
 * providing an x and y coordinate.
 * 
 * @param prCfg Pointer to the GameInfoConfig struct to assign.
 * @param x int containing the x coordinate to assign.
 * @param y int containing the y coordinate to assign.
 * @return GAMEINFO_NULLPTR is returned if the parameter prCfg is NULL.
 * GAMEINFO_FAILURE is returned if the position could not be assigned.
 * GAMEINFO_SUCCESS is returned if the position is assigned.
 */
int gameinfoconfig_set_cursor(GameInfoConfig* prCfg, int x, int y);

/** Assign a CursorPos to the specified GameInfoConfig
 * 
 * @param prCfg Pointer to the GameInfoConfig struct to assign to.
 * @param prPos Pointer to the CursorPos struct to assign.
 * @return GAMEINFO_NULLPTR is returned if either parameter is NULL.
 * GAMEINFO_FAILURE is returned if the CursorPos could not be assigned.
 * GAMEINFO_SUCCESS is returned if the CursorPos is assigned.
 */
int gameinfoconfig_set_cursorpos(GameInfoConfig* prCfg, CursorPos* prPos);

/** Return a pointer to the member GameInfoConfig struct of a specified GameInfo
 * struct.
 * 
 * @param prInfo Pointer to a GameInfo struct representing the Game Info.
 * @return A pointer to a GameInfoConfig struct or NULL.
 */
GameInfoConfig* gameinfo_get_config(GameInfo* prInfo);

/** Initialize a GameInfo struct.
 * 
 * @param prInfo Pointer to the GameInfo struct to initialize.
 * @return GAMEINFO_NULLPTR is returned if parameter prInfo is NULL.
 * GAMEINFO_FAILURE is returned if the GameInfo struct could not be initialized.
 * GAMEINFO_SUCCESS is returned if the GameInfo struct is initialized.
 */
int gameinfo_init(GameInfo* prInfo);

/** Populate a GameInfo struct with values loaded from the current running
 * sceModule.
 * 
 * @param prInfo Pointer to a GameInfo struct representing the Game Info.
 * @return GAMEINFO_NULLPTR is returned if the parameter prInfo is NULL.
 * GAMEINFO_SUCCESS is returned if the GameInfo struct is populated.
 */
int gameinfo_load(GameInfo* prInfo);

int gameinfo_log(GameInfo* prInfo, ELogLevel rLevel, const char* sMsg);
int gameinfoRedraw(GameInfo* prInfo);
int gameinfo_set_appletconfig(GameInfo* prInfo, AppletConfig* prCfg);
int gameinfo_set_colorconfig(GameInfo* prInfo, ColorConfig* prColor);
int gameinfo_set_cursor(GameInfo* prInfo, int x, int y);
int gameinfo_set_cursorpos(GameInfo* prInfo, CursorPos* prPos);
int gameinfo_set_logger(GameInfo* prInfo, GeeLog *prLog);

#ifdef	__cplusplus
}
#endif

#endif	/* _GAMEINFO_H */

