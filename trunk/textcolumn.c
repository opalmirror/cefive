#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "textcolumn.h"
#include "colorconfig.h"

ColorConfig* textcolumn_get_displaycolor(TextColumn* prCol) {
    if (prCol == NULL) {
        return NULL;
    }
    return &prCol->rColor;
}

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
    ColorConfig* prColor = NULL;
    ColorConfig* prCursor = NULL;
    int r = 0;

    if (prCol == NULL) {
        return TEXTCOLUMN_NULLPTR;
    }
    prColor = textcolumn_get_displaycolor(prCol);
    if (prColor == NULL) {
        return TEXTCOLUMN_FAILURE;
    }
    r = colorconfig_setcolor(prColor, TEXTCOLUMN_COLOR_BG, TEXTCOLUMN_COLOR_FG);
    if (r != COLORCONFIG_SUCCESS) {
        return TEXTCOLUMN_FAILURE;
    }
    /* TODO: Remove after compatibility confirmed. */
    prCol->prColor = prColor;

    prCursor = &prCol->rCursor;
    r = colorconfig_setcolor(prCursor, TEXTCOLUMN_CURSOR_BG, 
            TEXTCOLUMN_CURSOR_FG);
    if (r != COLORCONFIG_SUCCESS) {
        return TEXTCOLUMN_FAILURE;
    }
    /* TODO: Remove after compatibility confirmed. */
    prCol->prCursor = prCursor;
    
    prCol->text[0] = (char)0;
    textcolumn_set_columns(prCol, TEXTCOLUMN_MAX_LEN);
    prCol->justification = TCJ_Left;
    return TEXTCOLUMN_SUCCESS;
}

int textcolumn_invalidate(TextColumn* prCol) {
    if (prCol == NULL) {
        return TEXTCOLUMN_NULLPTR;
    }
    prCol->dirty = 1;
    return TEXTCOLUMN_SUCCESS;
}

int textcolumn_redraw(TextColumn* prCol) {
    ColorConfig* prColor = NULL;
    char fmt[7];
    if (prCol == NULL) {
        return TEXTCOLUMN_NULLPTR;
    }
    if (prCol->dirty == 0) {
        return TEXTCOLUMN_SUCCESS;
    }
    prColor = textcolumn_get_displaycolor(prCol);
    if (prCol->is_cursor != 0) {
        prColor = &prCol->rCursor;
    }
    textcolumn_getformat(prCol, fmt);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenKprintf(fmt, prCol->text);
    prCol->dirty = 0;

    return TEXTCOLUMN_SUCCESS;
}

int textcolumn_set_columns(TextColumn* prCol, int columns) {
    if (prCol == NULL) {
        return TEXTCOLUMN_NULLPTR;
    }
    if ((columns < 0) || (columns > TEXTCOLUMN_MAX_LEN)) {
        return TEXTCOLUMN_FAILURE;
    }
    prCol->columns = columns;
    textcolumn_invalidate(prCol);
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
    textcolumn_invalidate(prCol);
    return TEXTCOLUMN_SUCCESS;
}
