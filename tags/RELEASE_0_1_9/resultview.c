#include "resultview.h"

static int cursor_down(ResultView* prView);
static int cursor_left(ResultView* prView);
static int cursor_right(ResultView* prView);
static int cursor_up(ResultView* prView);
static int render_row(ResultView* prView, const int row);
static ColorConfig* row_color(ResultView* prView, const int row);
static int scroll_down(ResultView* prView, const int rows);
static int scroll_up(ResultView* prView, const int rows);

static int cursor_down(ResultView* prView) {
    CursorPos* prCursor = NULL;
    Dimension* prSize = NULL;
    int y = 0;
    int oldy = 0;
    
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prCursor = resultview_get_cursorpos(prView);
    prSize = resultview_get_size(prView);
    y = prCursor->y + 1;
    if (y >= prSize->height) {
        if (scroll_down(prView, 1) < 0) {
            return RESULTVIEW_FAILURE;
        }
        y = prSize->height - 1;
    }
    oldy = prCursor->y;
    prCursor->y = y;
    if (oldy != y) {
        if (render_row(prView, oldy) < 0) {
            return RESULTVIEW_FAILURE;
        }
        if (render_row(prView, prCursor->y) < 0) {
            return RESULTVIEW_FAILURE;
        }
    }

    return RESULTVIEW_SUCCESS;
}

