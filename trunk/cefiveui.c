/* cefiveui.c
 *  The User Interface for CEFive.
 * Author:
 *  Sir Gee of Five
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

static void closeCheatEditor(CEFiveUi *prUi);
static void drawApplet(CEFiveUi *prUi);
static void drawAppletMenu(CEFiveUi *prUi);
static void drawCheatStatus(CEFiveUi *prUi);
static void drawTitlebar(CEFiveUi *prUi);
static void editSelectedCheat(CEFiveUi *prUi);
static void dpadDownDown(CEFiveUi *prUi);
static void dpadDownHold(CEFiveUi* prUi);
static void dpadDownUp(CEFiveUi *prUi);
static void dpadLeftDown(CEFiveUi *prUi);
static void dpadLeftUp(CEFiveUi *prUi);
static void dpadRightDown(CEFiveUi *prUi);
static void dpadRightUp(CEFiveUi *prUi);
static void dpadUpDown(CEFiveUi *prUi);
static void dpadUpUp(CEFiveUi *prUi);

static void buttonCircleDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.circle = 1;
    }
}

static void buttonCircleUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.circle = 0;
        if (prUi->applet == 1) {
            closeCheatEditor(prUi);
            return;
        }
        if (prUi->applet == 2) {
            if (prUi->disassembler.editing == 1) {
                disassemblerCircleButton(&prUi->disassembler);
                return;
            }
        }
      if (prUi->appletmenu.visible == 1) {
            appletmenuCircleButton(&prUi->appletmenu);
            return;
        } 
        prUi->drawn = 0;
        prUi->running = 0;
    }
}

static void buttonCrossDown(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    prUi->buttons.cross = 1;
}

static void buttonCrossUp(CEFiveUi *prUi) {
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
                        prUi->applet = 2;
                        prUi->drawn = 0;
                        prUi->disassembler.dirty = 1;
                        break;
                    case 2:
                        prUi->applet = 3;
                        prUi->drawn = 0;
                        prUi->hexeditor.dirty = 1;
                        break;
                    case 3:
                        prUi->applet = 4;
                        prUi->drawn = 0;
                        break;
                    case 4:
                        prUi->applet = 5;
                        prUi->drawn = 0;
                        break;
                    case 5:
                        prUi->applet = 6;
                        prUi->drawn = 0;
                        break;
                }
            }
        } else {
            switch(prUi->applet) {
                case 0:
                    cheatpanelCrossButton(&prUi->cheatpanel);
                    break;
                case 1:
                    cheateditorCrossButton(&prUi->cheateditor);
                    break;
                case 2:
                    disassemblerCrossButton(&prUi->disassembler);
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

static void buttonLTriggerDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.ltrigger = 1;
    }
}

/* print/close the applet menu */
static void buttonLTriggerUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.ltrigger = 0;
        /* close the applet menu */
        if (prUi->appletmenu.visible == 1) {
            cefiveui_log(prUi, LOG_DEBUG, 
                    "cefiveui buttonLTriggerUp: Hiding Applet Menu.");
            prUi->appletmenu.visible = 0;
            prUi->drawn = 0;
            return;
        }
        /* draw the applet menu */
        if (prUi->appletmenu.visible == 0) {
            /* Don't pop the applet menu while the Cheat Editor is showing. */
            if (prUi->applet != 1) {
                cefiveui_log(prUi, LOG_DEBUG,
                        "cefiveui buttonLTriggerUp: Showing Applet Menu.");
                prUi->appletmenu.visible = 1;
                return;
            }
        }
    }
}

static void buttonRTriggerDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.rtrigger = 1;
    }
}

static void buttonRTriggerUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.rtrigger = 0;
    }
}

static void buttonSquareDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.square = 1;
    }
}

static void buttonSquareUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.square = 0;
        if (prUi->appletmenu.visible == 1) {
            return;
        }
        switch (prUi->applet) {
            case 0:
                cheatpanelSquareButton(&prUi->cheatpanel);
                break;
        }
    }
}

