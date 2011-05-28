/* cefiveui.c
 *  The User Interface for CEFive.
 * Authors:
 *  Sir Gee of Five
 *  PSP_Lord
 */

#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include "cefiveui.h"
#include "cheatpanel.h"
#include "cheateditor.h"
#include "disassembler.h"
#include "cheatengine.h"
#include "hexeditor.h"
#include "gameinfo.h"
#include "optionspanel.h"
#include "searchpanel.h"
#include "searchengine.h"
#include "cefive.h"

static void button_circle_down(CEFiveUi *prUi);
static void button_circle_up(CEFiveUi *prUi);
static void button_cross_down(CEFiveUi *prUi);
static void button_cross_up(CEFiveUi *prUi);
static void button_ltrigger_down(CEFiveUi *prUi);
static void button_ltrigger_up(CEFiveUi *prUi);
static void button_rtrigger_down(CEFiveUi *prUi);
static void button_rtrigger_up(CEFiveUi *prUi);
static void button_square_down(CEFiveUi* prUi);
static void button_square_up(CEFiveUi* prUi);
static void button_triangle_down(CEFiveUi* prUi);
static void button_triangle_up(CEFiveUi* prUi);
static void close_cheat_editor(CEFiveUi *prUi);
static void draw_applet(CEFiveUi *prUi);
static void draw_applet_menu(CEFiveUi *prUi);
static void draw_cheat_status(CEFiveUi *prUi);
static void draw_titlebar(CEFiveUi *prUi);
static void edit_selected_cheat(CEFiveUi *prUi);
static void dpad_down_down(CEFiveUi *prUi);
static void dpad_down_hold(CEFiveUi* prUi);
static void dpad_down_up(CEFiveUi *prUi);
static void dpad_left_down(CEFiveUi *prUi);
static void dpad_left_up(CEFiveUi *prUi);
static void dpad_right_down(CEFiveUi *prUi);
static void dpad_right_up(CEFiveUi *prUi);
static void dpad_up_down(CEFiveUi *prUi);
static void dpad_up_up(CEFiveUi *prUi);

static int init_appletmenu(CEFiveUi *prUi);
static int init_cheateditor(CEFiveUi *prUi);
static int init_cheatpanel(CEFiveUi *prUi);
static int init_disassembler(CEFiveUi *prUi);
static int init_hexeditor(CEFiveUi* prUi);

static void button_circle_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.circle = 1;
    }
}

static void button_circle_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.circle = 0;
        if (prUi->applet == 1) {
            close_cheat_editor(prUi);
            return;
        }
        if (prUi->applet == 2) {
            if (disassembler_is_editing(&prUi->disassembler)) {
                disassembler_button_circle(&prUi->disassembler);
                return;
            }
        }
        
        if (prUi->applet == 4) {
            if (searchpanel_circle_button(&prUi->searchpanel) == SEARCHPANEL_SUCCESS) {
                return;
            }        
        }
        
        if (prUi->appletmenu.visible == 1) {
            appletmenu_circle_button(&prUi->appletmenu);
            return;
        } 
        prUi->drawn = 0;
        prUi->running = 0;
    }
}

static void button_cross_down(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    prUi->buttons.cross = 1;
}

