#include "optionspanel.h"

static int commit_edit(OptionsPanel* prPanel);
static int cursor_down(OptionsPanel* prPanel);
static int cursor_left(OptionsPanel* prPanel);
static int cursor_right(OptionsPanel* prPanel);
static int cursor_up(OptionsPanel* prPanel);
static int edit_value(OptionsPanel* prPanel);
static AppletConfig* get_appletconfig(OptionsPanel* prPanel);
static CEFiveConfig* get_cefiveconfig(OptionsPanel* prPanel);
static ColorConfig* get_cursorcolor(OptionsPanel* prPanel);
static CursorPos* get_cursorpos(OptionsPanel* prPanel);
static ColorConfig* get_editcolor(OptionsPanel* prPanel);
static HexPad* get_hexpad(OptionsPanel* prPanel);
static ColorConfig* get_panelcolor(OptionsPanel* prPanel);
static PanelConfig* get_panelconfig(OptionsPanel* prPanel);
static CursorPos* get_position(OptionsPanel* prPanel);
static ColorConfig* get_statuscolor(OptionsPanel* prPanel);
static ColorConfig* get_titlecolor(OptionsPanel* prPanel);
static int render_cursorcolor_row(OptionsPanel* prPanel);
static int render_editcolor_row(OptionsPanel* prPanel);
static int render_panelcolor_row(OptionsPanel* prPanel);
static int render_pause_row(OptionsPanel* prPanel);
static int render_row(OptionsPanel* prPanel, const int row);
static int render_statuscolor_row(OptionsPanel* prPanel);
static int render_titlecolor_row(OptionsPanel* prPanel);

