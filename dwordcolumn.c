#include <pspkerneltypes.h>
#include <pspdebug.h>
#include "dwordcolumn.h"

unsigned int dwordcolumn_commit(DwordColumn *prCol) {
    unsigned int value = 0;
    if (prCol != NULL) {
        value = prCol->value;
        prCol->inedit = 0;
    }
    return value;
}

void dwordcolumn_decrement(DwordColumn* prCol) {
    if (prCol == NULL) {
        return;
    }
    unsigned int value = prCol->value;
    int digit = prCol->digit;
    int incr = prCol->increments[digit];
    value -= incr;
    if (value < prCol->min) {
        value = prCol->min;
    }
    prCol->value = value;
}

void dwordcolumn_edit(DwordColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return;
    }
    prCol->value = value;
    prCol->inedit = 1;
    prCol->digit = 0;
}

void dwordcolumn_increment(DwordColumn* prCol) {
    if (prCol == NULL) {
        return;
    }
    unsigned int value = prCol->value;
    int digit = prCol->digit;
    int incr = prCol->increments[digit];
    value += incr;
    if (value > prCol->max) {
        value = prCol->max;
    }
    prCol->value = value;
}

void dwordcolumn_init(DwordColumn* prCol) {
    if (prCol == NULL) {
        return;
    }
    prCol->color.background = (u32)0xFF000000;
    prCol->color.text = (u32)0xFFFFFFFF;
    prCol->edit.background = (u32)0xFFFFFFFF;
    prCol->edit.text = (u32)0xFF000000;
    prCol->editdigit.background = (u32)0xFF000000;
    prCol->editdigit.text = (u32)0xFFFFFFFF;
    prCol->digit = 0;
    prCol->increments[0] = 0x10000000;
    prCol->increments[1] = 0x01000000;
    prCol->increments[2] = 0x00100000;
    prCol->increments[3] = 0x00010000;
    prCol->increments[4] = 0x00001000;
    prCol->increments[5] = 0x00000100;
    prCol->increments[6] = 0x00000010;
    prCol->increments[7] = 0x00000001;
    prCol->inedit = 0;
    prCol->max = 0xFFFFFFFF;
    prCol->min = 0x00000000;
    prCol->prefix = 1;
    prCol->value = prCol->min;
}

void dwordcolumn_nextdigit(DwordColumn* prCol) {
    if (prCol == NULL) {
        return;
    }
    int digit = prCol->digit;
    digit++;
    if (digit > 7) {
        digit = 7;
    }
    prCol->digit = digit;
}

void dwordcolumn_prevdigit(DwordColumn* prCol) {
    if (prCol == NULL) {
        return;
    }
    int digit = prCol->digit;
    digit--;
    if (digit < 0) {
        digit = 0;
    }
    prCol->digit = digit;
}

void dwordcolumn_redraw(DwordColumn* prCol) {
    char buf[11];
    int i = 0;
    int pc = 0;
    int di = 0;
    u32 bg = 0;
    u32 fg = 0;

    if (prCol == NULL) {
        return;
    }
    if (prCol->prefix == 1) {
        sprintf(buf, "0x%08X", prCol->value);
        pc = 2;
    } else {
        sprintf(buf, "%08X", prCol->value);
    }
    if (prCol->inedit == 0) {
        pspDebugScreenSetBackColor(prCol->color.background);
        pspDebugScreenSetTextColor(prCol->color.text);
        pspDebugScreenPuts(buf);
        return;
    }
    if (prCol->prefix == 1) {
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
}

int dwordcolumn_setvalue(DwordColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return DWORDCOLUMN_NULLPTR;
    }
    prCol->value = value;
    return DWORDCOLUMN_SUCCESS;
}
