/* 
 * File:   byteeditor.h
 * Author: Sir Gee of Five
 *
 * Created on September 24, 2010, 12:00 PM
 */

#ifndef _BYTEEDITOR_H
#define	_BYTEEDITOR_H

#include <pspkerneltypes.h>
#include "colorconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

    #define BYTEEDITOR_DEFCOLORBG   ((u32)0xFF000000);
    #define BYTEEDITOR_DEFCOLORFG   ((u32)0xFFFFFFFF);
    #define BYTEEDITOR_DEFEDITBG    ((u32)0xFFFFFFFF);
    #define BYTEEDITOR_DEFEDITFG    ((u32)0xFF000000);
    #define BYTEEDITOR_SUCCESS      (0)
    #define BYTEEDITOR_FAILURE      (-1)
    #define BYTEEDITOR_MEMORY       (-2)
    
    typedef struct _ByteEditorConfig {
        ColorConfig color;
        ColorConfig edit;
        unsigned char increments[2];
    }ByteEditorConfig;

    typedef struct _ByteEditor {
        ByteEditorConfig config;
        unsigned char value;
        int digit;
        int editing;
    }ByteEditor;

    unsigned char byteeditor_commit(ByteEditor* prEd);
    void byteeditor_decrement(ByteEditor* prEd);
    void byteeditor_edit(ByteEditor* prEd, unsigned char value);
    void byteeditor_increment(ByteEditor* prEd);
    int byteeditor_init(ByteEditor* prEd);
    void byteeditor_nextdigit(ByteEditor *prEd);
    void byteeditor_prevdigit(ByteEditor *prEd);
    int byteeditor_redraw(ByteEditor *prEd);
    
#ifdef	__cplusplus
}
#endif

#endif	/* _BYTEEDITOR_H */

