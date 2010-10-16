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

#define CEFIVEUI_PATH_MAX   256
#define CEFIVEUI_SCEID_LEN  10

typedef enum _ECEApplet {
    CEA_CheatPanel,
            CEA_CheatEditor,
            CEA_Disassembler,
            CEA_HexEditor,
            CEA_SearchPanel,
            CEA_Options
}ECEApplet;

typedef struct _CEFiveUiConfig {
    ColorConfig color;
    ColorConfig titlebar;
    ColorConfig status;
    ColorConfig cursor;
    ColorConfig editcursor;
} CEFiveUiConfig;

typedef struct _CEFiveUi {
    CEFiveUiConfig  config;
    CEFiveConfig*   prCEConfig;
    SearchEngine*   prSearchEngine;
    CheatEngine*    prEngine;
    CheatPanel      cheatpanel;
    Disassembler    disassembler;
    CheatEditor     cheateditor;
    HexEditor       hexeditor;
    SearchPanel     searchpanel;
    GameInfo        gameinfo;
    OptionsPanel    optionspanel;
    AppletMenu      appletmenu;
    int             applet;
    int             splash_viewed;
    char            screenshot_path[CEFIVEUI_PATH_MAX];
    char            psid_file[CEFIVEUI_PATH_MAX];
    char            game_id_path[CEFIVEUI_PATH_MAX];
    char            game_id[CEFIVEUI_SCEID_LEN + 1];
    SceCtrlData     controlData;
    int             drawn;
    void*           vram;
    int             running;
    int             cross;
    ButtonState     buttons;
} CEFiveUi;

void cefiveui_buttoncallback(int curr, int last, CEFiveUi* prUi);
CEFiveConfig* cefiveui_get_config(CEFiveUi* prUi);

/* cefiveuiHandleInput
 *  Handle User Input for the Cheat Engine of Five User Interface.  This
 *  should be called once per run-cycle to poll the PSP buttons and interact
 *  with the User Interface.
 * Parameters:
 *  prUi(in) - Pointer to the CEFiveUi struct to handle input for.
 */
int cefiveuiHandleInput(CEFiveUi *prUi);

/* cefiveuiInit
 *  Initialize a CEFiveUi struct.
 * Parameters:
 *  prUi(in/out) - Pointer to the CEFiveUi struct to initialize.
 *  prEngine(in) - Pointer to a CheatEngine struct.
 *  prSearch(in) - Pointer to a SearchEngine struct.
 */
void cefiveuiInit(CEFiveUi *prUi, CheatEngine *prEngine,
        SearchEngine* prSearch);

/* cefiveuiRedraw
 *  Redraw the Cheat Engine of Five User Interface.
 * Parameters:
 *  prUi(in/out) - Pointer to the CEFiveUi struct to display.
 */
void cefiveuiRedraw(CEFiveUi *prUi);

#endif /* _CEFIVEUI_H */

