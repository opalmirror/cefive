/* cheatpanel.h
 *  The Cheat Panel Applet for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef _CHEATPANEL_H
#define _CHEATPANEL_H

#include <pspkerneltypes.h>
#include "cheat.h"
#include "block.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "cheatengine.h"
#include "appletconfig.h"

typedef struct _CheatPanelConfig {
    ColorConfig color;
    ColorConfig cursorcolor;
    ColorConfig cursorrowcolor;
    ColorConfig selectedcolor;
    ColorConfig constantcolor;
    int table_height;
    int top_row;
} CheatPanelConfig;

typedef struct _CheatPanel {
    CheatPanelConfig config;
    CheatEngine* prEngine;
    AppletConfig* prApCfg;
    CursorPos cursor;
    int page_position;
} CheatPanel;

void cheatpanelCircleButton(CheatPanel *);
void cheatpanelCrossButton(CheatPanel *);
void cheatpanelDpadDown(CheatPanel *);
void cheatpanelDpadLeft(CheatPanel *);
void cheatpanelDpadRight(CheatPanel *);
void cheatpanelDpadUp(CheatPanel *);
int cheatpanelGetSelectedIndex(CheatPanel *);
void cheatpanelInit(CheatPanel *, CheatEngine*);
void cheatpanelRedraw(CheatPanel *);
void cheatpanelScrollDown(CheatPanel *);
void cheatpanelScrollUp(CheatPanel *);
void cheatpanelSquareButton(CheatPanel *);
void cheatpanelTriangleButton(CheatPanel *);

#endif /* _CHEATPANEL_H */
