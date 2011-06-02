#include "asciicolumn.h"

char asciicolumn_commit(AsciiColumn* prCol) {
    char value = (char)0;
    if (prCol != NULL) {
        value = prCol->value;
        prCol->in_edit = 0;
    }
    return value;
}

int asciicolumn_crossbutton(AsciiColumn* prCol) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    if (prCol->in_edit == 0) {
        prCol->in_edit = 1;
    } else {
        prCol->in_edit = 0;
    }
    prCol->dirty = 1;

    return ASCIICOLUMN_SUCCESS;
}

int asciicolumn_cursor_down(AsciiColumn* prCol) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    if (prCol->in_edit == 0) {
        return ASCIICOLUMN_SUCCESS;
    }
    char val = prCol->value;
    if (val == (char)0) {
        val = (char)0xFF;
    } else {
        val --;
    }
    prCol->value = val;
    prCol->dirty = 1;

    return ASCIICOLUMN_SUCCESS;
}

int asciicolumn_cursor_up(AsciiColumn* prCol) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    if (prCol->in_edit == 0) {
        return ASCIICOLUMN_SUCCESS;
    }
    char val = prCol->value;
    if (val == (char)0xFF) {
        val = (char)0;
    } else {
        val++;
    }
    prCol->value = val;
    prCol->dirty = 1;

    return ASCIICOLUMN_SUCCESS;
}

int asciicolumn_edit(AsciiColumn* prCol, char value) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    prCol->value = value;
    prCol->in_edit = 1;
    prCol->dirty = 1;

    return ASCIICOLUMN_SUCCESS;
}

int asciicolumn_init(AsciiColumn* prCol) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    prCol->color.background = ASCIICOLUMN_COLOR_BG;
    prCol->color.text = ASCIICOLUMN_COLOR_FG;
    prCol->edit.background = ASCIICOLUMN_COLOR_FG;
    prCol->edit.text = ASCIICOLUMN_COLOR_BG;
    prCol->in_edit = 0;
    prCol->value = (char)0;
    prCol->dirty = 1;

    return ASCIICOLUMN_SUCCESS;
}

int asciicolumn_redraw(AsciiColumn* prCol) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    ColorConfig* prColor = &prCol->color;
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    if (prCol->value >= ' ') {
        pspDebugScreenKprintf("%c", prCol->value);
    } else {
        pspDebugScreenPuts(".");
    }
    prCol->dirty = 0;
    return ASCIICOLUMN_SUCCESS;
}

int asciicolumn_setvalue(AsciiColumn* prCol, char value) {
    if (prCol == NULL) {
        return ASCIICOLUMN_MEMORY;
    }
    if (value != prCol->value) {
        prCol->value = value;
        prCol->dirty = 1;
    }

    return ASCIICOLUMN_SUCCESS;
}
