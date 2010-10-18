#include <pspkerneltypes.h>
#include <stdlib.h>
#include <string.h>
#include "appletconfig.h"
#include "panelconfig.h"
#include "colorconfig.h"
#include "dimension.h"
#include "cursorpos.h"
#include "cefiveconfig.h"

AppletConfig* cefiveconfig_get_appletconfig(CEFiveConfig* prConfig) {
    AppletConfig* prAp = NULL;
    if (prConfig != NULL) {
        prAp = &prConfig->rAppletConfig;
    }
    return prAp;
}

int cefiveconfig_get_cheatfilename(char* sDest, CEFiveConfig* prConfig) {
    char* sPlugins = NULL;
    char* sCefive = NULL;
    char* sGameId = NULL;
    if (sDest == NULL || prConfig == NULL) {
        return CEFIVECONFIG_NULLPTR;
    }
    sPlugins = prConfig->plugins_dir;
    sCefive = prConfig->cefive_dir;
    sGameId = prConfig->game_id;
    sprintf(sDest, "ms0:/%s/%s/%s.txt", sPlugins, sCefive, sGameId);
    return CEFIVECONFIG_SUCCESS;
}

ColorConfig* cefiveconfig_get_cursorcolor(CEFiveConfig* prConfig) {
    ColorConfig* prColor = NULL;
    PanelConfig* prPanel = cefiveconfig_get_panelconfig(prConfig);
    if (prPanel != NULL) {
        prColor = panelconfig_get_cursorcolor(prPanel);
    }
    return prColor;
}

ColorConfig* cefiveconfig_get_editcolor(CEFiveConfig* prConfig) {
    ColorConfig* prColor = NULL;
    PanelConfig* prPanel = cefiveconfig_get_panelconfig(prConfig);
    if (prPanel != NULL) {
        prColor = panelconfig_get_editcolor(prPanel);
    }
    return prColor;
}

ColorConfig* cefiveconfig_get_panelcolor(CEFiveConfig* prConfig) {
    ColorConfig* prColor = NULL;
    PanelConfig* prPanel = cefiveconfig_get_panelconfig(prConfig);
    if (prPanel != NULL) {
        prColor = panelconfig_get_panelcolor(prPanel);
    }
    return prColor;
}

PanelConfig* cefiveconfig_get_panelconfig(CEFiveConfig* prConfig) {
    PanelConfig* prPanel = NULL;
    AppletConfig* prApp = cefiveconfig_get_appletconfig(prConfig);
    if (prApp != NULL) {
        prPanel = appletconfig_get_panelconfig(prApp);
    }
    return prPanel;
}

ColorConfig* cefiveconfig_get_statuscolor(CEFiveConfig* prConfig) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = cefiveconfig_get_appletconfig(prConfig);
    if (prApp != NULL) {
        prColor = appletconfig_get_statuscolor(prApp);
    }
    return prColor;
}

ColorConfig* cefiveconfig_get_titlecolor(CEFiveConfig* prConfig) {
    ColorConfig* prColor = NULL;
    AppletConfig* prApp = cefiveconfig_get_appletconfig(prConfig);
    if (prApp != NULL) {
        prColor = appletconfig_get_titlecolor(prApp);
    }
    return prColor;
}

int cefiveconfig_init(CEFiveConfig* prConfig) {
    AppletConfig* prApp = NULL;
    PanelConfig* prPanel = NULL;
    ColorConfig* prColor = NULL;
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;

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