static void button_cross_up(CEFiveUi *prUi) {
    int index = -1;
    if (prUi != NULL) {
        prUi->buttons.cross = 0;
        if (prUi->appletmenu.visible == 1) {
            appletmenuCrossButton(&prUi->appletmenu);
            if (prUi->appletmenu.visible == 0) {
                index = appletmenuGetSelectedIndex(&prUi->appletmenu);
                switch (index) {
                    case 0:
                        prUi->applet = 0;
                        prUi->drawn = 0;
                        break;
                    case 1:
                        prUi->applet = 2; /* Disassembler applet */
                        prUi->drawn = 0;
                        prUi->disassembler.dirty = 1;
                        break;
                    case 2:
                        prUi->applet = 3; /* Hexeditor applet */
                        prUi->drawn = 0;
                        prUi->hexeditor.dirty = 1;
                        break;
                    case 3:
                        prUi->applet = 4; /* Search Panel applet */
                        prUi->drawn = 0;
                        break;
                    case 4:
                        prUi->applet = 5; /* Game info applet*/
                        prUi->drawn = 0;
                        break;
                    case 5:
                        prUi->applet = 6; /* Options applet*/
                        prUi->drawn = 0;
                        break;
                }
            }
        } else {
            switch(prUi->applet) {
                case 0:
                    cheatpanel_cross_button(&prUi->cheatpanel);
                    break;
                case 1:
                    cheateditorCrossButton(&prUi->cheateditor);
                    break;
                case 2:
                    disassembler_button_cross(&prUi->disassembler);
                    break;
                case 3:
                    hexeditorCrossButton(&prUi->hexeditor);
                    break;
                case 4:
                    searchpanel_cross_button(&prUi->searchpanel);
                    break;
                case 6:
                    optionspanel_cross_button(&prUi->optionspanel);
                    break;
            }
        }
    }
}

static void button_ltrigger_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.ltrigger = 1;
    }
}

/* print/close the applet menu */
static void button_ltrigger_up(CEFiveUi *prUi) {
    AppletMenu* prMenu = NULL;
    if (prUi != NULL) {
        prMenu = cefiveui_get_appletmenu(prUi);
        prUi->buttons.ltrigger = 0;
        /* close the applet menu */
        if (prMenu->visible == 1) {
            cefiveui_log(prUi, LOG_DEBUG, 
                    "cefiveui buttonLTriggerUp: Hiding Applet Menu.");
            prMenu->visible = 0;
            appletmenu_invalidate(prMenu);
            prUi->drawn = 0;
            return;
        }
        /* draw the applet menu */
        if (prMenu->visible == 0) {
            /* Send ltriggers from the disassembler through if editing. */
            if (prUi->applet == 2) {
                if (disassembler_is_editing(&prUi->disassembler)) {
                    disassembler_button_ltrigger(&prUi->disassembler);
                    return;
                }
            }
            /* Don't pop the applet menu while the Cheat Editor is showing. */
            if (prUi->applet != 1) {
                cefiveui_log(prUi, LOG_DEBUG,
                        "cefiveui buttonLTriggerUp: Showing Applet Menu.");
                prMenu->visible = 1;
                appletmenu_invalidate(prMenu);
                return;
            }
        }
    }
}

static void button_rtrigger_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.rtrigger = 1;
    }
}

static void button_rtrigger_up(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    prUi->buttons.rtrigger = 0;
    switch (prUi->applet) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            if (disassembler_is_editing(&prUi->disassembler)) {
                disassembler_button_rtrigger(&prUi->disassembler);
            }
            break;
    }
}

static void button_square_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.square = 1;
    }
}

static void button_square_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.square = 0;
        if (prUi->appletmenu.visible == 1) {
            return;
        }
        switch (prUi->applet) {
            case 0:
                cheatpanel_square_button(&prUi->cheatpanel);
                break;
            case 2:
                disassembler_button_square(&prUi->disassembler);
                break;
        }
    }
}

static void button_triangle_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.triangle = 1;
    }
}

static void button_triangle_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.triangle = 0;
        switch (prUi->applet) {
            case 0:
                edit_selected_cheat(prUi);
                break;
            case 2:
                disassembler_button_triangle(&prUi->disassembler);
                break;
        }
    }
}

