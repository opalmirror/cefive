#include <pspdebug.h>
#include <pspkerneltypes.h>
#include "appletconfig.h"
#include "colorconfig.h"
#include "appletmenu.h"

void appletmenuCrossButton(AppletMenu *prMenu) {
    if (prMenu == NULL) {
        return;
    }
    if (prMenu->visible == 0) {
        return;
    }
    prMenu->visible = 0;
}

void appletmenuCursorDown(AppletMenu *prMenu) {
    int y;
    if (prMenu != NULL) {
        y = prMenu->cursor.y;
        y++;
        if (y > 5) {
            y = 5;
        }
        prMenu->cursor.y = y;
    }
}

void appletmenuCursorUp(AppletMenu *prMenu) {
    int y;
    if (prMenu != NULL) {
        y = prMenu->cursor.y;
        y--;
        if (y < 0) {
            y = 0;
        }
        prMenu->cursor.y = y;
    }
}

int appletmenuGetSelectedIndex(AppletMenu *prMenu) {
    int index = 0;
    if (prMenu != NULL) {
        index = prMenu->cursor.y;
    }
    return index;
}

void appletmenuRedraw(AppletMenu *prMenu) {
    AppletConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prCursor = NULL;
    u32 bg;
    u32 fg;
    int top;
    int left;
    int sel;

    if (prMenu == NULL) {
        return;
    }
    prCfg = prMenu->prApCfg;

    top = prMenu->config.top.y;
    left = prMenu->config.top.x;
    sel = prMenu->cursor.y;

    if (prCfg != NULL) {
        prColor = &prCfg->rStatus;
        prCursor = &prCfg->rPanel.rCursor;
    } else {
        prColor = &prMenu->config.color;
        prCursor = &prMenu->config.cursor;
    }
    bg = prColor->background;
    fg = prColor->text;
    if (sel == 0) {
        bg = prCursor->background;
        fg = prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenSetXY(left, top);
    pspDebugScreenKprintf("%-20s", "Cheat Panel");

    bg = prColor->background;
    fg = prColor->text;
    if (sel == 1) {
        bg = prCursor->background;
        fg = prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenSetXY(left, top + 1);
    pspDebugScreenKprintf("%-20s", "Disassembler");

    bg = prColor->background;
    fg = prColor->text;
    if (sel == 2) {
        bg = prCursor->background;
        fg = prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenSetXY(left, top + 2);
    pspDebugScreenKprintf("%-20s", "Hex Editor");

    bg = prColor->background;
    fg = prColor->text;
    if (sel == 3) {
        bg = prCursor->background;
        fg = prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenSetXY(left, top + 3);
    pspDebugScreenKprintf("%-20s", "Search Panel");

    bg = prColor->background;
    fg = prColor->text;
    if (sel == 4) {
        bg = prCursor->background;
        fg = prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenSetXY(left, top + 4);
    pspDebugScreenKprintf("%-20s", "Game Info");

    bg = prColor->background;
    fg = prColor->text;
    if (sel == 5) {
        bg = prCursor->background;
        fg = prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenSetXY(left, top + 5);
    pspDebugScreenKprintf("%-20s", "Options");

}
