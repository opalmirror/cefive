/* cheateditor.c
 *   Cheat Editor for NitePr G5
 * Author:
 *   Sir Gee of Five
 */

#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <string.h>
#include "cheateditor.h"
#include "cheat.h"
#include "block.h"
#include "dwordeditor.h"
#include "mips.h"
#include "cefiveconfig.h"
#include "colorconfig.h"

void cheateditorCrossButton(CheatEditor *prPanel) {
    Block *prBlock = NULL;
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        prBlock = getSelectedBlock(prPanel);
        if (prPanel->cursor.x == 0) {
            prPanel->rAddrEdit.value = prBlock->address;
            prPanel->rAddrEdit.selected_digit = 0;
            dwordeditorCrossButton(&prPanel->rAddrEdit);
        }
        if (prPanel->cursor.x == 1) {
            prPanel->rValEdit.value = prBlock->hakVal;
            prPanel->rValEdit.selected_digit = 0;
            dwordeditorCrossButton(&prPanel->rValEdit);
        }
        prPanel->editing = 1;
    } else {
        prBlock = getSelectedBlock(prPanel);
        if (prPanel->cursor.x == 0) {
            dwordeditorCrossButton(&prPanel->rAddrEdit);
            prBlock->address = prPanel->rAddrEdit.value;
        }
        if (prPanel->cursor.x == 1) {
            dwordeditorCrossButton(&prPanel->rValEdit);
            prBlock->hakVal = prPanel->rValEdit.value;
        }
        prPanel->editing = 0;
    }
    prPanel->dirty = 1;
}

void cheateditorCursorDown(CheatEditor *prPanel) {
    Cheat *prCheat;
    int y = 0;
    if (prPanel == NULL) {
        return;
    }
    y = prPanel->cursor.y;
    y++;
    prCheat = getSelectedCheat(prPanel);
    if (y >= prCheat->len) {
        y = prCheat->len - 1;
    }
    if (y >= prPanel->table_height) {
        cheateditorScrollDown(prPanel);
        y = prPanel->table_height - 1;
    }
    prPanel->cursor.y = y;
    prPanel->dirty = 1;
}

void cheateditorCursorLeft(CheatEditor *prPanel) {
    int x = 0;
    if (prPanel == NULL) {
        return;
    }
    x = prPanel->cursor.x;
    x--;
    if (x >= 0) {
        prPanel->cursor.x = x;
    }
    prPanel->dirty = 1;
}

void cheateditorCursorRight(CheatEditor *prPanel) {
    int x = 0;
    if (prPanel == NULL) {
        return;
    }
    x = prPanel->cursor.x;
    x++;
    if (x < 2) {
        prPanel->cursor.x = x;
    }
    prPanel->dirty = 1;
}

void cheateditorCursorUp(CheatEditor *prPanel) {
    int y = 0;
    if (prPanel == NULL) {
        return;
    }
    y = prPanel->cursor.y;
    y--;
    if (y <= 0) {
        cheateditorScrollUp(prPanel);
        y = 0;
    }
    prPanel->cursor.y = y;
    prPanel->dirty = 1;
}

void cheateditorDpadDown(CheatEditor *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        cheateditorCursorDown(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadDown(&prPanel->rAddrEdit);
        }
        if (prPanel->cursor.x == 1) {
            dwordeditorDpadDown(&prPanel->rValEdit);
        }
    }
    prPanel->dirty = 1;
}

void cheateditorDpadLeft(CheatEditor *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        cheateditorCursorLeft(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadLeft(&prPanel->rAddrEdit);
        }
        if (prPanel->cursor.x == 1) {
            dwordeditorDpadLeft(&prPanel->rValEdit);
        }
    }
    prPanel->dirty = 1;
}

void cheateditorDpadRight(CheatEditor *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        cheateditorCursorRight(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadRight(&prPanel->rAddrEdit);
        }
        if (prPanel->cursor.x == 1) {
            dwordeditorDpadRight(&prPanel->rValEdit);
        }
    }
    prPanel->dirty = 1;
}

void cheateditorDpadUp(CheatEditor *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->editing == 0) {
        cheateditorCursorUp(prPanel);
    } else {
        if (prPanel->cursor.x == 0) {
            dwordeditorDpadUp(&prPanel->rAddrEdit);
        }
        if (prPanel->cursor.x == 1) {
            dwordeditorDpadUp(&prPanel->rValEdit);
        }
    }
    prPanel->dirty = 1;
}

void cheateditorInit(CheatEditor *prPanel, Cheat *parCheat, 
        Block *parBlock) {
    if (prPanel == NULL) {
        return;
    }
    prPanel->prCeConfig = NULL;
    prPanel->prEngine = NULL;
    prPanel->cheat = parCheat;
    prPanel->block = parBlock;
    prPanel->cheat_count = 0;
    prPanel->selected_cheat = 0;
    prPanel->top_row = 0;
    prPanel->config.color.background = (u32)0xFF000000;
    prPanel->config.color.text = (u32)0xFFD0D0D0;
    prPanel->config.cursor.background = (u32)0xFF000080;
    prPanel->config.cursor.text = (u32)0xFFD0D000;
    prPanel->view_mode = CEVM_None;
    prPanel->table_height = 20;
    prPanel->table_top = 2;
    prPanel->table_y = 0;
    dwordeditorInit(&(prPanel->rAddrEdit));
    dwordeditorInit(&(prPanel->rValEdit));
    prPanel->rAddrEdit.increments[7] = 4;
    prPanel->dirty = 1;
}