void cefiveui_buttoncallback(int curr, int last, CEFiveUi* prUi) {
    if (prUi == NULL) {
        return;
    }
    ButtonState rSc;
    ButtonState rLa;

    rSc.circle = (curr & PSP_CTRL_CIRCLE);
    rSc.cross = (curr & PSP_CTRL_CROSS);
    rSc.down = (curr & PSP_CTRL_DOWN);
    rSc.left = (curr & PSP_CTRL_LEFT);
    rSc.ltrigger = (curr & PSP_CTRL_LTRIGGER);
    rSc.right = (curr & PSP_CTRL_RIGHT);
    rSc.rtrigger = (curr & PSP_CTRL_RTRIGGER);
    rSc.select = (curr & PSP_CTRL_SELECT);
    rSc.square = (curr & PSP_CTRL_SQUARE);
    rSc.start = (curr & PSP_CTRL_START);
    rSc.triangle = (curr & PSP_CTRL_TRIANGLE);
    rSc.up = (curr & PSP_CTRL_UP);

    rLa.circle = (last & PSP_CTRL_CIRCLE);
    rLa.cross = (last & PSP_CTRL_CROSS);
    rLa.down = (last & PSP_CTRL_DOWN);
    rLa.left = (last & PSP_CTRL_LEFT);
    rLa.ltrigger = (last & PSP_CTRL_LTRIGGER);
    rLa.right = (last & PSP_CTRL_RIGHT);
    rLa.rtrigger = (last & PSP_CTRL_RTRIGGER);
    rLa.select = (last & PSP_CTRL_SELECT);
    rLa.square = (last & PSP_CTRL_SQUARE);
    rLa.start = (last & PSP_CTRL_START);
    rLa.triangle = (last & PSP_CTRL_TRIANGLE);
    rLa.up = (last & PSP_CTRL_UP);

    if (rSc.circle && !rLa.circle) button_circle_down(prUi);
    if (rLa.circle && !rSc.circle) button_circle_up(prUi);
    if (rSc.cross && !rLa.cross) button_cross_down(prUi);
    if (rLa.cross && !rSc.cross) button_cross_up(prUi);
    if (rSc.down && !rLa.down) dpad_down_down(prUi);
    if (rSc.down && rLa.down) dpad_down_down(prUi);
    if (rLa.down && !rSc.down) dpad_down_up(prUi);
    if (rSc.left && !rLa.left) dpad_left_down(prUi);
    if (rSc.left && rLa.left) dpad_left_down(prUi);
    if (rLa.left && !rSc.left) dpad_left_up(prUi);
    if (rSc.ltrigger && !rLa.ltrigger) button_ltrigger_down(prUi);
    if (rLa.ltrigger && !rSc.ltrigger) button_ltrigger_up(prUi);
    if (rSc.right && !rLa.right) dpad_right_down(prUi);
    if (rSc.right && rLa.right) dpad_right_down(prUi);
    if (rLa.right && !rSc.right) dpad_right_up(prUi);
    if (rSc.rtrigger && !rLa.rtrigger) button_rtrigger_down(prUi);
    if (rLa.rtrigger && !rSc.rtrigger) button_rtrigger_up(prUi);
    if (rSc.square && !rLa.square) button_square_down(prUi);
    if (rLa.square && !rSc.square) button_square_up(prUi);
    if (rSc.triangle && !rLa.triangle) button_triangle_down(prUi);
    if (rLa.triangle && !rSc.triangle) button_triangle_up(prUi);
    if (rSc.up && !rLa.up) dpad_up_down(prUi);
    if (rSc.up && rLa.up) dpad_up_down(prUi);
    if (rLa.up && !rSc.up) dpad_up_up(prUi);
}

AppletConfig* cefiveui_get_appletconfig(CEFiveUi* prUi) {
    CEFiveConfig* prConfig = NULL;
    if (prUi != NULL) {
        prConfig = cefiveui_get_config(prUi);
        return &prConfig->rAppletConfig;
    }
    return NULL;
}

AppletMenu* cefiveui_get_appletmenu(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return &prUi->appletmenu;
    }
    return NULL;
}

CheatEditor* cefiveui_get_cheateditor(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return &prUi->cheateditor;
    }
    return NULL;
}

CheatEngine* cefiveui_get_cheatengine(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return prUi->prEngine;
    }
    return NULL;
}

CheatPanel* cefiveui_get_cheatpanel(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return &prUi->cheatpanel;
    }
    return NULL;
}

CEFiveConfig* cefiveui_get_config(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return prUi->prCEConfig;
    }
    return NULL;
}

HexEditor* cefiveui_get_hexeditor(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return &prUi->hexeditor;
    }
    return NULL;
}

CEFiveUiConfig* cefiveui_get_uiconfig(CEFiveUi* prUi) {
    if (prUi != NULL) {
        return &prUi->config;
    }
    return NULL;
}

