#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "cefiveconfig.h"
#include "textcolumn.h"
#include "togglecolumn.h"
#include "colorconfig.h"
#include "optionspanel.h"

static int draw_basecolor_row(OptionsPanel* prPanel);
static int draw_cursor(OptionsPanel* prPanel);
static int draw_cursorcolor_row(OptionsPanel* prPanel);
static int draw_editcolor_row(OptionsPanel* prPanel);
static int draw_pause_row(OptionsPanel* prPanel);
static int draw_statuscolor_row(OptionsPanel* prPanel);
static int draw_titlecolor_row(OptionsPanel* prPanel);

int optionspanel_cross_button(OptionsPanel* prPanel) {
    ColorConfig* prColor = NULL;
    DwordColumn* prCol = NULL;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    int y = prPanel->cursor.y;
    int x = prPanel->cursor.x;
    if (y == 0) {
        CEFiveConfig* prConfig = prPanel->prCEConfig;
        GamePauseRow* prRow = &prPanel->rPauseRow;
        ToggleColumn* prToggle = &prRow->rToggle;
        togglecolumn_toggle(prToggle);
        prConfig->pause_during_ui = prToggle->value;
        return OPTIONSPANEL_SUCCESS;
    }
    prCol = &prPanel->rColorCol;
    if (y == 1) {
        prColor = &prPanel->prCEConfig->rAppletConfig.rPanel.rColor;
        if (x == 0) {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->background);
                prPanel->editing = 1;
            } else {
                prColor->background = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        } else {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->text);
                prPanel->editing = 1;
            } else {
                prColor->text = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        }
    }
    if (y == 2) {
        prColor = &prPanel->prCEConfig->rAppletConfig.rPanel.rCursor;
        if (x == 0) {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->background);
                prPanel->editing = 1;
            } else {
                prColor->background = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        } else {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->text);
                prPanel->editing = 1;
            } else {
                prColor->text = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        }
    }
    if (y == 3) {
        prColor = &prPanel->prCEConfig->rAppletConfig.rPanel.rEdit;
        if (x == 0) {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->background);
                prPanel->editing = 1;
            } else {
                prColor->background = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        } else {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->text);
                prPanel->editing = 1;
            } else {
                prColor->text = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        }
    }
    if (y == 4) {
        prColor = &prPanel->prCEConfig->rAppletConfig.rTitlebar;
        if (x == 0) {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->background);
                prPanel->editing = 1;
            } else {
                prColor->background = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        } else {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->text);
                prPanel->editing = 1;
            } else {
                prColor->text = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        }
    }
    if (y == 5) {
        prColor = &prPanel->prCEConfig->rAppletConfig.rStatus;
        if (x == 0) {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->background);
                prPanel->editing = 1;
            } else {
                prColor->background = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        } else {
            if (prPanel->editing == 0) {
                dwordcolumn_edit(prCol, prColor->text);
                prPanel->editing = 1;
            } else {
                prColor->text = dwordcolumn_commit(prCol);
                prPanel->editing = 0;
            }
        }
    }

    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_cursor_down(OptionsPanel* prPanel) {
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    int y = prPanel->cursor.y;
    y++;
    if (y > 5) {
        y = 5;
    }
    prPanel->cursor.y = y;
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_cursor_left(OptionsPanel* prPanel) {
    int x = 0;
    int y = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    x = prPanel->cursor.x;
    y = prPanel->cursor.y;
    if (y == 0) {
        return OPTIONSPANEL_SUCCESS;
    }
    x--;
    if (x < 0) {
        x = 0;
    }
    prPanel->cursor.x = x;
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_cursor_right(OptionsPanel* prPanel) {
    int x = 0;
    int y = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    x = prPanel->cursor.x;
    y = prPanel->cursor.y;
    if (y == 0) {
        return OPTIONSPANEL_SUCCESS;
    }
    x++;
    if (x > 1) {
        x = 1;
    }
    prPanel->cursor.x = x;
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_cursor_up(OptionsPanel* prPanel) {
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    int y = prPanel->cursor.y;
    y--;
    if (y < 0) {
        y = 0;
    }
    prPanel->cursor.y = y;
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_down(OptionsPanel* prPanel) {
    DwordColumn *prCol = NULL;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    if (prPanel->editing == 0) {
        optionspanel_cursor_down(prPanel);
    } else {
        if (prPanel->cursor.y != 0) {
            prCol = &prPanel->rColorCol;
            dwordcolumn_decrement(prCol);
        }
    }

    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_left(OptionsPanel* prPanel) {
    DwordColumn *prCol = NULL;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    if (prPanel->editing == 0) {
        optionspanel_cursor_left(prPanel);
    } else {
        if (prPanel->cursor.y != 0) {
            prCol = &prPanel->rColorCol;
            dwordcolumn_prevdigit(prCol);
        }
    }

    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_right(OptionsPanel* prPanel) {
    DwordColumn *prCol = NULL;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    if (prPanel->editing == 0) {
        optionspanel_cursor_right(prPanel);
    } else {
        if (prPanel->cursor.y != 0) {
            prCol = &prPanel->rColorCol;
            dwordcolumn_nextdigit(prCol);
        }
    }

    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_dpad_up(OptionsPanel* prPanel) {
    DwordColumn* prCol = NULL;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    if (prPanel->editing == 0) {
        optionspanel_cursor_up(prPanel);
    } else {
        if (prPanel->cursor.y != 0) {
            prCol = &prPanel->rColorCol;
            dwordcolumn_increment(prCol);
        }
    }

    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_init(OptionsPanel* prPanel, CEFiveConfig* prConfig) {
    DwordColumn* prCCol = NULL;
    ColorConfig* prSrc = NULL;
    ColorConfig* prDst = NULL;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prPanel->prCEConfig = prConfig;
    GamePauseRow* prRow = &prPanel->rPauseRow;
    ColorConfig* prColor = &(prPanel->config.color);
    ColorConfig* prCursor = &(prPanel->config.cursor);
    TextColumn* prLabel = &(prRow->rLabel);
    ToggleColumn* prToggle = &(prRow->rToggle);

    textcolumn_init(prLabel);
    prLabel->prColor = prColor;
    prLabel->prCursor = prCursor;
    prLabel->columns = 30;
    textcolumn_settext(prLabel, "Pause Game During UI");

    togglecolumn_init(prToggle);
    prToggle->prColor = prColor;
    prToggle->prCursor = prCursor;
    prToggle->columns = 30;

    prPanel->editing = 0;

    prCCol = &prPanel->rColorCol;
    dwordcolumn_init(prCCol);
    prCCol->prefix = 1;
    prSrc = &prPanel->prCEConfig->rAppletConfig.rPanel.rCursor;
    prDst = &prCCol->color;
    colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    prSrc = &prPanel->prCEConfig->rAppletConfig.rPanel.rEdit;
    prDst = &prCCol->edit;
    colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    
    return OPTIONSPANEL_SUCCESS;
}

int optionspanel_redraw(OptionsPanel* prPanel) {
    ColorConfig* prColor = NULL;
    int x = 0;
    int y = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    x = prPanel->config.position.x;
    y = prPanel->config.position.y;
    prColor = &prPanel->prCEConfig->rAppletConfig.rPanel.rColor;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(x, y);
    pspDebugScreenPuts("Options");

    draw_pause_row(prPanel);
    draw_basecolor_row(prPanel);
    draw_cursorcolor_row(prPanel);
    draw_editcolor_row(prPanel);
    draw_titlecolor_row(prPanel);
    draw_statuscolor_row(prPanel);
    draw_cursor(prPanel);
    
    return OPTIONSPANEL_SUCCESS;
}

static int draw_basecolor_row(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    ColorConfig* prColor = NULL;
    ColorRow rRow;
    ColorConfig* prSrc = NULL;
    ColorConfig* prDst = NULL;
    int x = 0;
    int y = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prConfig = prPanel->prCEConfig;
    if (prConfig == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prColor = &prConfig->rAppletConfig.rPanel.rColor;
    textcolumn_init(&rRow.rLabel);
    dwordcolumn_init(&rRow.rBackground);
    dwordcolumn_init(&rRow.rText);
    rRow.rLabel.prColor = prColor;
    rRow.rLabel.columns = 30;
    textcolumn_settext(&rRow.rLabel, "Base Color (bg/fg)");
    prSrc = &prConfig->rAppletConfig.rPanel.rColor;
    prDst = &rRow.rBackground.color;
    colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    prDst = &rRow.rText.color;
    colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    //rRow.rBackground.color.background = prPanel->config.color.background;
    //rRow.rBackground.color.text = prPanel->config.color.text;
    //rRow.rText.color.background = prPanel->config.color.background;
    //rRow.rText.color.text = prPanel->config.color.text;
    dwordcolumn_setvalue(&rRow.rBackground, prColor->background);
    dwordcolumn_setvalue(&rRow.rText, prColor->text);
    x = prPanel->config.position.x;
    y = prPanel->config.position.y + 2;
    pspDebugScreenSetXY(x, y);
    textcolumn_redraw(&rRow.rLabel);
    if (prPanel->cursor.y == 1) {
        prDst = &rRow.rBackground.color;
        if (prPanel->cursor.x == 0) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rBackground);
    pspDebugScreenSetXY(x + 41, y);
    if (prPanel->cursor.y == 1) {
        prDst = &rRow.rText.color;
        if (prPanel->cursor.x == 1) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rText);
    return OPTIONSPANEL_SUCCESS;
}

static int draw_cursor(OptionsPanel* prPanel) {
    DwordColumn* prCol = NULL;
    int x = 0;
    int y = 0;
    int sx = 0;
    int sy = 0;
    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    x = prPanel->cursor.x;
    y = prPanel->cursor.y;
    if (prPanel->editing == 0) {
        return OPTIONSPANEL_SUCCESS;
    }
    if (y == 0) {
        return OPTIONSPANEL_SUCCESS;
    }
    if (x == 0) {
        sx = 30;
    } else {
        sx = 41;
    }
    sy = prPanel->config.position.y + 1 + y;
    prCol = &prPanel->rColorCol;
    pspDebugScreenSetXY(sx, sy);
    dwordcolumn_redraw(prCol);
    
    return OPTIONSPANEL_SUCCESS;
}

static int draw_cursorcolor_row(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    ColorConfig* prColor = NULL;
    ColorRow rRow;
    ColorConfig* prSrc = NULL;
    ColorConfig* prDst = NULL;
    int x = 0;
    int y = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prConfig = prPanel->prCEConfig;
    if (prConfig == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prColor = &prConfig->rAppletConfig.rPanel.rCursor;
    textcolumn_init(&rRow.rLabel);
    dwordcolumn_init(&rRow.rBackground);
    dwordcolumn_init(&rRow.rText);
    rRow.rLabel.prColor = prColor;
    rRow.rLabel.columns = 30;
    textcolumn_settext(&rRow.rLabel, "Cursor Color (bg/fg)");
    rRow.rBackground.color.background = prPanel->config.color.background;
    rRow.rBackground.color.text = prPanel->config.color.text;
    rRow.rText.color.background = prPanel->config.color.background;
    rRow.rText.color.text = prPanel->config.color.text;
    dwordcolumn_setvalue(&rRow.rBackground, prColor->background);
    dwordcolumn_setvalue(&rRow.rText, prColor->text);
    x = prPanel->config.position.x;
    y = prPanel->config.position.y + 3;
    pspDebugScreenSetXY(x, y);
    textcolumn_redraw(&rRow.rLabel);
    if (prPanel->cursor.y == 2) {
        prDst = &rRow.rBackground.color;
        if (prPanel->cursor.x == 0) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rBackground);
    pspDebugScreenSetXY(x + 41, y);
    if (prPanel->cursor.y == 2) {
        prDst = &rRow.rText.color;
        if (prPanel->cursor.x == 1) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rText);
    return OPTIONSPANEL_SUCCESS;
}

static int draw_editcolor_row(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    ColorConfig* prColor = NULL;
    ColorRow rRow;
    ColorConfig* prSrc = NULL;
    ColorConfig* prDst = NULL;
    int x = 0;
    int y = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prConfig = prPanel->prCEConfig;
    if (prConfig == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prColor = &prConfig->rAppletConfig.rPanel.rEdit;
    textcolumn_init(&rRow.rLabel);
    dwordcolumn_init(&rRow.rBackground);
    dwordcolumn_init(&rRow.rText);
    rRow.rLabel.prColor = prColor;
    rRow.rLabel.columns = 30;
    textcolumn_settext(&rRow.rLabel, "Edit Color (bg/fg)");
    rRow.rBackground.color.background = prPanel->config.color.background;
    rRow.rBackground.color.text = prPanel->config.color.text;
    rRow.rText.color.background = prPanel->config.color.background;
    rRow.rText.color.text = prPanel->config.color.text;
    dwordcolumn_setvalue(&rRow.rBackground, prColor->background);
    dwordcolumn_setvalue(&rRow.rText, prColor->text);
    x = prPanel->config.position.x;
    y = prPanel->config.position.y + 4;
    pspDebugScreenSetXY(x, y);
    textcolumn_redraw(&rRow.rLabel);
    if (prPanel->cursor.y == 3) {
        prDst = &rRow.rBackground.color;
        if (prPanel->cursor.x == 0) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rBackground);
    pspDebugScreenSetXY(x + 41, y);
    if (prPanel->cursor.y == 3) {
        prDst = &rRow.rText.color;
        if (prPanel->cursor.x == 1) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rText);
    return OPTIONSPANEL_SUCCESS;
}

static int draw_pause_row(OptionsPanel* prPanel) {
    GamePauseRow* prRow = NULL;
    TextColumn* prLabel = NULL;
    ToggleColumn* prToggle = NULL;
    CEFiveConfig* prConfig = NULL;
    ColorConfig* prColor = NULL;
    int x = 0;
    int y = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    x = prPanel->config.position.x;
    y = prPanel->config.position.y + 1;
    prRow = &(prPanel->rPauseRow);
    prLabel = &(prRow->rLabel);
    prToggle = &(prRow->rToggle);
    prConfig = prPanel->prCEConfig;

    prToggle->value = prConfig->pause_during_ui;

    pspDebugScreenSetXY(x, y);
    if (textcolumn_redraw(prLabel) != TEXTCOLUMN_SUCCESS) {
        pspDebugScreenKprintf("Unable to render TextColumn.\n");
        return OPTIONSPANEL_FAILURE;
    }
    if (prPanel->cursor.y == 0) {
        prColor = &prConfig->rAppletConfig.rPanel.rCursor;
    } else {
        prColor = &prConfig->rAppletConfig.rPanel.rColor;
    }
    prToggle->prColor = prColor;
    togglecolumn_redraw(prToggle);
    return OPTIONSPANEL_SUCCESS;
}

static int draw_statuscolor_row(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    ColorConfig* prColor = NULL;
    ColorRow rRow;
    ColorConfig* prSrc = NULL;
    ColorConfig* prDst = NULL;
    int x = 0;
    int y = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prConfig = prPanel->prCEConfig;
    if (prConfig == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prColor = &prConfig->rAppletConfig.rStatus;
    textcolumn_init(&rRow.rLabel);
    dwordcolumn_init(&rRow.rBackground);
    dwordcolumn_init(&rRow.rText);
    rRow.rLabel.prColor = prColor;
    rRow.rLabel.columns = 30;
    textcolumn_settext(&rRow.rLabel, "Status/Menu Color (bg/fg)");
    rRow.rBackground.color.background = prPanel->config.color.background;
    rRow.rBackground.color.text = prPanel->config.color.text;
    rRow.rText.color.background = prPanel->config.color.background;
    rRow.rText.color.text = prPanel->config.color.text;
    dwordcolumn_setvalue(&rRow.rBackground, prColor->background);
    dwordcolumn_setvalue(&rRow.rText, prColor->text);
    x = prPanel->config.position.x;
    y = prPanel->config.position.y + 6;
    pspDebugScreenSetXY(x, y);
    textcolumn_redraw(&rRow.rLabel);
    if (prPanel->cursor.y == 5) {
        prDst = &rRow.rBackground.color;
        if (prPanel->cursor.x == 0) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rBackground);
    pspDebugScreenSetXY(x + 41, y);
    if (prPanel->cursor.y == 5) {
        prDst = &rRow.rText.color;
        if (prPanel->cursor.x == 1) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rText);
    return OPTIONSPANEL_SUCCESS;
}

static int draw_titlecolor_row(OptionsPanel* prPanel) {
    CEFiveConfig* prConfig = NULL;
    ColorConfig* prColor = NULL;
    ColorRow rRow;
    ColorConfig* prSrc = NULL;
    ColorConfig* prDst = NULL;
    int x = 0;
    int y = 0;

    if (prPanel == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prConfig = prPanel->prCEConfig;
    if (prConfig == NULL) {
        return OPTIONSPANEL_MEMORY;
    }
    prColor = &prConfig->rAppletConfig.rTitlebar;
    textcolumn_init(&rRow.rLabel);
    dwordcolumn_init(&rRow.rBackground);
    dwordcolumn_init(&rRow.rText);
    rRow.rLabel.prColor = prColor;
    rRow.rLabel.columns = 30;
    textcolumn_settext(&rRow.rLabel, "Titlebar Color (bg/fg)");
    rRow.rBackground.color.background = prPanel->config.color.background;
    rRow.rBackground.color.text = prPanel->config.color.text;
    rRow.rText.color.background = prPanel->config.color.background;
    rRow.rText.color.text = prPanel->config.color.text;
    dwordcolumn_setvalue(&rRow.rBackground, prColor->background);
    dwordcolumn_setvalue(&rRow.rText, prColor->text);
    x = prPanel->config.position.x;
    y = prPanel->config.position.y + 5;
    pspDebugScreenSetXY(x, y);
    textcolumn_redraw(&rRow.rLabel);
    if (prPanel->cursor.y == 4) {
        prDst = &rRow.rBackground.color;
        if (prPanel->cursor.x == 0) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rBackground);
    pspDebugScreenSetXY(x + 41, y);
    if (prPanel->cursor.y == 4) {
        prDst = &rRow.rText.color;
        if (prPanel->cursor.x == 1) {
            prSrc = &prConfig->rAppletConfig.rPanel.rCursor;
        } else {
            prSrc = &prConfig->rAppletConfig.rPanel.rColor;
        }
        colorconfig_setcolor(prDst, prSrc->background, prSrc->text);
    }
    dwordcolumn_redraw(&rRow.rText);
    return OPTIONSPANEL_SUCCESS;
}
