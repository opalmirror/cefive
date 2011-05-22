/* disassembler.h
 *  The Disassembler Applet for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef _DISASSEMBLER_H_
#define _DISASSEMBLER_H_

#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dwordeditor.h"
#include "dimension.h"
#include "addresscolumn.h"
#include "dwordcolumn.h"
#include "textcolumn.h"
#include "gameinfo.h"
#include "appletconfig.h"
#include "geelog.h"
#include "dasmrow.h"

/** Indicates success. */
#define DISASSEMBLER_SUCCESS    (0)

/** Indicates failure. */
#define DISASSEMBLER_FAILURE    (-1)

/** Indicates a memory error. */
#define DISASSEMBLER_MEMORY     (-2)

/** Indicates a NULL pointer. */
#define DISASSEMBLER_NULLPTR    (-3)
    
/** Indicates an address that is out of bounds. */
#define DISASSEMBLER_BADADDR    (-4)

/** Default background color. */
#define DISASSEMBLER_DEFBGCOLOR ((u32)0xFFD0D0D0)

/** Default foreground color. */
#define DISASSEMBLER_DEFFGCOLOR ((u32)0xFF000000)

/** The number of elements in the Jump Stack. */
#define DISASSEMBLER_JSLEN      10

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DisassemblerConfig {
        SceUInt32 base_address;
        SceUInt32 min_offset;
        SceUInt32 max_offset;
        ColorConfig address_color;
        ColorConfig value_color;
        ColorConfig code_color;
        ColorConfig cursorrow_color;
        ColorConfig cursor_color;
        ColorConfig pointer_color;
        CursorPos position;
        CursorPos tablepos;
        Dimension tablesize;
    } DisassemblerConfig;

    typedef struct _Disassembler {
        DisassemblerConfig config;
        AppletConfig* prApCfg;
        SceUInt32 offset;
        SceUInt32 poffset;
        CursorPos cursor;
        CursorPos rPrev;
        int editing;
        DwordEditor address_editor;
        DwordEditor value_editor;
        int dirty;
        int cursordirty;
        SceUInt32 jump_stack[DISASSEMBLER_JSLEN];
        int cur_jump;
        DasmRow rRow;
        GameInfo* prGameInfo;
    } Disassembler;

    AppletConfig* disassembler_get_appletconfig(Disassembler* prDasm);

    ColorConfig* disassembler_get_cursorcolor(Disassembler* prDasm);
    
    CursorPos* disassembler_get_cursorpos(Disassembler* prDasm);
    
    ColorConfig* disassembler_get_panelcolor(Disassembler* prDasm);
    
    void disassemblerAttemptJump(Disassembler *);
    void disassemblerAttemptReturn(Disassembler *);
    void disassemblerCircleButton(Disassembler *);
    void disassemblerCrossButton(Disassembler *);
    void disassemblerCursorDown(Disassembler *);
    void disassemblerCursorLeft(Disassembler *);
    void disassemblerCursorRight(Disassembler *);
    void disassemblerCursorUp(Disassembler *);
    void disassemblerDpadDown(Disassembler *);
    void disassemblerDpadLeft(Disassembler *);
    void disassemblerDpadRight(Disassembler *);
    void disassemblerDpadUp(Disassembler *);
    int disassemblerInit(Disassembler *);
    void disassemblerPageDown(Disassembler *prPanel);
    void disassemblerPageUp(Disassembler *prPanel);
    void disassemblerRedraw(Disassembler *);
    void disassemblerScrollDown(Disassembler *);
    void disassemblerScrollUp(Disassembler *);
    int disassemblerSeek(Disassembler *, SceUInt32 offset);
    void disassemblerSquareButton(Disassembler *);
    SceUInt32 disassemblerTell(Disassembler *);
    void disassemblerTriangleButton(Disassembler *);

#ifdef	__cplusplus
}
#endif

#endif