int cefiveuiHandleInput(CEFiveUi *prUi) {
    unsigned int btns = 0;
    if (prUi == NULL) {
        return 0;
    }
    int handled = 0;
    sceCtrlPeekBufferPositive(&prUi->controlData, 1);
    btns = prUi->controlData.Buttons;

    if (btns & PSP_CTRL_CROSS) {
        if (prUi->buttons.cross == 0) {
            button_cross_down(prUi);
        }
    } else {
        if (prUi->buttons.cross == 1) {
            button_cross_up(prUi);
        }
    }

    if (btns & PSP_CTRL_CIRCLE) {
        if (prUi->buttons.circle == 0) {
            button_circle_down(prUi);
        }
    } else {
        if (prUi->buttons.circle == 1) {
            button_circle_up(prUi);
        }
    }

    if (btns & PSP_CTRL_LTRIGGER) {
        if (prUi->buttons.ltrigger == 0) {
            button_ltrigger_down(prUi);
        }
    } else {
        if (prUi->buttons.ltrigger == 1) {
            button_ltrigger_up(prUi);
        }
    }

    if (btns & PSP_CTRL_RTRIGGER) {
        if (prUi->buttons.rtrigger == 0) {
            button_rtrigger_down(prUi);
        }
    } else {
        if (prUi->buttons.rtrigger == 1) {
            button_rtrigger_up(prUi);
        }
    }

    if (btns & PSP_CTRL_SQUARE) {
        if (prUi->buttons.square == 0) {
            button_square_down(prUi);
        }
    } else {
        if (prUi->buttons.square == 1) {
            button_square_up(prUi);
        }
    }

    if (btns & PSP_CTRL_TRIANGLE) {
        if (prUi->buttons.triangle == 0) {
            button_triangle_down(prUi);
        }
    } else {
        if (prUi->buttons.triangle == 1) {
            button_triangle_up(prUi);
        }
    }

    if (btns & PSP_CTRL_UP) {
        if (prUi->buttons.up == 0) {
            dpad_up_down(prUi);
        } else {
            dpad_up_up(prUi);
        }
    } else {
        if (prUi->buttons.up == 1) {
            dpad_up_up(prUi);
        }
    }
    if (btns & PSP_CTRL_DOWN) {
        if (prUi->buttons.down == 0) {
            dpad_down_down(prUi);
        } else {
            dpad_down_up(prUi);
        }
    } else {
        if (prUi->buttons.down == 1) {
            dpad_down_up(prUi);
        }
    }
    if (btns & PSP_CTRL_LEFT) {
        if (prUi->buttons.left == 0) {
            dpad_left_down(prUi);
        } else {
            dpad_left_up(prUi);
        }
    } else {
        if (prUi->buttons.left == 1) {
            dpad_left_up(prUi);
        }
    }
    if (btns & PSP_CTRL_RIGHT) {
        if (prUi->buttons.right == 0) {
            dpad_right_down(prUi);
        } else {
            dpad_right_up(prUi);
        }
    } else {
        if (prUi->buttons.right == 1) {
            dpad_right_up(prUi);
        }
    }

    return handled;
}

