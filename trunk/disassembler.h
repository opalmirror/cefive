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
#include "appletconfig.h"
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "gameinfo.h"
#include "geelog.h"
#include "memviewpanel.h"
#include "mips.h"

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
        int editing;
        int dirty;
        int cursordirty;
        int cur_jump;
        GameInfo* prGameInfo;
        MemViewPanel memViewPanel;
    } Disassembler;

    void disassembler_button_circle(Disassembler *);
    
    void disassembler_button_cross(Disassembler *);
    
    void disassembler_button_square(Disassembler *);

    void disassembler_button_triangle(Disassembler *);

    void disassembler_dpad_down(Disassembler *);
    
    void disassembler_dpad_left(Disassembler *);
    
    void disassembler_dpad_right(Disassembler *);
    
    void disassembler_dpad_up(Disassembler *);

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
    
    MemViewPanel* disassembler_get_memview(Disassembler* prDasm);
    
    ColorConfig* disassembler_get_panelcolor(Disassembler* prDasm);
    
    CursorPos* disassembler_get_position(Disassembler* prDasm);
    
    Dimension* disassembler_get_size(Disassembler* prDasm);
    
    int disassembler_init(Disassembler *prPanel, AppletConfig *prApCfg);
    
    void disassembler_page_down(Disassembler *prPanel);
    
    void disassembler_page_up(Disassembler *prPanel);
    
    void disassembler_redraw(Disassembler *);
    
    void disassembler_scroll_down(Disassembler *);
    
    void disassembler_scroll_up(Disassembler *);
    
    int disassembler_seek(Disassembler *, SceUInt32 offset);
    
    SceUInt32 disassembler_tell(Disassembler *);

#ifdef	__cplusplus
}
#endif

#endif