static void buttonTriangleDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.triangle = 1;
    }
}

static void buttonTriangleUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.triangle = 0;
        if (prUi->applet == 0) {
            editSelectedCheat(prUi);
            return;
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

    if (rSc.circle && !rLa.circle) buttonCircleDown(prUi);
    if (rLa.circle && !rSc.circle) buttonCircleUp(prUi);
    if (rSc.cross && !rLa.cross) buttonCrossDown(prUi);
    if (rLa.cross && !rSc.cross) buttonCrossUp(prUi);
    if (rSc.down && !rLa.down) dpadDownDown(prUi);
    if (rSc.down && rLa.down) dpadDownDown(prUi);
    if (rLa.down && !rSc.down) dpadDownUp(prUi);
    if (rSc.left && !rLa.left) dpadLeftDown(prUi);
    if (rSc.left && rLa.left) dpadLeftDown(prUi);
    if (rLa.left && !rSc.left) dpadLeftUp(prUi);
    if (rSc.ltrigger && !rLa.ltrigger) buttonLTriggerDown(prUi);
    if (rLa.ltrigger && !rSc.ltrigger) buttonLTriggerUp(prUi);
    if (rSc.right && !rLa.right) dpadRightDown(prUi);
    if (rSc.right && rLa.right) dpadRightDown(prUi);
    if (rLa.right && !rSc.right) dpadRightUp(prUi);
    if (rSc.rtrigger && !rLa.rtrigger) buttonRTriggerDown(prUi);
    if (rLa.rtrigger && !rSc.rtrigger) buttonRTriggerUp(prUi);
    if (rSc.square && !rLa.square) buttonSquareDown(prUi);
    if (rLa.square && !rSc.square) buttonSquareUp(prUi);
    if (rSc.triangle && !rLa.triangle) buttonTriangleDown(prUi);
    if (rLa.triangle && !rSc.triangle) buttonTriangleUp(prUi);
    if (rSc.up && !rLa.up) dpadUpDown(prUi);
    if (rSc.up && rLa.up) dpadUpDown(prUi);
    if (rLa.up && !rSc.up) dpadUpUp(prUi);
}

CEFiveConfig* cefiveui_get_config(CEFiveUi* prUi) {
    CEFiveConfig* prCfg = NULL;
    if (prUi != NULL) {
        prCfg = prUi->prCEConfig;
    }
    return prCfg;
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
            buttonCrossDown(prUi);
        }
    } else {
        if (prUi->buttons.cross == 1) {
            buttonCrossUp(prUi);
        }
    }

    if (btns & PSP_CTRL_CIRCLE) {
        if (prUi->buttons.circle == 0) {
            buttonCircleDown(prUi);
        }
    } else {
        if (prUi->buttons.circle == 1) {
            buttonCircleUp(prUi);
        }
    }

    if (btns & PSP_CTRL_LTRIGGER) {
        if (prUi->buttons.ltrigger == 0) {
            buttonLTriggerDown(prUi);
        }
    } else {
        if (prUi->buttons.ltrigger == 1) {
            buttonLTriggerUp(prUi);
        }
    }

    if (btns & PSP_CTRL_RTRIGGER) {
        if (prUi->buttons.rtrigger == 0) {
            buttonRTriggerDown(prUi);
        }
    } else {
        if (prUi->buttons.rtrigger == 1) {
            buttonRTriggerUp(prUi);
        }
    }

    if (btns & PSP_CTRL_SQUARE) {
        if (prUi->buttons.square == 0) {
            buttonSquareDown(prUi);
        }
    } else {
        if (prUi->buttons.square == 1) {
            buttonSquareUp(prUi);
        }
    }

    if (btns & PSP_CTRL_TRIANGLE) {
        if (prUi->buttons.triangle == 0) {
            buttonTriangleDown(prUi);
        }
    } else {
        if (prUi->buttons.triangle == 1) {
            buttonTriangleUp(prUi);
        }
    }

    if (btns & PSP_CTRL_UP) {
        if (prUi->buttons.up == 0) {
            dpadUpDown(prUi);
        } else {
            dpadUpUp(prUi);
        }
    } else {
        if (prUi->buttons.up == 1) {
            dpadUpUp(prUi);
        }
    }
    if (btns & PSP_CTRL_DOWN) {
        if (prUi->buttons.down == 0) {
            dpadDownDown(prUi);
        } else {
            dpadDownUp(prUi);
        }
    } else {
        if (prUi->buttons.down == 1) {
            dpadDownUp(prUi);
        }
    }
    if (btns & PSP_CTRL_LEFT) {
        if (prUi->buttons.left == 0) {
            dpadLeftDown(prUi);
        } else {
            dpadLeftUp(prUi);
        }
    } else {
        if (prUi->buttons.left == 1) {
            dpadLeftUp(prUi);
        }
    }
    if (btns & PSP_CTRL_RIGHT) {
        if (prUi->buttons.right == 0) {
            dpadRightDown(prUi);
        } else {
            dpadRightUp(prUi);
        }
    } else {
        if (prUi->buttons.right == 1) {
            dpadRightUp(prUi);
        }
    }

    return handled;
}

