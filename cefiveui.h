/* cefiveui.h
 *  The User Interface for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef CEFIVEUI_H
#define CEFIVEUI_H

#include <stdio.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspkerneltypes.h>

#include "appletmenu.h"
#include "buttonstate.h"
#include "cefiveconfig.h"
#include "cheateditor.h"
#include "cheatengine.h"
#include "cheatpanel.h"
#include "colorconfig.h"
#include "disassembler.h"
#include "gameinfo.h"
#include "geelog.h"
#include "ggame.h"
#include "hexeditor.h"
#include "optionspanel.h"
#include "searchengine.h"
#include "searchpanel.h"

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
        CEA_CheatPanel = 0, /** Cheat Panel Applet */
        CEA_CheatEditor = 1, /** Cheat Editor Applet */
        CEA_Disassembler = 2, /** Disassembler Applet */
        CEA_HexEditor = 3, /** Hex Editor Applet */
        CEA_SearchPanel = 4, /** Search Panel Applet */
        CEA_GameInfo = 5, /** Game Information Applet */
        CEA_Options = 6 /** Options Panel Applet */
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
        /** Game struct */
        GGame game;
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
    
    /** Return a pointer to an AppletConfig struct representing the cefive
     * Applet Configuration.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* cefiveui_get_appletconfig(CEFiveUi* prUi);
    
    /** Return a pointer to an AppletMenu struct representing the Applet Menu.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to an AppletMenu struct or NULL is returned.
     */
    AppletMenu* cefiveui_get_appletmenu(CEFiveUi* prUi);
    
    /** Return a pointer to a CheatEditor struct representing the cefive Cheat
     * Editor Applet.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a CheatEditor struct or NULL is returned.
     */
    CheatEditor* cefiveui_get_cheateditor(CEFiveUi* prUi);
    
    /** Return a pointer to a CheatEngine struct representing the Cheat Engine.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a CheatEngine struct or NULL is returned.
     */
    CheatEngine* cefiveui_get_cheatengine(CEFiveUi* prUi);
    
    /** Return a pointer to a CheatPanel struct representing the cefive Cheat
     * Panel Applet.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a CheatPanel struct or NULL is returned.
     */
    CheatPanel* cefiveui_get_cheatpanel(CEFiveUi* prUi);
    
    /** Return a pointer to a CEFiveConfig struct representing the cefive
     * Configuration.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a CEFiveConfig struct or NULL is returned.
     */
    CEFiveConfig* cefiveui_get_config(CEFiveUi* prUi);

    /** Return a pointer to a HexEditor struct representing the Hex Editor 
     * Applet.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a HexEditor struct or NULL is returned.
     */
    HexEditor* cefiveui_get_hexeditor(CEFiveUi* prUi);
    
    /** Return a pointer to a CEFiveUiConfig struct representing the cefive
     * User Interface Configuration.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return A pointer to a CEFiveUiConfig struct or NULL is returned.
     */
    CEFiveUiConfig* cefiveui_get_uiconfig(CEFiveUi* prUi);
    
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

    /** Update the current control state of a User Interface.
     * 
     * @param prUi Pointer to a CEFiveUi struct representing the User Interface.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int cefiveui_update_controls(CEFiveUi* prUi);

#ifdef	__cplusplus
}
#endif

#endif /* CEFIVEUI_H */

