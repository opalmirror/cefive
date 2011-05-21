/* cefiveui.h
 *  The User Interface for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef _CEFIVEUI_H
#define _CEFIVEUI_H

#include <pspctrl.h>
#include "cheateditor.h"
#include "cheatpanel.h"
#include "disassembler.h"
#include "colorconfig.h"
#include "cheatengine.h"
#include "appletmenu.h"
#include "buttonstate.h"
#include "hexeditor.h"
#include "gameinfo.h"
#include "optionspanel.h"
#include "cefiveconfig.h"
#include "searchpanel.h"
#include "searchengine.h"
#include "geelog.h"
#include "gdasm.h"

/** Indicates success. */
#define CEFIVEUI_SUCCESS        (0)

/** Indicates failure. */
#define CEFIVEUI_FAILURE        (-1)

/** Indicates memory error. */
#define CEFIVEUI_MEMORY         (-2)

/** Indicates a NULL pointer. */
#define CEFIVEUI_NULLPTR        (-3)

/** Maximum path string length */
#define CEFIVEUI_PATH_MAX   256

/** Length of the SCE Id for a Game. */
#define CEFIVEUI_SCEID_LEN  10

#define CEFIVEUI_E_CIRCLE       (0x00000001)
#define CEFIVEUI_E_CROSS        (0x00000002)
#define CEFIVEUI_E_SQUARE       (0x00000004)
#define CEFIVEUI_E_TRIANGLE     (0x00000008)
#define CEFIVEUI_E_LTRIGGER     (0x00000010)
#define CEFIVEUI_E_RTRIGGER     (0x00000020)
#define CEFIVEUI_E_DOWN         (0x00000100)
#define CEFIVEUI_E_LEFT         (0x00000200)
#define CEFIVEUI_E_RIGHT        (0x00000400)
#define CEFIVEUI_E_UP           (0x00000800)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ECEApplet {
        CEA_CheatPanel, /** Cheat Panel Applet */
        CEA_CheatEditor, /** Cheat Editor Applet */
        CEA_Disassembler, /** Disassembler Applet */
        CEA_HexEditor, /** Hex Editor Applet */
        CEA_SearchPanel, /** Search Panel Applet */
        CEA_Options, /** Options Panel Applet */
        CEA_GameInfo /** Game Information Applet */
    } 
    /** Enumeration specifying an Applet Id. */
    ECEApplet;

    typedef struct _CEFiveUiConfig {
        /** Panel Color Configuration struct */
        ColorConfig color;
        /** Title Bar Color Configuration struct */
        ColorConfig titlebar;
        /** Status Bar Color Configuration struct */
        ColorConfig status;
        /** Cursor Color Configuration struct */
        ColorConfig cursor;
        /** Edit Cursor Color Configuration struct */
        ColorConfig editcursor;
    } 
    /** User Interface Configuration struct */
    CEFiveUiConfig;

    typedef struct _CEFiveUi {
        /** User Interface Configuration struct */
        CEFiveUiConfig config;
        /** Pointer to the cefive Configuration struct. */
        CEFiveConfig* prCEConfig;
        /** Pointer to the Logger instance. */
        GeeLog* prLog;
        /** Pointer to a SearchEngine struct representing the Search Engine */
        SearchEngine* prSearchEngine;
        /** Pointer to a CheatEngine struct representing the Cheat Engine */
        CheatEngine* prEngine;
        /** Cheat Panel Applet struct */
        CheatPanel cheatpanel;
        /** Disassembler Applet struct */
        Disassembler disassembler;
        /** Cheat Editor Applet struct */
        CheatEditor cheateditor;
        /** Hex Editor Applet struct */
        HexEditor hexeditor;
        /** Search Panel Applet struct */
        SearchPanel searchpanel;
        /** Game Info Applet struct */
        GameInfo gameinfo;
        /** Options Panel Applet struct */
        OptionsPanel optionspanel;
        /** Applet Menu Applet struct */
        AppletMenu appletmenu;
        /** The currently active Applet Id. */
        int applet;
        /** Indicates that the splash screen has been viewed. */
        int splash_viewed;
        /** Path to write screenshot files to. */
        char screenshot_path[CEFIVEUI_PATH_MAX];
        /** Path to read PSId from. */
        char psid_file[CEFIVEUI_PATH_MAX];
        /** Path to read Game Id from. */
        char game_id_path[CEFIVEUI_PATH_MAX];
        /** SCE Game Id */
        char game_id[CEFIVEUI_SCEID_LEN + 1];
        /** Current controller state struct */
        SceCtrlData controlData;
        /** Indicates that the UI has been drawn. */
        int drawn;
        /** VRAM pointer */
        void* vram;
        /** Indicates that the UI is running. */
        int running;
        /** I forgot */
        int cross;
        /** Current Button State struct */
        ButtonState buttons;
        Gdasm rDasm;
    } 
    /** The Cheat Engine of Five User Interface struct. */
    CEFiveUi;

    /** Handle user input for buttons.
     * 
     * @param curr int containing current button state.
     * @param last int containing last button state.
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     */
    void cefiveui_buttoncallback(int curr, int last, CEFiveUi* prUi);
    
    /** Return a pointer to a CEFiveConfig struct representing the cefive
     * Configuration.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a CEFiveConfig struct or NULL is returned.
     */
    CEFiveConfig* cefiveui_get_config(CEFiveUi* prUi);

    /** Handle User Input for the User Interface.  This should be called once
     * per run-cycle to poll the PSP buttons and interact with the User
     * Interface.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveuiHandleInput(CEFiveUi *prUi);

    /** Initialize a cefive User Interface.
     * 
     * @param prUi Pointer to the CEFiveUi struct to initialize.
     * @param prEngine Pointer to a CheatEngine struct representing the Cheat
     * Engine.
     * @param prSearch Pointer to a SearchEngine struct representing the Search
     * Engine.
     */
    void cefiveuiInit(CEFiveUi *prUi, CheatEngine *prEngine,
            SearchEngine* prSearch);

    /** Draw the User Interface on the debug screen.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     */
    void cefiveuiRedraw(CEFiveUi *prUi);

    /** Add a Log statement to the configured Logger of a User Interface.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @param level ELogLevel indicating the Log Level of the Message.
     * @param sMsg string message to log.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveui_log(CEFiveUi* prUi, ELogLevel level, const char* sMsg);

    /** Assign a Logger to a User Interface.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @param prLog Pointer to a GeeLog struct representing the Logger.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveui_set_logger(CEFiveUi* prUi, GeeLog *prLog);

    /** Update the current control state of a User Interface.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveui_update_controls(CEFiveUi* prUi);

#ifdef	__cplusplus
}
#endif

#endif /* _CEFIVEUI_H */