void cefiveuiInit(CEFiveUi* prUi, CheatEngine* prEngine,
        SearchEngine* prSearch) {
    CEFiveUiConfig* prUiCfg = NULL;
    CEFiveConfig* prConfig = NULL;
    AppletConfig* prAppCfg = NULL;
    PanelConfig* prPanel = NULL;
    ColorConfig* prColor = NULL;
    ColorConfig* prSrc = NULL;

    if (prUi == NULL) {
        return;
    }
    int toprow = 1;
    int tableheight = 25;
    
    prConfig = prUi->prCEConfig;
    prUiCfg = &prUi->config;

    prUi->prEngine = prEngine;
    prColor = &prUiCfg->color;
    prAppCfg = &prConfig->rAppletConfig;
    prPanel = &prAppCfg->rPanel;
    prSrc = &prPanel->rColor;
    colorconfig_setcolor(prColor, prSrc->background, prSrc->text);

    prColor = &prUiCfg->cursor;
    prSrc = &prPanel->rCursor;
    colorconfig_setcolor(prColor, prSrc->background, prSrc->text);

    prColor = &prUiCfg->editcursor;
    prSrc = &prPanel->rEdit;
    colorconfig_setcolor(prColor, prSrc->background, prSrc->text);

    prColor = &prUiCfg->status;
    prSrc = &prAppCfg->rStatus;
    colorconfig_setcolor(prColor, prSrc->background, prSrc->text);

    prColor = &prUiCfg->titlebar;
    prSrc = &prAppCfg->rTitlebar;
    colorconfig_setcolor(prColor, prSrc->background, prSrc->text);

    prUi->applet = 0;
    prUi->splash_viewed = 0;
    prUi->prSearchEngine = prSearch;

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing CheatPanel.");
    CheatPanel* prCp = &prUi->cheatpanel;
    cheatpanelInit(prCp, prEngine);
    prCp->prApCfg = &prConfig->rAppletConfig;
    prUi->cheatpanel.config.selectedcolor.text = (u32)0xFF00D000;
    prUi->cheatpanel.config.constantcolor.text = (u32)0xFFD00000;
    prUi->cheatpanel.config.table_height = 30;
    prUi->cheatpanel.config.top_row = toprow;
    prUi->cheatpanel.cursor.x = 0;
    prUi->cheatpanel.cursor.y = 0;
    prUi->cheatpanel.page_position = 0;

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing CheatEditor.");
    CheatEditor *prEd = &prUi->cheateditor;
    cheateditorInit(prEd, prEngine->cheatlist, prEngine->blocklist);
    prEd->prCeConfig = prConfig;
    prEd->prEngine = prEngine;
    prEd->cheat_count = prEngine->cheat_count;
    prEd->table_height = 30;
    prEd->top_row = toprow;
    
    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing Disassembler.");
    disassemblerInit(&prUi->disassembler);
    Disassembler *prDasm = &prUi->disassembler;
    prDasm->prApCfg = prAppCfg;
    prDasm->prLog = prUi->prLog;
    prDasm->config.code_color.background = prUi->config.color.background;
    prDasm->config.code_color.text = prUi->config.color.text;
    prDasm->config.address_color.background = (u32)0xFFA0A0A0;
    prDasm->config.address_color.text = prUi->config.color.text;
    prDasm->config.cursor_color.background = prUi->config.editcursor.background;
    prDasm->config.cursor_color.text = prUi->config.editcursor.text;
    prDasm->config.cursorrow_color.background = prUi->config.cursor.background;
    prDasm->config.cursorrow_color.text = prUi->config.cursor.text;
    prDasm->config.pointer_color.background = (u32)0xFFB0B0B0;
    prDasm->config.pointer_color.text = (u32)0xFF000060;
    prDasm->config.value_color.background = (u32)0xFFB0B0B0;
    prDasm->config.value_color.text = prUi->config.color.text;
    prDasm->config.position.x = 0;
    prDasm->config.position.y = 1;
    prDasm->config.tablesize.height = 30;
    prDasm->config.tablepos.y = 2;
    prDasm->address_editor.background_color = prUi->config.editcursor.background;
    prDasm->address_editor.edit_color = prUi->config.editcursor.text;
    prDasm->address_editor.text_color = prUi->config.cursor.text;
    prDasm->value_editor.background_color = prUi->config.editcursor.background;
    prDasm->value_editor.edit_color = prUi->config.editcursor.text;
    prDasm->value_editor.text_color = prUi->config.cursor.text;

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing AppletMenu.");
    AppletMenu *prMenu = &prUi->appletmenu;
    prMenu->prApCfg = prAppCfg;
    prMenu->config.top.y = 1;
    prMenu->cursor.y = 0;

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing HexEditor.");
    HexEditor *prHex = &prUi->hexeditor;
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

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing SearchPanel.");
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

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing OptionsPanel.");
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

    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Initializing GameInfo.");
    GameInfo* prInfo = &prUi->gameinfo;
    prColor = &prUi->config.color;
    prInfo->prLog = prUi->prLog;
    gameinfo_init(prInfo);
    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Assigning AppletConfig.");
    gameinfo_set_appletconfig(prInfo, prAppCfg);
    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Assigning ColorConfig.");
    gameinfo_set_colorconfig(prInfo, prColor);
    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: Assigning Cursor Position.");
    gameinfo_set_cursor(prInfo, 0, 1);
    
    cefiveui_log(prUi, LOG_DEBUG, "cefiveuiInit: UI Initialized.");
}


