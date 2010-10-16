/* 
 * File:   appletmenu.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 9:39 AM
 */

#ifndef _APPLETMENU_H
#define	_APPLETMENU_H

#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "appletconfig.h"

typedef struct _AppletMenuConfig {
    ColorConfig color;
    ColorConfig cursor;
    Dimension size;
    CursorPos top;
}AppletMenuConfig;

typedef struct _AppletMenu {
    AppletMenuConfig config;
    AppletConfig* prApCfg;
    CursorPos cursor;
    int visible;
}AppletMenu;

void appletmenuCrossButton(AppletMenu *prMenu);
void appletmenuCursorDown(AppletMenu *prMenu);
void appletmenuCursorUp(AppletMenu *prMenu);
int appletmenuGetSelectedIndex(AppletMenu *prMenu);
void appletmenuRedraw(AppletMenu *prMenu);

#endif	/* _APPLETMENU_H */