void cefiveuiInit(CEFiveUi* prUi, CheatEngine* prEngine,
        SearchEngine* prSearch) {
    const char* sFunc = "cefiveuiInit";
    CEFiveUiConfig* prUiCfg = NULL;
    CEFiveConfig* prConfig = NULL;
    AppletConfig* prAppCfg = NULL;
    PanelConfig* prPanel = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prSrc = NULL;
    CheatEditor* prEd = NULL;
    HexEditor* prHex = NULL;

    if (prUi == NULL) {
        return;
    }
    int toprow = 1;
    int tableheight = 25;
    
    prConfig = cefiveui_get_config(prUi);
    prUiCfg = cefiveui_get_uiconfig(prUi);

    prUi->prEngine = prEngine;
    prColor = &prUiCfg->color;
    
    prAppCfg = cefiveui_get_appletconfig(prUi);
    prPanel = appletconfig_get_panelconfig(prAppCfg);
    prSrc = panelconfig_get_panelcolor(prPanel);
    colorconfig_copy(prColor, prSrc);

    prColor = &prUiCfg->cursor;
    prSrc = panelconfig_get_cursorcolor(prPanel);
    colorconfig_copy(prColor, prSrc);

    prColor = &prUiCfg->editcursor;
    prSrc = panelconfig_get_editcolor(prPanel);
    colorconfig_copy(prColor, prSrc);

    prColor = &prUiCfg->status;
    prSrc = appletconfig_get_statuscolor(prAppCfg);
    colorconfig_copy(prColor, prSrc);

    prColor = &prUiCfg->titlebar;
    prSrc = appletconfig_get_titlecolor(prAppCfg);
    colorconfig_copy(prColor, prSrc);

    prUi->applet = 0;
    prUi->splash_viewed = 0;
    prUi->prSearchEngine = prSearch;

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Cheat Panel Applet.");
    if (init_cheatpanel(prUi) != CEFIVEUI_SUCCESS) {
        geelog_flog(LOG_WARN, sFunc, 
                "Failed to initialize Cheat Panel Applet.");
    }

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Cheat Editor Applet.");
    prEd = cefiveui_get_cheateditor(prUi);
    cheateditorInit(prEd, prEngine->cheatlist, prEngine->blocklist);
    prEd->prCeConfig = prConfig;
    prEd->prEngine = prEngine;
    prEd->cheat_count = prEngine->cheat_count;
    prEd->table_height = 30;
    prEd->top_row = toprow;

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Disassembler Applet.");
    Disassembler *prDasm = &prUi->disassembler;
    disassembler_init(prDasm, prAppCfg);

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Applet Menu.");
    if (init_appletmenu(prUi) != CEFIVEUI_SUCCESS) {
        geelog_flog(LOG_WARN, sFunc, "Failed to Initialize Applet Menu.");
    }

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Hex Editor Applet.");
    prHex = cefiveui_get_hexeditor(prUi);
    prHex->prApCfg = prAppCfg;
    hexeditorInit(prHex);
    prHex->config.color.background = prUi->config.color.background;
    prHex->config.color.text = prUi->config.color.text;
    prHex->config.cursor.background = prUi->config.cursor.background;
    prHex->config.cursor.text = prUi->config.cursor.text;
    prHex->config.address.background = (u32)0xFFA0A0A0;
    prHex->config.address.text = (u32)0xFF000000;
    prHex->config.datacolumns.background = prUi->config.color.background;
    prHex->config.datacolumns.text = prUi->config.color.text;
    prHex->config.ascii.background = prUi->config.color.background;
    prHex->config.ascii.text = prUi->config.color.text;
    prHex->config.base_address = 0x08800000;
    prHex->config.bytes_per_line = 16;
    prHex->config.max_offset = 0x49FFFFFF;
    prHex->config.min_offset = 0x48800000;
    prHex->config.position.x = 0;
    prHex->config.position.y = 1;
    prHex->config.table_pos.x = 0;
    prHex->config.table_pos.y = 2;
    prHex->config.table_size.height = 30;
    prHex->config.table_size.width = 0;
    prHex->cursor.x = 0;
    prHex->cursor.y = 0;
    prHex->dirty = 1;
    prHex->offset = 0x48800000;
    prHex->address_editor.background_color = prUi->config.editcursor.background;
    prHex->address_editor.text_color = prUi->config.cursor.text;
    prHex->address_editor.edit_color = prUi->config.editcursor.text;

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Search Panel Applet.");
    SearchPanel* prSp = &(prUi->searchpanel);
    prSp->prApCfg = prAppCfg;
    searchpanel_init(prSp, prUi->prSearchEngine);
    prSp->config.color.background = prUi->config.color.background;
    prSp->config.color.text = prUi->config.color.text;
    prSp->config.cursor.background = prUi->config.cursor.background;
    prSp->config.cursor.text = prUi->config.cursor.text;
    prSp->config.size.height = tableheight;
    prSp->config.top.x = 0;
    prSp->config.top.y = 1;

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Options Panel Applet.");
    OptionsPanel* prOp = &prUi->optionspanel;
    optionspanel_init(prOp, prUi->prCEConfig);
    prOp->config.color.background = prUi->config.color.background;
    prOp->config.color.text = prUi->config.color.text;
    prOp->config.cursor.background = prUi->config.cursor.background;
    prOp->config.cursor.text = prUi->config.cursor.text;
    prOp->config.edit.background = prUi->config.editcursor.background;
    prOp->config.edit.text = prUi->config.editcursor.text;
    prOp->config.position.x = 0;
    prOp->config.position.y = 1;
    prOp->cursor.x = 0;
    prOp->cursor.y = 0;

    geelog_flog(LOG_DEBUG, sFunc, "Initializing Game Info Applet.");
    GameInfo* prInfo = &prUi->gameinfo;
    prColor = &prUi->config.color;
    gameinfo_init(prInfo);
    gameinfo_set_appletconfig(prInfo, prAppCfg);
    gameinfo_set_colorconfig(prInfo, prColor);
    gameinfo_set_cursor(prInfo, 0, 1);

    geelog_flog(LOG_INFO, sFunc, "User Interface Initialized.");
}