/* Redraw the cefive User Interface screen.
 * 
 * Parameters:
 *   prUi       Pointer to a CEFiveUi struct containing the User Interface.
 */
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
        drawAppletMenu(prUi);
    } else {
        drawTitlebar(prUi);
        drawApplet(prUi);
    }
    prUi->drawn = 1;
}

int cefiveui_log(CEFiveUi* prUi, ELogLevel level, const char* sMsg) {
    GeeLog* prLog = NULL;
    if (prUi == NULL) {
        return CEFIVEUI_NULLPTR;
    }
    prLog = prUi->prLog;
    if (prLog == NULL) {
        return CEFIVEUI_FAILURE;
    }
    geelog_log(prLog, level, sMsg);
    return CEFIVEUI_SUCCESS;
}

int cefiveui_set_logger(CEFiveUi* prUi, GeeLog *prLog) {
    if (prUi == NULL) {
        return CEFIVEUI_NULLPTR;
    }
    prUi->prLog = prLog;
    return CEFIVEUI_SUCCESS;
}

/* Update the current controller data, calling the button callback function
 * manually to simulate a key repeat.
 * 
 * Parameters:
 *   prUi       Pointer to a CEFiveUi struct containing the User Interface.
 * 
 * Returns:
 *   CEFIVEUI_NULLPTR is returned if the CEFiveUi pointer is NULL.
 *   CEFIVEUI_SUCCESS is returned on success.
 */
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

