#include "blockview.h"

static int cursor_down(BlockView* prView);
static int cursor_left(BlockView* prView);
static int cursor_right(BlockView* prView);
static int cursor_up(BlockView* prView);
static int render_row(BlockView* prView, const int row);
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
    return BLOCKVIEW_SUCCESS;
}

int blockview_page_down(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_page_up(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return BLOCKVIEW_SUCCESS;
}

int blockview_redraw(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
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
    return BLOCKVIEW_SUCCESS;
}

static int cursor_right(BlockView* prView) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
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

static int render_row(BlockView* prView, const int row) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return BLOCKVIEW_SUCCESS;
}

static int scroll_down(BlockView* prView, const int rows) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return BLOCKVIEW_SUCCESS;
}

static int scroll_up(BlockView* prView, const int rows) {
    if (prView == NULL) {
        return BLOCKVIEW_NULLPTR;
    }
    return BLOCKVIEW_SUCCESS;
}
