/* disassembler.h
 *  The Disassembler Applet for Cheat Engine of Five
 * Author: Sir Gee of Five
 * 
 */
#ifndef _DISASSEMBLER_H_
#define _DISASSEMBLER_H_

#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <stdio.h>
#include "addresscolumn.h"
#include "appletconfig.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dasmrow.h"
#include "dimension.h"
#include "dwordcolumn.h"
#include "dwordeditor.h"
#include "gameinfo.h"
#include "geelog.h"
#include "memviewpanel.h"
#include "mips.h"
#include "textcolumn.h"

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
        MemViewPanel memViewPanel;
    } Disassembler;

    void disassembler_circle_button(Disassembler *);
    
    void disassembler_cross_button(Disassembler *);
    
    void disassembler_cursor_down(Disassembler *prPanel);
    
    void disassembler_cursor_left(Disassembler *);
    
    void disassembler_cursor_right(Disassembler *);
    
    void disassembler_cursor_up(Disassembler *prPanel);
    
    /** Return a pointer to an AppletConfig struct representing the Applet
     * Configuration.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* disassembler_get_appletconfig(Disassembler* prDasm);

    /** Return a pointer to a ColorConfig struct representing the Cursor
     * Color Configuration.
     * 
     * @param prDasm Pointer to a Disassembler struct representing the
     * Disassembler Applet.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* disassembler_get_cursorcolor(Disassembler* prDasm);
    
    CursorPos* disassembler_get_cursorpos(Disassembler* prDasm);
    
    MemViewPanel* disassembler_get_memview(Disassembler* prDasm);
    
    ColorConfig* disassembler_get_panelcolor(Disassembler* prDasm);
    
    CursorPos* disassembler_get_position(Disassembler* prDasm);
    
    Dimension* disassembler_get_size(Disassembler* prDasm);
    
    void disassembler_attempt_jump(Disassembler *prPanel);
    
    void disassemblerAttemptReturn(Disassembler *);
    
    void disassemblerDpadDown(Disassembler *);
    void disassemblerDpadLeft(Disassembler *);
    void disassemblerDpadRight(Disassembler *);
    void disassemblerDpadUp(Disassembler *);
    
    int disassembler_init(Disassembler *prPanel, AppletConfig *prApCfg);
    
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