void cefiveuiRedraw(CEFiveUi *prUi) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prUi == NULL) {
        return;
    }
    prCfg = prUi->prCEConfig;
    if (prCfg != NULL) {
        prColor = cefiveconfig_get_panelcolor(prCfg);
    } else {
        prColor = &prUi->config.color;
    }
    if (prUi->drawn == 0) {
        //pspDebugScreenInitEx(prUi->vram, 0, 0);
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenClear();
    }
    if (prUi->appletmenu.visible == 1) {
        draw_applet_menu(prUi);
    } else {
        draw_titlebar(prUi);
        draw_applet(prUi);
    }
    prUi->drawn = 1;
}

int cefiveui_log(CEFiveUi* prUi, ELogLevel level, const char* sMsg) {
    if (prUi == NULL) {
        return CEFIVEUI_NULLPTR;
    }
    geelog_log(level, sMsg);
    return CEFIVEUI_SUCCESS;
}

int cefiveui_update_controls(CEFiveUi* prUi) {
    static unsigned int last = 0;
    static int rpt = 0;
    SceCtrlData rCdata;
    unsigned int curr = 0;
    
    if (prUi == NULL) {
        return CEFIVEUI_NULLPTR;
    }
    sceCtrlPeekBufferPositive(&rCdata, 1);
    curr = rCdata.Buttons;
    if (curr == last && last != 0) {
        rpt++;
        if (rpt > 2) {
            cefiveui_buttoncallback(curr, last, prUi);
            rpt = 0;
        }
    }
    last = curr;
    
    return CEFIVEUI_SUCCESS;
}

static void close_cheat_editor(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    cefiveui_log(prUi, LOG_DEBUG, "cefiveui closeCheatEditor: Closing Editor.");
    prUi->applet = 0;
    prUi->drawn = 0;
}

static void dpad_down_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.down = 1;
        if (prUi->appletmenu.visible == 1) {
            appletmenuCursorDown(&prUi->appletmenu);
            return;
        }
        switch(prUi->applet) {
            case 0:
                cheatpanel_dpad_down(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadDown(&prUi->cheateditor);
                break;
            case 2:
                if (prUi->buttons.square == 1) {
                    disassembler_page_down(&prUi->disassembler);
                } else {
                    disassembler_dpad_down(&prUi->disassembler);
                }
                break;
            case 3:
                if (prUi->buttons.square == 1) {
                    hexeditorPageDown(&prUi->hexeditor);
                } else {
                    hexeditorDpadDown(&prUi->hexeditor);
                }
                break;
            case 4:
                if (prUi->buttons.square == 1) {
                    searchpanel_page_down(&prUi->searchpanel);
                } else {
                    searchpanel_dpad_down(&prUi->searchpanel);
                }
                break;
            case 5:
                break;
            case 6:
                optionspanel_dpad_down(&prUi->optionspanel);
                break;
        }
    }
}