static int commit_edit(OptionsPanel* prPanel) {
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    SceUInt32 value = 0;
    ColorConfig* prColor = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->cancelled == 1) {
        if (hexpad_reset(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        prPanel->editing = 0;
        prPanel->dirty = 1;
        return OPTIONSPANEL_SUCCESS;
    }
    value = hexpad_get_value(prPad);
    prCursor = get_cursorpos(prPanel);
    switch (prCursor->y) {
        case 1:
            prColor = get_panelcolor(prPanel);
            break;
        case 2:
            prColor = get_cursorcolor(prPanel);
            break;
        case 3:
            prColor = get_editcolor(prPanel);
            break;
        case 4:
            prColor = get_titlecolor(prPanel);
            break;
        case 5:
            prColor = get_statuscolor(prPanel);
            break;
    }
    if (prColor != NULL) {
        if (prCursor->x == 0) {
            prColor->background = value;
        } else {
            prColor->text = value;
        }
    }
    if (hexpad_reset(prPad) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    prPanel->editing = 0;
    prPanel->dirty = 1;
    return OPTIONSPANEL_SUCCESS;
}

static int cursor_down(OptionsPanel* prPanel) {
    CursorPos* prCursor = NULL;
    int y = 0;
    int oldy = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prCursor = get_cursorpos(prPanel);
    y = prCursor->y + 1;
    if (y > 5) {
        y = 0;
    }
    if (y != prCursor->y) {
        prCursor->x = 0;
    }
    oldy = prCursor->y;
    prCursor->y = y;
    if (oldy != y) {
        if (render_row(prPanel, oldy) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        if (render_row(prPanel, prCursor->y) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

static int cursor_left(OptionsPanel* prPanel) {
    CursorPos* prCursor = NULL;
    int x = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prCursor = get_cursorpos(prPanel);
    if (prCursor->y > 0) {
        x = prCursor->x - 1;
        if (x < 0) {
            x = 1;
        }
        if (x != prCursor->x) {
            prCursor->x = x;
            if (render_row(prPanel, prCursor->y) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

static int cursor_right(OptionsPanel* prPanel) {
    CursorPos* prCursor = NULL;
    int x = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prCursor = get_cursorpos(prPanel);
    if (prCursor->y > 0) {
        x = prCursor->x + 1;
        if (x > 1) {
            x = 0;
        }
        if (x != prCursor->x) {
            prCursor->x = x;
            if (render_row(prPanel, prCursor->y) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

static int cursor_up(OptionsPanel* prPanel) {
    CursorPos* prCursor = NULL;
    int y = 0;
    int oldy = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prCursor = get_cursorpos(prPanel);
    y = prCursor->y - 1;
    if (y < 0) {
        y = 5;
    }
    if (y != prCursor->y) {
        prCursor->x = 0;
    }
    oldy = prCursor->y;
    prCursor->y = y;
    if (oldy != prCursor->y) {
        if (render_row(prPanel, oldy) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        if (render_row(prPanel, y) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

static int edit_value(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    HexPad* prPad = NULL;
    ColorConfig* prColor = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    prCursor = get_cursorpos(prPanel);
    prConfig = get_cefiveconfig(prPanel);
    switch (prCursor->y) {
        case 0: /* Pause Row*/
            if (prConfig->pause_during_ui == 0) {
                prConfig->pause_during_ui = 1;
            } else {
                prConfig->pause_during_ui = 0;
            }
            prPanel->dirty = 1;
            prPanel->editing = 0;
            break;
        case 1: /* Panel Color Row */
            prColor = get_panelcolor(prPanel);
            break;
        case 2: /* Cursor Color Row */
            prColor = get_cursorcolor(prPanel);
            break;
        case 3: /* Edit Color Row */
            prColor = get_editcolor(prPanel);
            break;
        case 4: /* Title Color Row */
            prColor = get_titlecolor(prPanel);
            break;
        case 5: /* Status Color Row */
            prColor = get_statuscolor(prPanel);
            break;
    }
    if (prColor != NULL) {
        if (prCursor->x == 0) {
            if (hexpad_set_value(prPad, prColor->background) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
        } else {
            if (hexpad_set_value(prPad, prColor->text) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
        }
        prPanel->editing = 1;
        prPad->visible = 1;
        prPad->cancelled = 0;
    }
    
    return OPTIONSPANEL_SUCCESS;
}

static AppletConfig* get_appletconfig(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_cefiveconfig(prPanel);
    return cefiveconfig_get_appletconfig(prConfig);
}

static CEFiveConfig* get_cefiveconfig(OptionsPanel* prPanel) {
    if (prPanel == NULL) {
        return NULL;
    }
    return prPanel->prCEConfig;
}

static ColorConfig* get_cursorcolor(OptionsPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_panelconfig(prPanel);
    if (prConfig != NULL) {
        return panelconfig_get_cursorcolor(prConfig);
    }
    return NULL;
}

static CursorPos* get_cursorpos(OptionsPanel* prPanel) {
    if (prPanel == NULL) {
        return NULL;
    }
    return &prPanel->cursor;
}

static ColorConfig* get_editcolor(OptionsPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_panelconfig(prPanel);
    if (prConfig != NULL) {
        return panelconfig_get_editcolor(prConfig);
    }
    return NULL;
}

static HexPad* get_hexpad(OptionsPanel* prPanel) {
    if (prPanel == NULL) {
        return NULL;
    }
    return &prPanel->hexPad;
}

static ColorConfig* get_panelcolor(OptionsPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_panelconfig(prPanel);
    if (prConfig != NULL) {
        return panelconfig_get_panelcolor(prConfig);
    }
    return NULL;
}

static PanelConfig* get_panelconfig(OptionsPanel* prPanel) {
    AppletConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_appletconfig(prPanel);
    if (prConfig != NULL) {
        return appletconfig_get_panelconfig(prConfig);
    }
    return NULL;
}

static CursorPos* get_position(OptionsPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = get_panelconfig(prPanel);
        if (prConfig != NULL) {
            return panelconfig_get_position(prConfig);
        }
    }
    return NULL;
}

static Dimension* get_size(OptionsPanel* prPanel) {
    PanelConfig* prConfig = NULL;
    if (prPanel != NULL) {
        prConfig = get_panelconfig(prPanel);
        if (prConfig != NULL) {
            return panelconfig_get_size(prConfig);
        }
    }
    return NULL;
}

static ColorConfig* get_statuscolor(OptionsPanel* prPanel) {
    AppletConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_appletconfig(prPanel);
    if (prConfig != NULL) {
        return appletconfig_get_statuscolor(prConfig);
    }
    return NULL;
}

static ColorConfig* get_titlecolor(OptionsPanel* prPanel) {
    AppletConfig* prConfig = NULL;
    if (prPanel == NULL) {
        return NULL;
    }
    prConfig = get_appletconfig(prPanel);
    if (prConfig != NULL) {
        return appletconfig_get_titlecolor(prConfig);
    }
    return NULL;
}

int optionspanel_button_circle(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_circle(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_button_cross(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_cross(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        return OPTIONSPANEL_SUCCESS;
    }
    if (edit_value(prPanel) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_button_ltrigger(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_prev_digit(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_button_rtrigger(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_next_digit(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_button_square(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_square(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_button_triangle(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_button_triangle(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_down(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_down(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        return OPTIONSPANEL_SUCCESS;
    }
    if (cursor_down(prPanel) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_left(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_left(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        return OPTIONSPANEL_SUCCESS;
    }
    if (cursor_left(prPanel) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_right(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_right(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        return OPTIONSPANEL_SUCCESS;
    }
    if (cursor_right(prPanel) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_up(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPad = get_hexpad(prPanel);
    if (prPad->visible == 1) {
        if (hexpad_cursor_up(prPad) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
        return OPTIONSPANEL_SUCCESS;
    }
    if (cursor_up(prPanel) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_init(OptionsPanel* prPanel, CEFiveConfig* prConfig) {
    HexPad* prPad = NULL;
    ColorConfig* prColor = NULL;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prPanel->prCEConfig = prConfig;
    prPanel->editing = 0;
    prPad = get_hexpad(prPanel);
    if (hexpad_init(prPad) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    prColor = get_panelcolor(prPanel);
    if (prColor != NULL) {
        if (hexpad_set_panelcolor(prPad, 
                prColor->background, prColor->text) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    prColor = get_cursorcolor(prPanel);
    if (prColor != NULL) {
        if (hexpad_set_cursorcolor(prPad,
                prColor->background, prColor->text) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    if (hexpad_set_position(prPad, 4, 4) < 0) {
        return OPTIONSPANEL_FAILURE;
    }
    prPanel->dirty = 1;
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_redraw(OptionsPanel* prPanel) {
    HexPad* prPad = NULL;
    ColorConfig* prColor = NULL;
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    char sFmt[10];
    int i = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    if (prPanel->editing == 1) {
        prPad = get_hexpad(prPanel);
        if (prPad->visible == 1) {
            if (hexpad_redraw(prPad) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            return OPTIONSPANEL_SUCCESS;
        }
        if (commit_edit(prPanel) < 1) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    
    if (prPanel->dirty == 0) {
        return OPTIONSPANEL_SUCCESS;
    }
    prColor = get_panelcolor(prPanel);
    prPos = get_position(prPanel);
    prSize = get_size(prPanel);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 1);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    sprintf(sFmt, "%%-%ds", prSize->width);
    pspDebugScreenKprintf(sFmt, "Options");

    for (i = 0; i < prSize->height - 2; i++) {
        if (render_row(prPanel, i) < 0) {
            return OPTIONSPANEL_FAILURE;
        }
    }
    prPanel->dirty = 0;
    return OPTIONSPANEL_SUCCESS;
}

static int render_cursorcolor_row(OptionsPanel* prPanel) {
    CursorPos* prPos = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    Dimension* prSize = NULL;
    int cr = 0;
    char sFmt[10];
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    prPos = get_position(prPanel);
    prCursor = get_cursorpos(prPanel);
    prPcolor = get_panelcolor(prPanel);
    prCcolor = get_cursorcolor(prPanel);
    prSize = get_size(prPanel);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 4);
    if (prCcolor != NULL) {
        pspDebugScreenSetBackColor(prCcolor->background);
        pspDebugScreenSetTextColor(prCcolor->text);
    }
    pspDebugScreenPuts(OPTIONSPANEL_L_CURSORC);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    if (prCursor->y == 2) {
        if (prCursor->x == 0) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prCcolor->background);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(" ");
    if (prCursor->y == 2) {
        if (prCursor->x == 1) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prCcolor->text);
    cr = prSize->width - strlen(OPTIONSPANEL_L_CURSORC) - 22;
    sprintf(sFmt, "%%-%ds", cr);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenKprintf(sFmt, "");
    return OPTIONSPANEL_SUCCESS;
}

static int render_editcolor_row(OptionsPanel* prPanel) {
    CursorPos* prPos = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    ColorConfig* prColor = NULL;
    Dimension* prSize = NULL;
    int cr = 0;
    char sFmt[10];

    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    prPos = get_position(prPanel);
    prCursor = get_cursorpos(prPanel);
    prPcolor = get_panelcolor(prPanel);
    prCcolor = get_cursorcolor(prPanel);
    prColor = get_editcolor(prPanel);
    prSize = get_size(prPanel);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 5);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenPuts(OPTIONSPANEL_L_EDITC);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    if (prCursor->y == 3) {
        if (prCursor->x == 0) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prColor->background);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(" ");
    if (prCursor->y == 3) {
        if (prCursor->x == 1) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prColor->text);
    cr = prSize->width - strlen(OPTIONSPANEL_L_EDITC) - 22;
    sprintf(sFmt, "%%-%ds", cr);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenKprintf(sFmt, "");
    return OPTIONSPANEL_SUCCESS;
}

static int render_panelcolor_row(OptionsPanel* prPanel) {
    CursorPos* prPos = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    Dimension* prSize = NULL;
    int cr = 0;
    char sFmt[10];
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    prPos = get_position(prPanel);
    prCursor = get_cursorpos(prPanel);
    prPcolor = get_panelcolor(prPanel);
    prCcolor = get_cursorcolor(prPanel);
    prSize = get_size(prPanel);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 3);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(OPTIONSPANEL_L_PANELC);
    if (prCursor->y == 1) {
        if (prCursor->x == 0) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prPcolor->background);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(" ");
    if (prCursor->y == 1) {
        if (prCursor->x == 1) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prPcolor->text);
    cr = prSize->width - strlen(OPTIONSPANEL_L_PANELC) - 22;
    sprintf(sFmt, "%%-%ds", cr);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenKprintf(sFmt, "");
    return OPTIONSPANEL_SUCCESS;
}

static int render_pause_row(OptionsPanel* prPanel) {
    CursorPos* prPos = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    CEFiveConfig* prConfig = NULL;
    Dimension* prSize = NULL;
    const char* sLabel = "Pause Game during UI: ";
    const char* sYes = "Yes";
    const char* sNo = "No ";
    char sFmt[10];
    
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    prPos = get_position(prPanel);
    pspDebugScreenSetXY(prPos->x, prPos->y + 2);
    prPcolor = get_panelcolor(prPanel);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(sLabel);
    prCcolor = get_cursorcolor(prPanel);
    prCursor = get_cursorpos(prPanel);
    if (prCursor->y == 0) {
        if (prCcolor != NULL) {
            pspDebugScreenSetBackColor(prCcolor->background);
            pspDebugScreenSetTextColor(prCcolor->text);
        }
    }
    prConfig = get_cefiveconfig(prPanel);
    if (prConfig->pause_during_ui == 1) {
        pspDebugScreenPuts(sYes);
    } else {
        pspDebugScreenPuts(sNo);
    }
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    prSize = get_size(prPanel);
    sprintf(sFmt, "%%-%ds", prSize->width - strlen(sLabel) - strlen(sYes));
    pspDebugScreenKprintf(sFmt, "");
    return OPTIONSPANEL_SUCCESS;
}

static int render_row(OptionsPanel* prPanel, const int row) {
    CursorPos* prPos = NULL;
    Dimension* prSize = NULL;
    ColorConfig* prColor = NULL;
    char sFmt[10];
    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    switch (row) {
        case 0: /* Pause Row */
            if (render_pause_row(prPanel) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            break;
        case 1: /* Panel Color Row */
            if (render_panelcolor_row(prPanel) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            break;
        case 2: /* Cursor Color Row */
            if (render_cursorcolor_row(prPanel) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            break;
        case 3: /* Edit Color Row */
            if (render_editcolor_row(prPanel) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            break;
        case 4: /* Title Color Row */
            if (render_titlecolor_row(prPanel) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            break;
        case 5: /* Status Color Row */
            if (render_statuscolor_row(prPanel) < 0) {
                return OPTIONSPANEL_FAILURE;
            }
            break;
        default:
            prPos = get_position(prPanel);
            prSize = get_size(prPanel);
            prColor = get_panelcolor(prPanel);
            sprintf(sFmt, "%%-%ds", prSize->width - 1);
            pspDebugScreenSetXY(prPos->x, prPos->y + 2 + row);
            pspDebugScreenSetBackColor(prColor->background);
            pspDebugScreenSetTextColor(prColor->text);
            pspDebugScreenKprintf(sFmt, "");
    }
    return OPTIONSPANEL_SUCCESS;
}

static int render_statuscolor_row(OptionsPanel* prPanel) {
    CursorPos* prPos = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    ColorConfig* prColor = NULL;
    Dimension* prSize = NULL;
    int cr = 0;
    char sFmt[10];

    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    prPos = get_position(prPanel);
    prCursor = get_cursorpos(prPanel);
    prPcolor = get_panelcolor(prPanel);
    prCcolor = get_cursorcolor(prPanel);
    prColor = get_statuscolor(prPanel);
    prSize = get_size(prPanel);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 7);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenPuts(OPTIONSPANEL_L_STATUSC);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    if (prCursor->y == 5) {
        if (prCursor->x == 0) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prColor->background);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(" ");
    if (prCursor->y == 5) {
        if (prCursor->x == 1) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prColor->text);
    cr = prSize->width - strlen(OPTIONSPANEL_L_EDITC) - 22;
    sprintf(sFmt, "%%-%ds", cr);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenKprintf(sFmt, "");
    return OPTIONSPANEL_SUCCESS;
}

static int render_titlecolor_row(OptionsPanel* prPanel) {
    CursorPos* prPos = NULL;
    CursorPos* prCursor = NULL;
    ColorConfig* prPcolor = NULL;
    ColorConfig* prCcolor = NULL;
    ColorConfig* prColor = NULL;
    Dimension* prSize = NULL;
    int cr = 0;
    char sFmt[10];

    if (prPanel == NULL) {
        return OPTIONSPANEL_NULLPTR;
    }
    
    prPos = get_position(prPanel);
    prCursor = get_cursorpos(prPanel);
    prPcolor = get_panelcolor(prPanel);
    prCcolor = get_cursorcolor(prPanel);
    prColor = get_titlecolor(prPanel);
    prSize = get_size(prPanel);
    
    pspDebugScreenSetXY(prPos->x, prPos->y + 6);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenPuts(OPTIONSPANEL_L_TITLEC);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    if (prCursor->y == 4) {
        if (prCursor->x == 0) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prColor->background);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenPuts(" ");
    if (prCursor->y == 4) {
        if (prCursor->x == 1) {
            if (prCcolor != NULL) {
                pspDebugScreenSetBackColor(prCcolor->background);
                pspDebugScreenSetTextColor(prCcolor->text);
            }
        }
    }
    pspDebugScreenKprintf("0x%08X", prColor->text);
    cr = prSize->width - strlen(OPTIONSPANEL_L_TITLEC) - 22;
    sprintf(sFmt, "%%-%ds", cr);
    if (prPcolor != NULL) {
        pspDebugScreenSetBackColor(prPcolor->background);
        pspDebugScreenSetTextColor(prPcolor->text);
    }
    pspDebugScreenKprintf(sFmt, "");
    return OPTIONSPANEL_SUCCESS;
}
