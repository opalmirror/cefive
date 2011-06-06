#include "dwordcolumn.h"

unsigned int dwordcolumn_commit(DwordColumn *prCol) {
    unsigned int value = 0;
    if (prCol != NULL) {
        value = prCol->value;
        dwordcolumn_set_editing(prCol, 0);
    }
    return value;
}

int dwordcolumn_decrement(DwordColumn* prCol) {
    unsigned int incr = 0;
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    unsigned int value = prCol->value;
    incr = dwordcolumn_get_increment(prCol);
    value -= incr;
    if (value < prCol->min) {
        value = prCol->min;
    }
    dwordcolumn_setvalue(prCol, value);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_edit(DwordColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    dwordcolumn_setvalue(prCol, value);
    dwordcolumn_set_editing(prCol, 1);
    dwordcolumn_set_digit(prCol, 0);
    return DWORDCOLUMN_SUCCESS;
}

ColorConfig* dwordcolumn_get_digitcolor(DwordColumn* prCol) {
    ColorConfig* prColor = NULL;
    if (prCol != NULL) {
        prColor = &prCol->editdigit;
    }
    return prColor;
}

ColorConfig* dwordcolumn_get_displaycolor(DwordColumn *prCol) {
    ColorConfig* prColor = NULL;
    if (prCol != NULL) {
        prColor = &prCol->color;
    }
    return prColor;
}

ColorConfig* dwordcolumn_get_editcolor(DwordColumn *prCol) {
    ColorConfig* prColor = NULL;
    if (prCol != NULL) {
        prColor = &prCol->edit;
    }
    return prColor;
}

unsigned int dwordcolumn_get_increment(DwordColumn *prCol) {
    unsigned int incr = 0;
    int digit = 0;
    if (prCol != NULL) {
        digit = prCol->digit;
        incr = prCol->increments[digit];
    }
    return incr;
}

int dwordcolumn_increment(DwordColumn* prCol) {
    unsigned int incr = 0;
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    unsigned int value = prCol->value;
    incr = dwordcolumn_get_increment(prCol);
    value += incr;
    if (value > prCol->max) {
        value = prCol->max;
    }
    dwordcolumn_setvalue(prCol, value);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_init(DwordColumn* prCol) {
    ColorConfig* prColor = NULL;
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prColor = dwordcolumn_get_displaycolor(prCol);
    colorconfig_setcolor(prColor, (u32)0xFF000000, (u32)0xFFFFFFFF);
    prColor = dwordcolumn_get_editcolor(prCol);
    colorconfig_setcolor(prColor, (u32)0xFFFFFFFF, (u32)0xFF000000);
    prColor = dwordcolumn_get_digitcolor(prCol);
    colorconfig_setcolor(prColor, (u32)0xFF000000, (u32)0xFFFFFFFF);
    dwordcolumn_set_digit(prCol, 0);
    dwordcolumn_set_increment(prCol, 0, 0x10000000);
    dwordcolumn_set_increment(prCol, 1, 0x01000000);
    dwordcolumn_set_increment(prCol, 2, 0x00100000);
    dwordcolumn_set_increment(prCol, 3, 0x00010000);
    dwordcolumn_set_increment(prCol, 4, 0x00001000);
    dwordcolumn_set_increment(prCol, 5, 0x00000100);
    dwordcolumn_set_increment(prCol, 6, 0x00000010);
    dwordcolumn_set_increment(prCol, 7, 0x00000001);
    dwordcolumn_set_editing(prCol, 0);
    dwordcolumn_set_max(prCol, 0xFFFFFFFF);
    dwordcolumn_set_min(prCol, 0x00000000);
    dwordcolumn_set_prefixed(prCol, 1);
    dwordcolumn_setvalue(prCol, prCol->min);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_invalidate(DwordColumn* prCol) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prCol->dirty = 1;
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_is_editing(DwordColumn* prCol) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    return (prCol->inedit != 0);
}

int dwordcolumn_is_prefixed(DwordColumn* prCol) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    return (prCol->prefix != 0);
}

int dwordcolumn_nextdigit(DwordColumn* prCol) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    int digit = prCol->digit;
    digit++;
    if (digit > 7) {
        digit = 7;
    }
    dwordcolumn_set_digit(prCol, digit);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_prevdigit(DwordColumn* prCol) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    int digit = prCol->digit;
    digit--;
    if (digit < 0) {
        digit = 0;
    }
    dwordcolumn_set_digit(prCol, digit);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_redraw(DwordColumn* prCol) {
    char buf[11];
    int i = 0;
    int pc = 0;
    int di = 0;
    u32 bg = 0;
    u32 fg = 0;

    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    if (prCol->dirty == 0) {
        return DWORDCOLUMN_SUCCESS;
    }
    if (prCol->prefix == 1) {
        sprintf(buf, "0x%08X", prCol->value);
        pc = 2;
    } else {
        sprintf(buf, "%08X", prCol->value);
    }
    if (!dwordcolumn_is_editing(prCol)) {
        pspDebugScreenSetBackColor(prCol->color.background);
        pspDebugScreenSetTextColor(prCol->color.text);
        pspDebugScreenPuts(buf);
        prCol->dirty = 0;
        return DWORDCOLUMN_SUCCESS;
    }
    if (dwordcolumn_is_prefixed(prCol)) {
        pspDebugScreenSetBackColor(prCol->edit.background);
        pspDebugScreenSetTextColor(prCol->edit.text);
        pspDebugScreenPuts("0x");
    }
    for (i = 0; i < 8; i++) {
        di = i + pc;
        bg = prCol->edit.background;
        fg = prCol->edit.text;
        if (prCol->digit == i) {
            bg = prCol->editdigit.background;
            fg = prCol->editdigit.text;
        }
        pspDebugScreenSetBackColor(bg);
        pspDebugScreenSetTextColor(fg);
        pspDebugScreenKprintf("%c", buf[di]);
    }
    prCol->dirty = 0;
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_set_digit(DwordColumn* prCol, int digit) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    if ((digit < 0) || (digit > 7)) {
        return DWORDCOLUMN_FAILURE;
    }
    prCol->digit = digit;
    dwordcolumn_invalidate(prCol);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_set_editing(DwordColumn* prCol, int editing) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prCol->inedit = (editing != 0);
    dwordcolumn_invalidate(prCol);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_set_increment(DwordColumn* prCol, int digit,
            unsigned int amount) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    if ((digit < 0) || (digit > 7)) {
        return DWORDCOLUMN_FAILURE;
    }
    prCol->increments[digit] = amount;
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_set_max(DwordColumn* prCol, unsigned int max) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prCol->max = max & 0xFFFFFFFF;
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_set_min(DwordColumn* prCol, unsigned int min) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prCol->min = min & 0xFFFFFFFF;
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_set_prefixed(DwordColumn* prCol, int prefixed) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prCol->prefix = (prefixed != 0);
    dwordcolumn_invalidate(prCol);
    return DWORDCOLUMN_SUCCESS;
}

int dwordcolumn_setvalue(DwordColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    if ((value < prCol->min) || (value > prCol->max)) {
        return DWORDCOLUMN_FAILURE;
    }
    if (value != prCol->value) {
        prCol->value = value;
        dwordcolumn_invalidate(prCol);
    }
    return DWORDCOLUMN_SUCCESS;
}
