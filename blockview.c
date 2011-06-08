#include "blockview.h"

static int commit_edit(BlockView* prView);
static int cursorcolor(BlockView* prView);
static int cursor_down(BlockView* prView);
static int cursor_left(BlockView* prView);
static int cursor_right(BlockView* prView);
static int cursor_up(BlockView* prView);
static Block* get_block(BlockView* prView, const int row);
static int panelcolor(BlockView* prView);
static int render_address_col(BlockView* prView, const int row);
static int render_comment_col(BlockView* prView, const int row);
static int render_row(BlockView* prView, const int row);
static int render_value_col(BlockView* prView, const int row);
static int row_index(BlockView* prView, const int row);
static int scroll_down(BlockView* prView, const int rows);
static int scroll_up(BlockView* prView, const int rows);

int blockview_button_circle(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_button_circle(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_button_cross(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_button_cross(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_button_ltrigger(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_prev_digit(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_button_rtrigger(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_next_digit(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_button_square(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_button_square(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_button_triangle(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_button_triangle(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_dpad_down(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_cursor_down(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    if (cursor_down(prView) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_dpad_left(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_cursor_left(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    if (cursor_left(prView) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_dpad_right(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_cursor_right(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    if (cursor_right(prView) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_dpad_up(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->hexPad.visible == 1) {
        if (hexpad_cursor_up(&prView->hexPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        return BLOCKVIEW_SUCCESS;
    }
    if (cursor_up(prView) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_init(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (panelconfig_init(&prView->panelConfig) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    if (cursorpos_init(&prView->cursorPos) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    if (hexpad_init(&prView->hexPad) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    prView->pageIndex = 0;
    prView->blockIndex = 0;
    prView->blockCount = 0;
    prView->blockList = NULL;
    prView->editing = 0;
    prView->dirty = 1;
    prView->panelConfig.rColor.background = BLOCKVIEW_PANELBG;
    prView->panelConfig.rColor.text = BLOCKVIEW_PANELFG;
    prView->panelConfig.rCursor.background = BLOCKVIEW_CURSORBG;
    prView->panelConfig.rCursor.text = BLOCKVIEW_CURSORFG;
    prView->panelConfig.rTop.x = BLOCKVIEW_POS_X;
    prView->panelConfig.rTop.y = BLOCKVIEW_POS_Y;
    prView->panelConfig.rSize.width = BLOCKVIEW_SIZE_W;
    prView->panelConfig.rSize.height = BLOCKVIEW_SIZE_H;
    return BLOCKVIEW_SUCCESS;
}

int blockview_page_down(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return scroll_down(prView, prView->panelConfig.rSize.height);
}

int blockview_page_up(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return scroll_up(prView, prView->panelConfig.rSize.height);
}

int blockview_redraw(BlockView* prView) {
    int i = 0;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->editing == 1) {
        if (prView->hexPad.visible == 1) {
            if (hexpad_redraw(&prView->hexPad) < 0) {
                return BLOCKVIEW_FAILURE;
            }
            return BLOCKVIEW_SUCCESS;
        }
        if (commit_edit(prView) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    if (prView->dirty == 0) {
        return BLOCKVIEW_SUCCESS;
    }
    for (i = 0; i < prView->panelConfig.rSize.height; i++) {
        if (render_row(prView, i) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    prView->dirty = 0;
    return BLOCKVIEW_SUCCESS;
}

int blockview_set_cursorcolor(BlockView* prView, const u32 background,
        const u32 text) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prView->panelConfig.rCursor.background = background;
    prView->panelConfig.rCursor.text = text;
    return BLOCKVIEW_SUCCESS;
}

int blockview_set_cursorpos(BlockView* prView, const int x, const int y) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prView->cursorPos.x = x;
    prView->cursorPos.y = y;
    return BLOCKVIEW_SUCCESS;
}

int blockview_set_panelcolor(BlockView* prView, const u32 background,
        const u32 text) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prView->panelConfig.rColor.background = background;
    prView->panelConfig.rColor.text = text;
    return BLOCKVIEW_SUCCESS;
}

int blockview_set_position(BlockView* prView, const int x, const int y) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prView->panelConfig.rTop.x = x;
    prView->panelConfig.rTop.y = y;
    return BLOCKVIEW_SUCCESS;
}

int blockview_set_size(BlockView* prView, const int width,
        const int height) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prView->panelConfig.rSize.width = width;
    prView->panelConfig.rSize.height = height;
    return BLOCKVIEW_SUCCESS;
}

static int commit_edit(BlockView* prView) {
    HexPad* prPad = NULL;
    Block* prBlock = NULL;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prPad = &prView->hexPad;
    if (prPad->cancelled == 1) {
        if (hexpad_reset(prPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        prView->editing = 0;
        prView->dirty = 1;
        return BLOCKVIEW_SUCCESS;
    }
    prBlock = get_block(prView, prView->cursorPos.y);
    if (prBlock == NULL) {
        return BLOCKVIEW_FAILURE;
    }
    if (prView->cursorPos.x == 0) {
        prBlock->address = hexpad_get_value(prPad);
        if (hexpad_reset(prPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    } else {
        prBlock->hakVal = hexpad_get_value(prPad);
        if (hexpad_reset(prPad) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    prView->editing = 0;
    prView->dirty = 1;
    return BLOCKVIEW_SUCCESS;
}

static int cursor_down(BlockView* prView) {
    int y = 0;
    int oldy = 0;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    y = prView->cursorPos.y + 1;
    if (y >= prView->blockCount) {
        y = prView->blockCount - 1;
    } else {
        if (y >= prView->panelConfig.rSize.height) {
            if (scroll_down(prView, 1) < 0) {
                return BLOCKVIEW_FAILURE;
            }
            y = prView->panelConfig.rSize.height - 1;
        }
    }
    oldy = prView->cursorPos.y;
    prView->cursorPos.y = y;
    if (oldy != y) {
        if (render_row(prView, oldy) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        if (render_row(prView, prView->cursorPos.y) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    return BLOCKVIEW_SUCCESS;
}

static int cursor_left(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->cursorPos.x > 0) {
        prView->cursorPos.x--;
        if (render_row(prView, prView->cursorPos.y) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    return BLOCKVIEW_SUCCESS;
}

static int cursor_right(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->cursorPos.x < 1) {
        prView->cursorPos.x++;
        if (render_row(prView, prView->cursorPos.y) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    return BLOCKVIEW_SUCCESS;
}

static int cursor_up(BlockView* prView) {
    int y = 0;
    int oldy = 0;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    y = prView->cursorPos.y - 1;
    if (y < 0) {
        if (scroll_up(prView, 1) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        y = 0;
    }
    oldy = prView->cursorPos.y;
    prView->cursorPos.y = y;
    if (oldy != y) {
        if (render_row(prView, oldy) < 0) {
            return BLOCKVIEW_FAILURE;
        }
        if (render_row(prView, prView->cursorPos.y) < 0) {
            return BLOCKVIEW_FAILURE;
        }
    }
    return BLOCKVIEW_SUCCESS;
}

static int cursorcolor(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    pspDebugScreenSetBackColor(prView->panelConfig.rCursor.background);
    pspDebugScreenSetTextColor(prView->panelConfig.rCursor.text);
    return BLOCKVIEW_SUCCESS;
}

static Block* get_block(BlockView* prView, const int row) {
    int index = 0;
    if (prView == NULL) {
        return NULL;
    }
    if ((row < 0) || (row >= prView->panelConfig.rSize.height)) {
        return NULL;
    }
    index = prView->blockIndex + prView->pageIndex + row;
    if ((index < prView->blockIndex) || 
            (index >= prView->blockIndex + prView->blockCount)) {
        return NULL;
    }
    if (prView->blockList == NULL) {
        return NULL;
    }
    return &prView->blockList[index];
}

static int panelcolor(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    pspDebugScreenSetBackColor(prView->panelConfig.rColor.background);
    pspDebugScreenSetTextColor(prView->panelConfig.rColor.text);
    return BLOCKVIEW_SUCCESS;
}

static int render_address_col(BlockView* prView, const int row) {
    Block* prBlock = NULL;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    prBlock = get_block(prView, row);
    if (prBlock == NULL) {
        pspDebugScreenKprintf("%-10s", "");
        return BLOCKVIEW_SUCCESS;
    }
    if ((prView->cursorPos.y == row) && (prView->cursorPos.x == 0)) {
        cursorcolor(prView);
    } else {
        panelcolor(prView);
    }
    pspDebugScreenKprintf("0x%08X", prBlock->address);
    return BLOCKVIEW_SUCCESS;
}

static int render_comment_col(BlockView* prView, const int row) {
    Block* prBlock = NULL;
    char sFmt[10];
    char sComment[71];
    int cw = 0;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    panelcolor(prView);
    prBlock = get_block(prView, row);
    cw = prView->panelConfig.rSize.width - 22;
    sprintf(sFmt, "%%-%ds", cw);
    if (prBlock == NULL) {
        pspDebugScreenKprintf(sFmt, "");
        return BLOCKVIEW_SUCCESS;
    }
    if (block_is_dword(prBlock)) {
        mipsDecode(sComment, prBlock->hakVal, prBlock->address);
        pspDebugScreenKprintf(sFmt, sComment);
        return BLOCKVIEW_SUCCESS;
    }
    pspDebugScreenKprintf(sFmt, "");
    return BLOCKVIEW_SUCCESS;
}

static int render_row(BlockView* prView, const int row) {
    int x = 0;
    int y = 0;
    int index = 0;
    char sFmt[10];
    
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if ((row < 0) || (row >= prView->panelConfig.rSize.height)) {
        return BLOCKVIEW_INVIDX;
    }
    x = prView->panelConfig.rTop.x;
    y = prView->panelConfig.rTop.y + row;
    index = row_index(prView, row);
    pspDebugScreenSetXY(x, y);
    panelcolor(prView);
    if (index >= prView->blockCount) {
        sprintf(sFmt, "%%-%ds", prView->panelConfig.rSize.width);
        pspDebugScreenKprintf(sFmt, "");
        return BLOCKVIEW_SUCCESS;
    }
    if (render_address_col(prView, row) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    panelcolor(prView);
    pspDebugScreenPuts(" ");
    if (render_value_col(prView, row) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    panelcolor(prView);
    pspDebugScreenPuts(" ");
    if (render_comment_col(prView, row) < 0) {
        return BLOCKVIEW_FAILURE;
    }
    return BLOCKVIEW_SUCCESS;
}

static int render_value_col(BlockView* prView, const int row) {
    Block* prBlock = NULL;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if ((row == prView->cursorPos.y) && (prView->cursorPos.x == 1)) {
        cursorcolor(prView);
    } else {
        panelcolor(prView);
    }
    prBlock = get_block(prView, row);
    if (prBlock == NULL) {
        pspDebugScreenKprintf("%-10s", "");
        return BLOCKVIEW_SUCCESS;
    }
    if (block_is_byte(prBlock)) {
        pspDebugScreenKprintf("      0x%02X", (prBlock->hakVal & 0xFF));
        return BLOCKVIEW_SUCCESS;
    }
    if (block_is_word(prBlock)) {
        pspDebugScreenKprintf("    0x%04X", (prBlock->hakVal & 0xFFFF));
        return BLOCKVIEW_SUCCESS;
    }
    if (block_is_dword(prBlock)) {
        pspDebugScreenKprintf("0x%08X", prBlock->hakVal);
    }
    return BLOCKVIEW_SUCCESS;
}

static int row_index(BlockView* prView, const int row) {
    int index = -1;
    if (prView != NULL) {
        index = prView->pageIndex + row;
    }
    return index;
}

static int scroll_down(BlockView* prView, const int rows) {
    int li = 0;
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    /* If there are not enough blocks to scroll, don't. */
    if (prView->blockCount < prView->panelConfig.rSize.height) {
        return BLOCKVIEW_SUCCESS;
    }
    li = prView->blockCount - prView->panelConfig.rSize.height;
    if (prView->pageIndex + rows >= li) {
        prView->pageIndex = li;
    } else {
        prView->pageIndex += rows;
    }
    prView->dirty = 1;
    return BLOCKVIEW_SUCCESS;
}

static int scroll_up(BlockView* prView, const int rows) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    if (prView->pageIndex > rows) {
        prView->pageIndex -= rows;
        prView->dirty = 1;
    }
    return BLOCKVIEW_SUCCESS;
}
