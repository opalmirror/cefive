/* 
 * File:   optionspanel.h
 * Author: Sir Gee of Five
 *
 * Created on September 26, 2010, 10:49 AM
 */

#ifndef _OPTIONSPANEL_H
#define	_OPTIONSPANEL_H

#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "cefiveconfig.h"
#include "textcolumn.h"
#include "togglecolumn.h"
#include "dwordcolumn.h"

#define OPTIONSPANEL_SUCCESS    (0)
#define OPTIONSPANEL_FAILURE    (-1)
#define OPTIONSPANEL_MEMORY     (-2)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _ColorRow {
        TextColumn rLabel;
        DwordColumn rBackground;
        DwordColumn rText;
    } ColorRow;

    typedef struct _GamePauseRow {
        TextColumn rLabel;
        ToggleColumn rToggle;
    } GamePauseRow;

    typedef struct _OptionsPanelConfig {
        ColorConfig color;
        ColorConfig cursor;
        ColorConfig edit;
        CursorPos position;
    } OptionsPanelConfig;

    typedef struct _OptionsPanel {
        OptionsPanelConfig config;
        CursorPos cursor;
        Dimension size;
        CEFiveConfig* prCEConfig;
        GamePauseRow rPauseRow;
        int editing;
        DwordColumn rColorCol;
    } OptionsPanel;

    int optionspanel_cross_button(OptionsPanel* prPanel);
    int optionspanel_cursor_down(OptionsPanel* prPanel);
    int optionspanel_cursor_left(OptionsPanel* prPanel);
    int optionspanel_cursor_right(OptionsPanel* prPanel);
    int optionspanel_cursor_up(OptionsPanel* prPanel);
    int optionspanel_dpad_down(OptionsPanel* prPanel);
    int optionspanel_dpad_left(OptionsPanel* prPanel);
    int optionspanel_dpad_right(OptionsPanel* prPanel);
    int optionspanel_dpad_up(OptionsPanel* prPanel);
    int optionspanel_init(OptionsPanel* prPanel, CEFiveConfig* prConfig);
    int optionspanel_redraw(OptionsPanel* prPanel);

#ifdef	__cplusplus
}
#endif

#endif	/* _OPTIONSPANEL_H */

