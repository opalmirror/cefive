#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <string.h>
#include "byteeditor.h"

unsigned char byteeditor_commit(ByteEditor* prEd) {
    unsigned char value = 0;
    if (prEd != NULL) {
        value = prEd->value;
        prEd->editing = 0;
    }
    return value;
}

void byteeditor_decrement(ByteEditor* prEd) {
    if (prEd == NULL) {
        return;
    }
    unsigned char val = prEd->value;
    int digit = prEd->digit;
    int decamt = prEd->config.increments[digit];
    val -= decamt;
    if (val < 0) {
        val = 0;
    }
    prEd->value = val;
}

void byteeditor_edit(ByteEditor* prEd, unsigned char value) {
    if (prEd == NULL) {
        return;
    }
    prEd->value = value;
    prEd->editing = 1;
    prEd->digit = 0;
}

void byteeditor_increment(ByteEditor* prEd) {
    if (prEd == NULL) {
        return;
    }
    unsigned char val = prEd->value;
    int digit = prEd->digit;
    int decamt = prEd->config.increments[digit];
    val += decamt;
    if (val > 0xFF) {
        val = 0xFF;
    }
    prEd->value = val;
}

int byteeditor_init(ByteEditor* prEd) {
    if (prEd == NULL) {
        return BYTEEDITOR_MEMORY;
    }
    ByteEditorConfig* prCfg = &prEd->config;
    prCfg->color.background = BYTEEDITOR_DEFCOLORBG;
    prCfg->color.text = BYTEEDITOR_DEFCOLORFG;
    prCfg->edit.background = BYTEEDITOR_DEFEDITBG;
    prCfg->edit.text = BYTEEDITOR_DEFEDITFG;
    prCfg->increments[0] = 0x10;
    prCfg->increments[1] = 0x01;
    prEd->digit = 0;
    prEd->editing = 0;
    prEd->value = 0;

    return BYTEEDITOR_SUCCESS;
}

void byteeditor_nextdigit(ByteEditor *prEd) {
    if (prEd == NULL) {
        return;
    }
    if (prEd->digit == 1) {
        return;
    }
    prEd->digit++;
}

void byteeditor_prevdigit(ByteEditor *prEd) {
    if (prEd == NULL) {
        return;
    }
    if (prEd->digit == 0) {
        return;
    }
    prEd->digit--;
}

int byteeditor_redraw(ByteEditor* prEd) {
    char buf[3];
    if (prEd == NULL) {
        return BYTEEDITOR_MEMORY;
    }
    sprintf(buf, "%02X", prEd->value);
    u32 bg = prEd->config.color.background;
    u32 fg = prEd->config.color.text;
    if (prEd->editing == 1) {
        if (prEd->digit == 0) {
            bg = prEd->config.edit.background;
            fg = prEd->config.edit.text;
        }
        pspDebugScreenSetBackColor(bg);
        pspDebugScreenSetTextColor(fg);
        pspDebugScreenKprintf("%c", buf[0]);
        bg = prEd->config.color.background;
        fg = prEd->config.color.text;
        if (prEd->digit == 1) {
            bg = prEd->config.edit.background;
            fg = prEd->config.edit.text;
        }
        pspDebugScreenSetBackColor(bg);
        pspDebugScreenSetTextColor(fg);
        pspDebugScreenKprintf("%c", buf[1]);
    } else {
        pspDebugScreenSetBackColor(bg);
        pspDebugScreenSetTextColor(fg);
        pspDebugScreenPuts(buf);
    }

    return BYTEEDITOR_SUCCESS;
}