static void closeCheatEditor(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    cefiveui_log(prUi, LOG_DEBUG, "cefiveui closeCheatEditor: Closing Editor.");
    prUi->applet = 0;
    prUi->drawn = 0;
}

static void dpadDownDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.down = 1;
        if (prUi->appletmenu.visible == 1) {
            appletmenuCursorDown(&prUi->appletmenu);
            return;
        }
        switch(prUi->applet) {
            case 0:
                cheatpanelDpadDown(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadDown(&prUi->cheateditor);
                break;
            case 2:
                if (prUi->buttons.square == 1) {
                    disassemblerPageDown(&prUi->disassembler);
                } else {
                    disassemblerDpadDown(&prUi->disassembler);
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

static void dpadDownHold(CEFiveUi* prUi) {
    static int downcount = 0;
    if (prUi == NULL) {
        return;
    }
    if (downcount < 2) {
        downcount++;
    } else {
        dpadDownDown(prUi);
        downcount = 0;
    }
}

static void dpadDownUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.down = 0;
    }
}

static void dpadLeftDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.left = 1;
        switch(prUi->applet) {
            case 0:
                cheatpanelDpadLeft(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadLeft(&prUi->cheateditor);
                break;
            case 2:
                disassemblerDpadLeft(&prUi->disassembler);
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

static void dpadLeftUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.left = 0;
    }
}

static void dpadRightDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.right = 1;
        switch(prUi->applet) {
            case 0:
                cheatpanelDpadRight(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadRight(&prUi->cheateditor);
                break;
            case 2:
                disassemblerDpadRight(&prUi->disassembler);
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

static void dpadRightUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.right = 0;
    }
}

static void dpadUpDown(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.up = 1;
        if (prUi->appletmenu.visible == 1) {
            appletmenuCursorUp(&prUi->appletmenu);
            return;
        }
        switch(prUi->applet) {
            case 0:
                cheatpanelDpadUp(&prUi->cheatpanel);
                break;
            case 1:
                cheateditorDpadUp(&prUi->cheateditor);
                break;
            case 2:
                if (prUi->buttons.square == 1) {
                    disassemblerPageUp(&prUi->disassembler);
                } else {
                    disassemblerDpadUp(&prUi->disassembler);
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

static void dpadUpUp(CEFiveUi *prUi) {
    if (prUi != NULL) {
        prUi->buttons.up = 0;
    }
}

static void drawApplet(CEFiveUi *prUi) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;

    if (prUi == NULL) {
        return;
    }
    prCfg = prUi->prCEConfig;
    switch(prUi->applet) {
        case 0:
            cheatpanelRedraw(&prUi->cheatpanel);
            break;
        case 1:
            cheateditorRedraw(&prUi->cheateditor);
            break;
        case 2:
            if (prUi->drawn == 0) {
                prUi->disassembler.dirty = 1;
            }
            disassemblerRedraw(&prUi->disassembler);
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

static void drawAppletMenu(CEFiveUi *prUi) {
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

static void drawCheatStatus(CEFiveUi *prUi) {
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

static void drawTitlebar(CEFiveUi *prUi) {
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
    drawCheatStatus(prUi);
}

static void editSelectedCheat(CEFiveUi *prUi) {
    if (prUi == NULL) {
        return;
    }
    int index = cheatpanelGetSelectedIndex(&prUi->cheatpanel);
    cheateditorSelectCheat(&prUi->cheateditor, index);
    prUi->applet = 1;
    prUi->drawn = 0;
    prUi->cheateditor.dirty = 1;
}

