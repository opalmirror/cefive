#include "dwordeditor.h"

void dwordeditorCrossButton(DwordEditor *prEditor) {
    if (prEditor == NULL) {
        return;
    }
    prEditor->editing = (prEditor->editing == 1) ? 0 : 1;
}

void dwordeditorDpadDown(DwordEditor *prEditor) {
    if (prEditor == NULL) {
        return;
    }
    if (prEditor->editing == 0) {
        return;
    }
    prEditor->value -= prEditor->increments[prEditor->selected_digit];
}

void dwordeditorDpadLeft(DwordEditor *prEditor) {
    if (prEditor == NULL) {
        return;
    }
    int x = prEditor->selected_digit;
    x--;
    if (x < 0) {
        x = 0;
    }
    prEditor->selected_digit = x;
}

void dwordeditorDpadRight(DwordEditor *prEditor) {
    if (prEditor == NULL) {
        return;
    }
    int x = prEditor->selected_digit;
    x++;
    if (x > 7) {
        x = 7;
    }
    prEditor->selected_digit = x;
}

void dwordeditorDpadUp(DwordEditor *prEditor) {
    if (prEditor == NULL) {
        return;
    }
    if (prEditor->editing == 0) {
        return;
    }
    prEditor->value += prEditor->increments[prEditor->selected_digit];
}

void dwordeditorInit(DwordEditor *prEditor) {
    if (prEditor == NULL) {
        return;
    }
    prEditor->value = 0;
    prEditor->selected_digit = 0;
    prEditor->editing = 0;
    prEditor->background_color = (u32)0xFF000000;
    prEditor->text_color = (u32)0xFFD0D0D0;
    prEditor->edit_color = (u32)0xFFD0D000;
    prEditor->increments[0] = 0x10000000;
    prEditor->increments[1] = 0x01000000;
    prEditor->increments[2] = 0x00100000;
    prEditor->increments[3] = 0x00010000;
    prEditor->increments[4] = 0x00001000;
    prEditor->increments[5] = 0x00000100;
    prEditor->increments[6] = 0x00000010;
    prEditor->increments[7] = 0x00000001;
}

void dwordeditorRedraw(DwordEditor *prEditor) {
    char dbuf[11];
    char sc[2];
    SceUInt32 digit;
    SceUInt32 digitmask;
    int sa;
    int i;

    if (prEditor == NULL) {
        return;
    }
    if (prEditor->editing == 0) {
        sprintf(dbuf, "0x%08X", prEditor->value);
        pspDebugScreenSetBackColor(prEditor->background_color);
        pspDebugScreenSetTextColor(prEditor->text_color);
        pspDebugScreenPuts(dbuf);
        return;
    }
    sprintf(dbuf, "0x%08X", prEditor->value);
    pspDebugScreenSetBackColor(prEditor->background_color);
    pspDebugScreenPuts("0x");
    for (i = 0; i < 8; i++) {
        if (i == prEditor->selected_digit) {
            pspDebugScreenSetTextColor(prEditor->edit_color);
        } else {
            pspDebugScreenSetTextColor(prEditor->text_color);
        }
        sc[0] = dbuf[i + 2];
        sc[1] = (char)0;
        pspDebugScreenPuts(sc);
    }
}

int dwordeditorIncrementAmount(DwordEditor *prEditor) {
    int amount = 0;
    if (prEditor != NULL) {
        amount = prEditor->increments[prEditor->selected_digit];
    }
    return amount;
}

/* vim:cin:et:ts=4 sw=4: */