static void dpad_down_hold(CEFiveUi* prUi) {
    static int downcount = 0;
    if (prUi == NULL) {
        return;
    }
    if (downcount < 2) {
        downcount++;
    } else {
        dpad_down_down(prUi);
        downcount = 0;
    }
}

static void dpad_down_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.down = 0;
    }
}

static void dpad_left_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.left = 1;
        switch(prUi->applet) {
            case 0:
                cheatpanel_dpad_left(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadLeft(&prUi->cheateditor);
                break;
            case 2:
                disassembler_dpad_left(&prUi->disassembler);
                break;
            case 3:
                hexeditorDpadLeft(&prUi->hexeditor);
                break;
            case 4:
                searchpanel_dpad_left(&prUi->searchpanel);
                break;
            case 5:
                break;
            case 6:
                optionspanel_dpad_left(&prUi->optionspanel);
                break;
        }
    }
}

static void dpad_left_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.left = 0;
    }
}

static void dpad_right_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.right = 1;
        switch(prUi->applet) {
            case 0:
                cheatpanel_dpad_right(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadRight(&prUi->cheateditor);
                break;
            case 2:
                disassembler_dpad_right(&prUi->disassembler);
                break;
            case 3:
                hexeditorDpadRight(&prUi->hexeditor);
                break;
            case 4:
                searchpanel_dpad_right(&prUi->searchpanel);
                break;
            case 5:
                break;
            case 6:
                optionspanel_dpad_right(&prUi->optionspanel);
        }
    }
}

static void dpad_right_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.right = 0;
    }
}

static void dpad_up_down(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.up = 1;
        if (prUi->appletmenu.visible == 1) {
            appletmenuCursorUp(&prUi->appletmenu);
            return;
        }
        switch(prUi->applet) {
            case 0:
                cheatpanel_dpad_up(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadUp(&prUi->cheateditor);
                break;
            case 2:
                if (prUi->buttons.square == 1) {
                    disassembler_page_up(&prUi->disassembler);
                } else {
                    disassembler_dpad_up(&prUi->disassembler);
                }
                break;
            case 3:
                if (prUi->buttons.square == 1) {
                    hexeditorPageUp(&prUi->hexeditor);
                } else {
                    hexeditorDpadUp(&prUi->hexeditor);
                }
                break;
            case 4:
                if (prUi->buttons.square == 1) {
                    searchpanel_page_up(&prUi->searchpanel);
                } else {
                    searchpanel_dpad_up(&prUi->searchpanel);
                }
                break;
            case 5:
                break;
            case 6:
                optionspanel_dpad_up(&prUi->optionspanel);
                break;
        }
    }
}

static void dpad_up_up(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.up = 0;
    }
}

static void draw_applet(CEFiveUi *prUi) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prUi == NULL) {
        return;
    }
    prCfg = prUi->prCEConfig;
    switch(prUi->applet) {
        case 0:
            if (prUi->drawn == 0) {
                prUi->cheatpanel.dirty = 1;
            }
            cheatpanel_redraw(&prUi->cheatpanel);
            break;
        case 1:
            cheateditorRedraw(&prUi->cheateditor);
            break;
        case 2:
            if (prUi->drawn == 0) {
                prUi->disassembler.dirty = 1;
            }
            disassembler_redraw(&prUi->disassembler);
            break;
        case 3:
            if (prUi->drawn == 0) {
                prUi->hexeditor.dirty = 1;
            }
            hexeditorRedraw(&prUi->hexeditor);
            break;
        case 4:
            searchpanel_redraw(&prUi->searchpanel);
            break;
        case 5:
            gameinfoRedraw(&prUi->gameinfo);
            break;
        case 6:
            optionspanel_redraw(&prUi->optionspanel);
            break;
    }
    if (prCfg != NULL) {
        prColor = cefiveconfig_get_panelcolor(prCfg);
    } else {
        prColor = &prUi->config.color;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
}

