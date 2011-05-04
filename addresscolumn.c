#include <psptypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "colorconfig.h"
#include "addresscolumn.h"

unsigned int addresscolumn_commit(AddressColumn *prCol) {
    unsigned int value = 0;
    if (prCol != NULL) {
        value = prCol->value;
        addresscolumn_setediting(prCol, 0);
    }
    addresscolumn_invalidate(prCol);
    return value;
}

int addresscolumn_decrement(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    unsigned int value = prCol->value;
    int incr = addresscolumn_getincrement(prCol);
    value -= incr;
    if (value < prCol->min) {
        value = prCol->min;
    }
    addresscolumn_setvalue(prCol, value);
    addresscolumn_invalidate(prCol);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_edit(AddressColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    addresscolumn_setvalue(prCol, value);
    addresscolumn_setediting(prCol, 1);
    addresscolumn_setdigit(prCol, 0);
    addresscolumn_invalidate(prCol);
    return ADDRESSCOLUMN_SUCCESS;
}

ColorConfig* addresscolumn_get_digitcolor(AddressColumn* prCol) {
    ColorConfig* prCfg = NULL;
    if (prCol != NULL) {
        prCfg = &prCol->editdigit;
    }
    return prCfg;
}

ColorConfig* addresscolumn_get_displaycolor(AddressColumn* prCol) {
    ColorConfig* prCfg = NULL;
    if (prCol != NULL) {
        prCfg = &prCol->color;
    }
    return prCfg;
}

SceUInt32 addresscolumn_get_displaymask(AddressColumn* prCol) {
    if (prCol == NULL) {
        return 0;
    }
    return prCol->uiDispMask;
}

ColorConfig* addresscolumn_get_editcolor(AddressColumn* prCol) {
    ColorConfig* prCfg = NULL;
    if (prCol != NULL) {
        prCfg = &prCol->edit;
    }
    return prCfg;
}

int addresscolumn_getincrement(AddressColumn* prCol) {
    int incr = 0;
    if (prCol != NULL) {
        incr = prCol->increments[prCol->digit];
    }
    return incr;
}

int addresscolumn_increment(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    unsigned int value = prCol->value;
    int incr = addresscolumn_getincrement(prCol);
    value += incr;
    if (value > prCol->max) {
        value = prCol->max;
    }
    addresscolumn_setvalue(prCol, value);
    addresscolumn_invalidate(prCol);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_init(AddressColumn* prCol) {
    ColorConfig* prColor = NULL;
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    prColor = addresscolumn_get_displaycolor(prCol);
    colorconfig_setcolor(prColor, (u32)0xFF000000, (u32)0xFFFFFFFF);
    prColor = addresscolumn_get_editcolor(prCol);
    colorconfig_setcolor(prColor, (u32)0xFFFFFFFF, (u32)0xFF000000);
    prColor = addresscolumn_get_digitcolor(prCol);
    colorconfig_setcolor(prColor, (u32)0xFF000000, (u32)0xFFFFFFFF);
    addresscolumn_setdigit(prCol, 0);
    addresscolumn_setincrement(prCol, 0, 0x10000000);
    addresscolumn_setincrement(prCol, 1, 0x01000000);
    addresscolumn_setincrement(prCol, 2, 0x00100000);
    addresscolumn_setincrement(prCol, 3, 0x00010000);
    addresscolumn_setincrement(prCol, 4, 0x00001000);
    addresscolumn_setincrement(prCol, 5, 0x00000100);
    addresscolumn_setincrement(prCol, 6, 0x00000010);
    addresscolumn_setincrement(prCol, 7, 0x00000001);
    addresscolumn_setediting(prCol, 0);
    addresscolumn_setmax(prCol, 0x09FFFFFF);
    addresscolumn_setmin(prCol, 0x00000000);
    addresscolumn_setprefixed(prCol, 1);
    addresscolumn_setvalue(prCol, prCol->min);
    addresscolumn_invalidate(prCol);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_invalidate(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    prCol->dirty = 1;
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_is_editing(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    return (prCol->inedit != 0);
}

int addresscolumn_is_prefixed(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    return (prCol->prefix != 0);
}

int addresscolumn_nextdigit(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    int digit = prCol->digit;
    digit++;
    if (digit > 7) {
        digit = 7;
    }
    addresscolumn_setdigit(prCol, digit);
    addresscolumn_invalidate(prCol);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_prevdigit(AddressColumn* prCol) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    int digit = prCol->digit;
    digit--;
    if (digit < 0) {
        digit = 0;
    }
    addresscolumn_setdigit(prCol, digit);
    addresscolumn_invalidate(prCol);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_redraw(AddressColumn* prCol) {
    char buf[11];
    int i = 0;
    int pc = 0;
    int di = 0;
    u32 bg = 0;
    u32 fg = 0;
    SceUInt32 vaddr = 0;
    SceUInt32 dmask = 0;

    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    dmask = addresscolumn_get_displaymask(prCol);
    if (dmask == 0) {
        vaddr = prCol->value;
    } else {
        vaddr = prCol->value & dmask;
    }
    if (addresscolumn_is_prefixed(prCol)) {
        sprintf(buf, "0x%08X", vaddr);
        pc = 2;
    } else {
        sprintf(buf, "%08X", vaddr);
    }
    if (!addresscolumn_is_editing(prCol)) {
        pspDebugScreenSetBackColor(prCol->color.background);
        pspDebugScreenSetTextColor(prCol->color.text);
        pspDebugScreenPuts(buf);
        return ADDRESSCOLUMN_SUCCESS;
    }
    if (addresscolumn_is_prefixed(prCol)) {
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
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setdigit(AddressColumn* prCol, int digit){
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    if ((digit < 0) || (digit > 7)) {
        return ADDRESSCOLUMN_FAILURE;
    }
    prCol->digit = digit;
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_set_displaymask(AddressColumn* prCol, SceUInt32 mask) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    if (mask != prCol->uiDispMask) {
        addresscolumn_invalidate(prCol);
    }
    prCol->uiDispMask = mask;
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setediting(AddressColumn* prCol, int editing) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    prCol->inedit = (editing != 0);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setincrement(AddressColumn* prCol, int digit, 
        unsigned int amt) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    if ((digit < 0) || (digit > 7)) {
        return ADDRESSCOLUMN_FAILURE;
    }
    prCol->increments[digit] = amt;
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setmax(AddressColumn* prCol, unsigned int max) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    prCol->max = max;
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setmin(AddressColumn* prCol, unsigned int min) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    prCol->min = min;
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setprefixed(AddressColumn* prCol, int prefixed) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    prCol->prefix = (prefixed != 0);
    return ADDRESSCOLUMN_SUCCESS;
}

int addresscolumn_setvalue(AddressColumn* prCol, unsigned int value) {
    if (prCol == NULL) {
        return ADDRESSCOLUMN_NULLPTR;
    }
    if ((value > prCol->max) || (value < prCol->min)) {
        return ADDRESSCOLUMN_FAILURE;
    }
    if (value != prCol->value) {
        addresscolumn_invalidate(prCol);
    }
    prCol->value = value;
    return ADDRESSCOLUMN_SUCCESS;
}
