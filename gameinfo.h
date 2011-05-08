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

#define GAMEINFO_SUCCESS    (0)
#define GAMEINFO_FAILURE    (-1)
#define GAMEINFO_MEMORY     (-2)
#define GAMEINFO_NULLPTR    (-3)

#define GAMEINFO_MAXSTUBCT  64

#ifdef	__cplusplus
extern "C" {
#endif

    /** Configuration settings for the Game Info applet. */
    typedef struct _GameInfoConfig {
        ColorConfig color; /** ColorConfig struct containing color settings. */
        CursorPos position; /** CursorPos struct containing the cursor position. */
    } GameInfoConfig;

    typedef struct _GameInfo {
        /** Pointer to a GeeLog struct acting as the Logger. */
        GeeLog* prLog;
        /** Pointer to a GameInfoConfig struct containing the configuration. */
        GameInfoConfig config;
        /** Pointer to a AppletConfig struct containing Applet configuration. */
        AppletConfig* prApCfg;
        /** The number of loaded modules. */
        int module_count;
        /** Whether a GameInfo has been loaded. */
        int loaded;
        /** An array of SceUID values representing the UIDs of the import stubs. */
        SceUID arUID[GAMEINFO_MAXSTUBCT];
        /** A Pointer to the SceModule struct representing the GameModule */
        SceModule* prModule;
        /** Pointer to the SceLibraryEntryTable struct for the Game. */
        SceLibraryEntryTable* prLibTable;
        /** Array of SceLibraryStubTable pointers representing imported Library 
         * stubs. */
        SceLibraryStubTable * aprStubTable[GAMEINFO_MAXSTUBCT];
        /** The number of entries in the export table. */
        int libEntryCount;
        /** The number of entries in the import table. */
        int libStubCount;
        /** Pointer to the end address of the .text segment. */
        void* textEnd;
        /** Pointer to the Game Id of the Game. */
        char* sGameId;
    } GameInfo;

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

    /** Add a log statement to the configured GeeLog of a GameInfo.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @param rLevel ELogLevel value specifying the Log Level of the Message.
     * @param sMsg const char pointer to the message to add.
     * @return GAMEINFO_NULLPTR is returned if the prInfo parameter is NULL.
     * GAMEINFO_FAILURE is returned if the statement could not be added.
     * GAMEINFO_SUCCESS is returned if the statement is added.
     */
    int gameinfo_log(GameInfo* prInfo, ELogLevel rLevel, const char* sMsg);

    /** Redraw the Game Info interface screen.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @return GAMEINFO_NULLPTR is returned if the prInfo parameter is NULL.
     * GAMEINFO_FAILURE is returned if the Game Info screen could not be redrawn.
     * GAMEINFO_SUCCESS is returned if the Game Info screen is redrawn.
     */
    int gameinfoRedraw(GameInfo* prInfo);

    /** Assign an AppletConfig to the specified GameInfo.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @param prCfg Pointer to an AppletConfig struct to assign.
     * @return GAMEINFO_NULLPTR is returned if prInfo is NULL.  GAMEINFO_SUCCESS is
     * returned if the AppletConfig is assigned.
     */
    int gameinfo_set_appletconfig(GameInfo* prInfo, AppletConfig* prCfg);

    /** Assign a ColorConfig to the specified GameInfo.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @param prColor Pointer to the ColorConfig struct to assign.
     * @return GAMEINFO_NULLPTR is returned if either parameter is NULL.
     * GAMEINFO_SUCCESS is returned if the ColorConfig is assigned.
     */
    int gameinfo_set_colorconfig(GameInfo* prInfo, ColorConfig* prColor);

    /** Assign the cursor position of a Game Info by specifying the x and y
     * coordinates.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @param x int containing the cursor column to assign.
     * @param y int containing the cursor row to assign.
     * @return GAMEINFO_NULLPTR is returned if the prInfo parameter is NULL.
     * GAMEINFO_SUCCESS is returned if the cursor position is assigned.
     */
    int gameinfo_set_cursor(GameInfo* prInfo, int x, int y);

    /** Assign a CursorPos to the specified GameInfo.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @param prPos Pointer to the CursorPos struct to assign.
     * @return GAMEINFO_NULLPTR is returned if either parameter is NULL.
     * GAMEINFO_SUCCESS is returned if the CursorPos is assigned.
     */
    int gameinfo_set_cursorpos(GameInfo* prInfo, CursorPos* prPos);

    /** Assign a GeeLog to the specified GameInfo.  A GeeLog is assigned to 
     * facilitate trace logging to a text file.
     * 
     * @param prInfo Pointer to a GameInfo struct representing the Game Info.
     * @param prLog Pointer to the GeeLog struct to assign.
     * @return GAMEINFO_NULLPTR is returned if the prInfo parameter is NULL.
     * GAMEINFO_SUCCESS is returned if the GeeLog is assigned.
     */
    int gameinfo_set_logger(GameInfo* prInfo, GeeLog *prLog);

#ifdef	__cplusplus
}
#endif

#endif	/* _GAMEINFO_H */