static void draw_applet_menu(CEFiveUi *prUi) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    if (prUi == NULL) {
        return;
    }
    prCfg = prUi->prCEConfig;
    AppletMenu *prMenu = &prUi->appletmenu;
    
    appletmenuRedraw(prMenu);
    if (prCfg != NULL) {
        prColor = cefiveconfig_get_panelcolor(prCfg);
    } else {
        prColor = &prUi->config.color;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
}

static void draw_cheat_status(CEFiveUi *prUi) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    if (prUi == NULL) {
        return;
    }
    prCfg = prUi->prCEConfig;
    if (prCfg != NULL) {
        prColor = cefiveconfig_get_titlecolor(prCfg);
    } else {
        prColor = &prUi->config.titlebar;
    }
    CheatEngine* prEngine = prUi->prEngine;
    int trigger = prEngine->trigger_active;
    u32 fg = (u32)0xFF0000FF;
    char* msg = "Cheats OFF";
    if (trigger == 1) {
        msg = "Cheats ON";
        fg = (u32)0xFF00FF00;
    }
    pspDebugScreenSetXY(50, 0);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenPuts(msg);
}

static void draw_titlebar(CEFiveUi *prUi) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    if (prUi == NULL) {
        return;
    }
    prCfg = prUi->prCEConfig;
    if (prCfg != NULL) {
        prColor = cefiveconfig_get_titlecolor(prCfg);
    } else {
        prColor = &prUi->config.titlebar;
    }
    char buf[68];
    u32 bg = prColor->background;
    u32 fg = prColor->text;
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    sprintf(buf, "CEFive %d.%d - %s",
            CEFIVE_VERSION_MAJ, CEFIVE_VERSION_MIN, prUi->game_id);
    pspDebugScreenKprintf("%-68s", buf);
    draw_cheat_status(prUi);
}

static void edit_selected_cheat(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    int index = cheatpanel_get_selectedindex(&prUi->cheatpanel);
    cheateditorSelectCheat(&prUi->cheateditor, index);
    prUi->applet = 1;
    prUi->drawn = 0;
    prUi->cheateditor.dirty = 1;
}

static int init_appletmenu(CEFiveUi* prUi) {
    AppletMenu* prMenu = NULL;
    AppletConfig* prConfig = NULL;
    if (prUi == NULL) {
        return CEFIVEUI_NULLPTR;
    }
    prConfig = cefiveui_get_appletconfig(prUi);
    prMenu = cefiveui_get_appletmenu(prUi);
    if (appletmenu_init(prMenu) != APPLETMENU_SUCCESS) {
        return CEFIVEUI_FAILURE;
    }
    prMenu->prApCfg = prConfig;
    if (appletmenu_set_position(prMenu, 0, 1) != APPLETMENU_SUCCESS) {
        return CEFIVEUI_FAILURE;
    }
    if (appletmenu_set_size(prMenu, 20, 10) != APPLETMENU_SUCCESS) {
        return CEFIVEUI_FAILURE;
    }
    return CEFIVEUI_SUCCESS;
}

static int init_cheatpanel(CEFiveUi *prUi) {
    CheatPanel* prPanel = NULL;
    AppletConfig* prConfig = NULL;
    CheatEngine* prEngine = NULL;
    ColorConfig* prColor = NULL;
    
    if (prUi == NULL) {
        return CEFIVEUI_NULLPTR;
    }
    prPanel = cefiveui_get_cheatpanel(prUi);
    prConfig = cefiveui_get_appletconfig(prUi);
    prEngine = cefiveui_get_cheatengine(prUi);
    
    cheatpanel_init(prPanel, prEngine);
    prPanel->prApCfg = prConfig;
    if (prConfig != NULL) {
        prColor = appletconfig_get_panelcolor(prConfig);
        cheatpanel_set_selectedcolor(prPanel, prColor->background, 
                (u32)0xFF00D000);
        cheatpanel_set_constantcolor(prPanel, prColor->background,
                (u32)0xFF00D0D0);
    }
    cheatpanel_set_tableheight(prPanel, 30);
    cheatpanel_set_cursorpos(prPanel, 0, 0);
    prPanel->config.top_row = 1;
    prPanel->page_position = 0;
    return CEFIVEUI_SUCCESS;
}
