#ifndef _DWORDEDITOR_H
#define _DWORDEDITOR_H

#include <pspkerneltypes.h>

typedef struct _DwordEditor {
    SceUInt32 value;
    int selected_digit;
    int editing;
    u32 background_color;
    u32 text_color;
    u32 edit_color;
    int increments[8];
} DwordEditor;

void dwordeditorCrossButton(DwordEditor *);
void dwordeditorDpadDown(DwordEditor *);
void dwordeditorDpadLeft(DwordEditor *);
void dwordeditorDpadRight(DwordEditor *);
void dwordeditorDpadUp(DwordEditor *);
void dwordeditorInit(DwordEditor *);
void dwordeditorRedraw(DwordEditor *);
int dwordeditorIncrementAmount(DwordEditor *);

#endif
/* vim:cin:et:ts=4 sw=4: */

