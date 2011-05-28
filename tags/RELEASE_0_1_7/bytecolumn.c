#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "bytecolumn.h"

unsigned int bytecolumn_commit(ByteColumn *prCol) {
    unsigned int value = 0;
    if (prCol != NULL) {
        value = prCol->value;
        prCol->inedit = 0;
    }
    return value;
}

void bytecolumn_decrement(ByteColumn* prCol) {
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

void bytecolumn_edit(ByteColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return;
    }
    prCol->value = value;
    prCol->inedit = 1;
    prCol->digit = 0;
}

void bytecolumn_increment(ByteColumn* prCol) {
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

void bytecolumn_init(ByteColumn* prCol) {
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
    prCol->increments[0] = 0x10;
    prCol->increments[1] = 0x01;
    prCol->inedit = 0;
    prCol->max = 0xFF;
    prCol->min = 0x00;
    prCol->prefix = 0;
    prCol->value = prCol->min;
}

void bytecolumn_nextdigit(ByteColumn* prCol) {
    if (prCol == NULL) {
        return;
    }
    int digit = prCol->digit;
    digit++;
    if (digit > 1) {
        digit = 1;
    }
    prCol->digit = digit;
}

void bytecolumn_prevdigit(ByteColumn* prCol) {
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

void bytecolumn_redraw(ByteColumn* prCol) {
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
        sprintf(buf, "0x%02X", prCol->value);
        pc = 2;
    } else {
        sprintf(buf, "%02X", prCol->value);
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
    for (i = 0; i < 2; i++) {
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
