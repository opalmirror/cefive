#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "togglecolumn.h"
#include "colorconfig.h"

int togglecolumn_format(ToggleColumn* prCol, char* fmt) {
    if (prCol == NULL || fmt == NULL) {
        return TOGGLECOLUMN_MEMORY;
    }
    if (prCol->justify == ToCJ_Left) {
        sprintf(fmt, "%%-%ds", prCol->columns);
    } else {
        sprintf(fmt, "%%%ds", prCol->columns);
    }

    return TOGGLECOLUMN_SUCCESS;
}

int togglecolumn_init(ToggleColumn* prCol) {
    ColorConfig rColor;
    ColorConfig rCursor;
    if (prCol == NULL) {
        return TOGGLECOLUMN_MEMORY;
    }
    rColor.background = TOGGLECOLUMN_COLOR_BG;
    rColor.text = TOGGLECOLUMN_COLOR_FG;
    prCol->prColor = &rColor;
    rCursor.background = TOGGLECOLUMN_CURSOR_BG;
    rCursor.text = TOGGLECOLUMN_CURSOR_FG;
    prCol->prCursor = &rCursor;
    prCol->positive_label = TOGGLECOLUMN_POSITIVE;
    prCol->negative_label = TOGGLECOLUMN_NEGATIVE;
    prCol->is_cursor = 0;
    prCol->value = 0;
    prCol->columns = 8;
    prCol->justify = ToCJ_Left;

    return TOGGLECOLUMN_SUCCESS;
}

int togglecolumn_redraw(ToggleColumn* prCol) {
    char fmt[7];
    if (prCol == NULL) {
        return TOGGLECOLUMN_MEMORY;
    }
    u32 bg = prCol->prColor->background;
    u32 fg = prCol->prColor->text;
    if (prCol->is_cursor == 1) {
        bg = prCol->prCursor->background;
        fg = prCol->prCursor->text;
    }
    togglecolumn_format(prCol, fmt);
    char *val = NULL;
    if (prCol->value == 0) {
        val = prCol->negative_label;
    } else {
        val = prCol->positive_label;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenKprintf(fmt, val);

    return TOGGLECOLUMN_SUCCESS;
}

int togglecolumn_toggle(ToggleColumn* prCol) {
    if (prCol == NULL) {
        return TOGGLECOLUMN_MEMORY;
    }
    if (prCol->value == 0) {
        prCol->value = 1;
    } else {
        prCol->value = 0;
    }
    return TOGGLECOLUMN_SUCCESS;
}