static int cursor_left(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

static int cursor_right(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

static int cursor_up(ResultView* prView) {
    CursorPos* prCursor = NULL;
    Dimension* prSize = NULL;
    int y = 0;
    int oldy = 0;
    
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prCursor = resultview_get_cursorpos(prView);
    prSize = resultview_get_size(prView);
    y = prCursor->y - 1;
    if (y < 0) {
        if (scroll_up(prView, 1) < 0) {
            return RESULTVIEW_FAILURE;
        }
        y = 0;
    }
    oldy = prCursor->y;
    prCursor->y = y;
    if (oldy != y) {
        if (render_row(prView, oldy) < 0) {
            return RESULTVIEW_FAILURE;
        }
        if (render_row(prView, prCursor->y) < 0) {
            return RESULTVIEW_FAILURE;
        }
    }

    return RESULTVIEW_SUCCESS;
}

static int render_row(ResultView* prView, const int row) {
    ResultModel* prModel = NULL;
    ColorConfig* prColor = NULL;
    Dimension* prSize = NULL;
    CursorPos* prPos = NULL;
    Result* prResult = NULL;
    char sFmt[10];
    int index = 0;
    
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prColor = row_color(prView, row);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    prSize = resultview_get_size(prView);
    prPos = resultview_get_position(prView);
    pspDebugScreenSetXY(prPos->x, prPos->y + row);
    index = prView->pageIndex + row;
    prModel = resultview_get_model(prView);
    if ((prModel == NULL) || (prModel->count < index)) {
        sprintf(sFmt, "%%-%ds", prSize->width);
        pspDebugScreenKprintf(sFmt, "");
        return RESULTVIEW_SUCCESS;
    }
    prResult = resultmodel_get(prModel, index);
    if (prResult != NULL) {
        pspDebugScreenKprintf(" 0x%08X ", prResult->address);
        switch (prResult->resultType) {
            case SRT_UChar8:
                sprintf(sFmt, "0x%%02X%%-%ds", prSize->width - 16);
                pspDebugScreenKprintf(sFmt, prResult->value);
                break;
            case SRT_UShort16:
                sprintf(sFmt, "0x%%04X%%-%ds", prSize->width - 18);
                pspDebugScreenKprintf(sFmt, prResult->value);
                break;
            case SRT_None:
            case SRT_UInt32:
                sprintf(sFmt, "0x%%08X%%-%ds", prSize->width - 22);
                pspDebugScreenKprintf(sFmt, prResult->value);
                break;
            case SRT_String:
                sprintf(sFmt, "%%-%ds", prSize->width - 12);
                pspDebugScreenKprintf(sFmt, prResult->text);
                break;
            case SRT_Instruction:
                break;
        }
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_button_circle(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_button_cross(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_button_ltrigger(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_button_rtrigger(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_button_square(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_button_triangle(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_dpad_down(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    if (cursor_down(prView) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_dpad_left(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    if (cursor_left(prView) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_dpad_right(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    if (cursor_right(prView) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_dpad_up(ResultView* prView) {
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    if (cursor_up(prView) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

ColorConfig* resultview_get_cursorcolor(ResultView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = resultview_get_panelconfig(prView);
        return panelconfig_get_cursorcolor(prConfig);
    }
    return NULL;
}

CursorPos* resultview_get_cursorpos(ResultView* prView) {
    if (prView != NULL) {
        return &prView->cursorPos;
    }
    return NULL;
}

ResultModel* resultview_get_model(ResultView* prView) {
    if (prView != NULL) {
        return prView->resultModel;
    }
    return NULL;
}

ColorConfig* resultview_get_panelcolor(ResultView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = resultview_get_panelconfig(prView);
        return panelconfig_get_panelcolor(prConfig);
    }
    return NULL;    
}

PanelConfig* resultview_get_panelconfig(ResultView* prView) {
    if (prView != NULL) {
        return &prView->panelConfig;
    }
    return NULL;
}

CursorPos* resultview_get_position(ResultView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = resultview_get_panelconfig(prView);
        return panelconfig_get_position(prConfig);
    }
    return NULL;    
}

Dimension* resultview_get_size(ResultView* prView) {
    PanelConfig* prConfig = NULL;
    if (prView != NULL) {
        prConfig = resultview_get_panelconfig(prView);
        return panelconfig_get_size(prConfig);
    }
    return NULL;    
}

int resultview_init(ResultView* prView) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prConfig = resultview_get_panelconfig(prView);
    if (panelconfig_init(prConfig) < 0) {
        return RESULTVIEW_FAILURE;
    }
    prCursor = resultview_get_cursorpos(prView);
    if (cursorpos_init(prCursor) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_page_down(ResultView* prView) {
    Dimension* prSize = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prSize = resultview_get_size(prView);
    if (scroll_down(prView, prSize->height) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_page_up(ResultView* prView) {
    Dimension* prSize = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prSize = resultview_get_size(prView);
    if (scroll_up(prView, prSize->height) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_redraw(ResultView* prView) {
    Dimension* prSize = NULL;
    int i = 0;
    
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    if (prView->dirty == 0) {
        return RESULTVIEW_SUCCESS;
    }
    prSize = resultview_get_size(prView);
    for (i = 0; i < prSize->height; i++) {
        if (render_row(prView, i) < 0) {
            return RESULTVIEW_FAILURE;
        }
    }
    prView->dirty = 0;
    return RESULTVIEW_SUCCESS;
}

int resultview_set_cursorcolor(ResultView* prView, 
        const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prColor = resultview_get_cursorcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_set_cursorpos(ResultView* prView, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prCursor = resultview_get_cursorpos(prView);
    if (cursorpos_set(prCursor, x, y) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_set_panelcolor(ResultView* prView,
            const u32 background, const u32 text) {
    ColorConfig* prColor = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prColor = resultview_get_panelcolor(prView);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_set_position(ResultView* prView, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prCursor = resultview_get_position(prView);
    if (cursorpos_set(prCursor, x, y) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

int resultview_set_size(ResultView* prView, const int width, const int height) {
    Dimension* prSize = NULL;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prSize = resultview_get_size(prView);
    if (dimension_set(prSize, width, height) < 0) {
        return RESULTVIEW_FAILURE;
    }
    return RESULTVIEW_SUCCESS;
}

static ColorConfig* row_color(ResultView* prView, const int row) {
    ResultModel* prModel = NULL;
    CursorPos* prCursor = NULL;
    if (prView == NULL) {
        return NULL;
    }
    prModel = resultview_get_model(prView);
    prCursor = resultview_get_cursorpos(prView);
    /* If the specified row is the current cursor row */
    if (prCursor->y == row) {
        /* If there is a valid Result Model */
        if (prModel != NULL) {
            /* If the Model has at least one result */
            if (prModel->count > 0) {
                return resultview_get_cursorcolor(prView);
            }
        }
    }
    return resultview_get_panelcolor(prView);
}

static int scroll_down(ResultView* prView, const int rows) {
    Dimension* prSize = NULL;
    ResultModel* prModel = NULL;
    int y = 0;
    int maxy = 0;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    prModel = resultview_get_model(prView);
    if (prModel == NULL) {
        return RESULTVIEW_INVMODEL;
    }
    prSize = resultview_get_size(prView);
    maxy = prModel->count - prSize->height;
    /* If there are not enough results to scroll */
    if (maxy <= 0) {
        return RESULTVIEW_SUCCESS;
    }
    y = prView->pageIndex + rows;
    /* Don't scroll further than showing the last result. */
    if (y > maxy) {
        y = maxy;
    }
    prView->pageIndex = y;
    prView->dirty = 1;
    
    return RESULTVIEW_SUCCESS;
}

static int scroll_up(ResultView* prView, const int rows) {
    int y = 0;
    if (prView == NULL) {
        return RESULTVIEW_NULLPTR;
    }
    y = prView->pageIndex - rows;
    /* Don't scroll past the first row. */
    if (y < 0) {
        return RESULTVIEW_SUCCESS;
    }
    prView->pageIndex = y;
    prView->dirty = 1;
    return RESULTVIEW_SUCCESS;
}
