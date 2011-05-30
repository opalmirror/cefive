/* 
 * File:   cefive.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 11:43 AM
 */

#ifndef _CEFIVE_H
#define	_CEFIVE_H

#include <pspkerneltypes.h>
#include <psploadcore.h>
#include "cefiveui.h"
#include "cheatengine.h"
#include "searchengine.h"
#include "cefiveconfig.h"
#include "gameinfo.h"
#include "geelog.h"
#include "ggame.h"

/** Indicates success. */
#define CEFIVE_SUCCESS      (0)

/** Indicates failure. */
#define CEFIVE_FAILURE      (-1)

/** Indicates a memory error. */
#define CEFIVE_MEMORY       (-2)

/** Indicates a NULL pointer. */
#define CEFIVE_NULLPTR      (-3)

/** The length of a SCE Game Id */
#define CEFIVE_GAMEID_LEN   (10)

/** The Major Version Number */
#define CEFIVE_VERSION_MAJ  0

/** The Minor Version Number */
#define CEFIVE_VERSION_MIN  1

/** The module name of the Kernel Library */
#define CEFIVE_KERNEL_LIB   "sceKernelLibrary"

/** The path to the UMD Data for the UMD Drive. */
#define CEFIVE_UMD_PATH     "disc0:/UMD_DATA.BIN"

/** The path to CFW Plugins. */
#define CEFIVE_PLUGIN_PATH "ms0:/seplugins"

/** The CFW Plugins directory. */
#define CEFIVE_PLUGIN_DIR "seplugins"

/** The CEFive Data Folder */
#define CEFIVE_DATA_DIR "NitePr"

/** The CEFive Configuration File */
#define CEFIVE_CONFIG_FILE "CEFive.cdf"

/** The name of the CEFive Thread. */
#define CEFIVE_THREAD_NAME "CEFiveThread"

/** The Init Priority of the CEFive Thread. */
#define CEFIVE_INIT_PRIO (24)

/** The Stack Size of the CEFive Thread. */
#define CEFIVE_STACK_SIZE (16384)

/** The Name of the Run State Semaphore. */
#define CEFIVE_RSS_NAME "CEFiveRSSema"

/** The Length of a Path */
#define CEFIVE_PATH_LEN (255)

/** STOPPED Signal */
#define CEFIVE_S_STOPPED (0)

/** FAULT Signal */
#define CEFIVE_S_FAULT (9)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ECEState {
        CES_Fault, /** An Engine Fault has occured. */
        CES_Stopped, /** Engine is stopped. */
        CES_Starting, /** Engine is starting. */
        CES_Started, /** Engine is started. */
        CES_Running, /** Engine is running. */
        CES_UIRequest, /** User Interface is requested. */
        CES_UIShowing, /** User Interface is showing. */
        CES_UIDismissed, /** User Interface is dismissed. */
        CES_CheatUpdate, /** Cheats being updated. */
        CES_Stopping /** Engine is stopping. */
    } 
    /** Enumeration indicating the cefive Engine Run State. */
    CEState;

    typedef enum _ECEStartState {
        CESS_Fault, /** A Fault has occured. */
        CESS_WaitKernelLib, /** Waiting for Kernel Library */
        CESS_WaitGameId, /** Waiting for Game Id */
        CESS_WaitGameModule, /** Waiting for Game Module */
        CESS_WaitCheatLoad, /** Waiting for Cheats to Load */
        CESS_InitVram, /** Waiting for initial VRAM */
        CESS_InitCtrl, /** Waiting for Controller */
        CESS_WaitVram, /** Waiting for debug VRAM */
        CESS_Finished /** Finished Starting */
    } 
    /** Enuemeration indicating the cefive Engine Start State. */
    ECEStartState;

    typedef struct _CEFive {
        /** Logger struct pointer */
        GeeLog* prLogger;
        /** cefive Configuration struct */
        CEFiveConfig rConfig;
        /** Cheat Engine struct */
        CheatEngine rCheatEngine;
        /** Search Engine struct */
        SearchEngine rSearchEngine;
        /** User Interface struct */
        CEFiveUi rUi;
        /** Game Info struct */
        GameInfo rGameInfo;
        /** Engine Run State */
        CEState rRunState;
        /** Engine Start State */
        ECEStartState rStart;
        /** Pointer to the Kernel Library Module */
        SceModule* prKernelLib;
        /** Pointer to the Game Module */
        SceModule* prGame;
        /** Pointer to debug VRAM */
        void* prVideoRam;
        /** String containing the SCE Game Id */
        char sGameId[CEFIVE_GAMEID_LEN + 1];
        /** RunState Signal UID */
        SceUID rSigRunState;
        /** GGame struct representing the current Game. */
        GGame rGame;
        /** UID of Input Events */
        SceUID rIEvtUID;
    }
    /** Structure representing a Cheat Engine of Five. */
    CEFive;

    /** Return a pointer to a CheatEngine struct representing the Cheat Engine.
     * 
     * @return A pointer to a CheatEngine struct is returned.
     */
    CheatEngine* cefive_get_cheatengine();
    
    /** Return a pointer to a CEFiveConfig struct representing the CEFive
     * Configuration.
     * 
     * @return A pointer to a CEFiveConfig struct is returned.
     */
    CEFiveConfig* cefive_get_config();
    
    /** Return a pointer to a GGame struct representing the current Game.
     * 
     * @return A pointer to a GGame struct is returned.
     */
    GGame* cefive_get_game();
    
    /** Return a pointer to a CEFiveUi struct representing the CEFive User
     * Interface.
     * 
     * @return A pointer to a CEFiveUi struct is returned.
     */
    CEFiveUi* cefive_get_interface();
    
    /** Return a pointer to a SceModule struct representing the 
     * sceKernelLibrary library.
     * 
     * @return A pointer to a SceModule struct or NULL is returned.
     */
    SceModule* cefive_get_kernellib();
    
    /** Return a pointer to a SearchEngine struct representing the Search 
     * Engine.
     * 
     * @return A pointer to a SearchEngine struct is returned.
     */
    SearchEngine* cefive_get_searchengine();
    
    /** Return the Thread Id of the CEFive thread.
     * 
     * @return A SceUID containing the Thread Id is returned.
     */
    SceUID cefive_get_threadid();
    
    /** Initialize Cheat Engine of Five.
     * 
     * @param prLog Pointer to a GeeLog struct representing the Logger.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefive_init(GeeLog* prLog);
    
    /** Start Cheat Engine of Five.
     * 
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefive_start();
    
    /** Stop Cheat Engine of Five.
     * 
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefive_stop();

#ifdef	__cplusplus
}
#endif

#endif	/* _CEFIVE_H */