void cheateditorRedraw(CheatEditor *prPanel) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    char lbuf[69];
    Cheat rCheat;
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->dirty == 0) {
        return;
    }
    prCfg = prPanel->prCeConfig;
    rCheat = prPanel->cheat[prPanel->selected_cheat];
    sprintf(lbuf, "Name: %s", rCheat.name);
    if (prCfg != NULL) {
        prColor = &prCfg->rAppletConfig.rPanel.rColor;
    } else {
        prColor = &prPanel->config.color;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(0, prPanel->top_row);
    pspDebugScreenPuts(lbuf);
    drawBlockTable(prPanel, &rCheat);
    prPanel->dirty = 0;
}

void cheateditorScrollDown(CheatEditor *prPanel) {
    Cheat *prCheat = NULL;
    if (prPanel == NULL) {
        return;
    }
    prCheat = getSelectedCheat(prPanel);
    if (prCheat == NULL) {
        return;
    }
    if (prCheat->len <= prPanel->table_height) {
        return;
    }
    if (prPanel->table_y + prPanel->table_height >= prCheat->len) {
        return;
    }
    prPanel->table_y++;
}

void cheateditorScrollUp(CheatEditor *prPanel) {
    if (prPanel == NULL) {
        return;
    }
    if (prPanel->table_y <= 0) {
        return;
    }
    prPanel->table_y--;
}

void cheateditorSelectCheat(CheatEditor *prPanel, int index) {
    if (prPanel == NULL) {
        return;
    }
    if (index != prPanel->selected_cheat) {
        prPanel->cursor.y = 0;
        prPanel->table_y = 0;
    }
    prPanel->selected_cheat = index;
}

static void drawBlankRow(CheatEditor *prPanel) {
    char lbuf[69];
    if (prPanel == NULL) {
        return;
    }
    sprintf(lbuf, "%67s", " ");
    pspDebugScreenPuts(lbuf);
}

static void drawBlockRow(CheatEditor *prPanel, Block *prBlock) {
    char lbuf[69];
    char assm[45];
    unsigned int vaddr = 0;
    if (prPanel == NULL || prBlock == NULL) {
        return;
    }
    assm[0] = (char)0;
    vaddr = prBlock->address & 0xBFFFFFFF;
    mipsDecode(assm, prBlock->hakVal, vaddr);
    sprintf(lbuf, "0x%08X 0x%08X %-45s", vaddr, prBlock->hakVal,
            assm);
    pspDebugScreenPuts(lbuf);
}

static void drawBlockTable(CheatEditor *prPanel, Cheat *prCheat) {
    int i;
    int ib;
    int tt;
    Block rBlock;
    
    if (prPanel == NULL || prCheat == NULL) {
        return;
    }
    tt = prPanel->top_row + 2;
    for (i = 0; i < prPanel->table_height; i++) {
        ib = prCheat->block + prPanel->table_y + i;
        pspDebugScreenSetXY(0, tt + i);
        if (i >= prCheat->len) {
            drawBlankRow(prPanel);
            continue;
        }
        rBlock = prPanel->block[ib];
        drawBlockRow(prPanel, &rBlock);
    }
    drawCursorRow(prPanel);
}

static void drawCursorRow(CheatEditor *prPanel) {
    CEFiveConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    int tt = 0;
    int x = 0;
    Block *sel = NULL;
    if (prPanel == NULL) {
        return;
    }
    prCfg = prPanel->prCeConfig;

    tt = prPanel->top_row + 2 + prPanel->cursor.y;
    pspDebugScreenSetXY(0, tt);
    sel = getSelectedBlock(prPanel);
    if (prCfg != NULL) {
        prColor = &prCfg->rAppletConfig.rPanel.rCursor;
    } else {
        prColor = &prPanel->config.cursor;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    drawBlockRow(prPanel, sel);
    x = prPanel->cursor.x;
    if (x == 0) {
        pspDebugScreenSetXY(0, tt);
        if (prPanel->editing == 0) {
            prPanel->rAddrEdit.value = sel->address;
        }
        dwordeditorRedraw(&prPanel->rAddrEdit);
    } else {
        pspDebugScreenSetXY(11, tt);
        if (prPanel->editing == 0) {
            prPanel->rValEdit.value = sel->hakVal;
        }
        dwordeditorRedraw(&prPanel->rValEdit);
    }
}

static Block* getSelectedBlock(CheatEditor *prPanel) {
    Block *sel = NULL;
    Cheat *cht = NULL;
    int idx = 0;
    int isi = 0;
    
    if (prPanel == NULL) {
        return sel;
    }
    cht = getSelectedCheat(prPanel);
    if (cht == NULL) {
        return sel;
    }
    idx = cht->block;
    isi = getSelectedIndex(prPanel);
    sel = &(prPanel->block[idx + isi]);
    return sel;
}

static Cheat* getSelectedCheat(CheatEditor *prPanel) {
    Cheat *sel = NULL;
    
    if (prPanel == NULL) {
        return sel;
    }
    sel = &(prPanel->cheat[prPanel->selected_cheat]);
    return sel;
}

static int getSelectedIndex(CheatEditor *prPanel) {
    int index = -1;
    if (prPanel == NULL) {
        return index;
    }
    index = prPanel->table_y + prPanel->cursor.y;
    return index;
}
