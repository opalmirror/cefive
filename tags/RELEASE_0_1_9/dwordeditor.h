/* dwordeditor
 *  The DWord Editor for NitePr G5
 * Author:
 *  Sir Gee of Five
 */

#ifndef _DWORDEDITOR_H
#define _DWORDEDITOR_H

#include <stdio.h>
#include <pspdebug.h>
#include <pspkerneltypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

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

#ifdef	__cplusplus
}
#endif

#endif
