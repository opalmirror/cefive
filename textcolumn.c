#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "textcolumn.h"
#include "colorconfig.h"

int textcolumn_getformat(TextColumn* prCol, char *fmt) {
    int cols = 0;
    if (prCol == NULL) {
        return TEXTCOLUMN_MEMORY;
    }

    cols = prCol->columns;
    if (prCol->justification == TCJ_Left) {
        sprintf(fmt, "%s%ds", "%-", cols);
    } else {
        sprintf(fmt, "%s%ds", "%", cols);
    }
    return TEXTCOLUMN_SUCCESS;
}

int textcolumn_init(TextColumn* prCol) {
    ColorConfig rColor;
    ColorConfig rCursor;

    if (prCol == NULL) {
        return TEXTCOLUMN_MEMORY;
    }

    rColor.background = TEXTCOLUMN_COLOR_BG;
    rColor.text = TEXTCOLUMN_COLOR_FG;
    prCol->prColor = &rColor;
    rCursor.background = TEXTCOLUMN_CURSOR_BG;
    rCursor.text = TEXTCOLUMN_CURSOR_FG;
    prCol->prCursor = &rCursor;
    prCol->text[0] = (char)0;
    prCol->columns = TEXTCOLUMN_MAX_LEN;
    prCol->justification = TCJ_Left;
    return TEXTCOLUMN_SUCCESS;
}

int textcolumn_redraw(TextColumn* prCol) {
    char fmt[7];
    if (prCol == NULL) {
        return TEXTCOLUMN_MEMORY;
    }
    u32 bg = prCol->prColor->background;
    u32 fg = prCol->prColor->text;
    textcolumn_getformat(prCol, fmt);
    if (prCol->is_cursor == 1) {
        bg = prCol->prCursor->background;
        fg = prCol->prCursor->text;
    }
    pspDebugScreenSetBackColor(bg);
    pspDebugScreenSetTextColor(fg);
    pspDebugScreenKprintf(fmt, prCol->text);

    return TEXTCOLUMN_SUCCESS;
}

int textcolumn_settext(TextColumn* prCol, const char* text) {
    if (prCol == NULL) {
        return TEXTCOLUMN_MEMORY;
    }
    if (text == NULL) {
        prCol->text[0] = (char)0;
    } else {
        sprintf(prCol->text, "%s", text);
    }
    return TEXTCOLUMN_SUCCESS;
}
