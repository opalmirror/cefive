/* 
 * File:   memviewpanel.h
 * Author: Sir Gee of Five
 *
 * Created on May 24, 2011, 10:41 AM
 */

#ifndef MEMVIEWPANEL_H
#define	MEMVIEWPANEL_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspdebug.h>
#include <psputils.h>
#include <psputilsforkernel.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "ggame.h"
#include "glabel.h"
#include "hexpad.h"
#include "jumpstack.h"
#include "mips.h"
#include "panelconfig.h"

/** Indicates success. */
#define MEMVIEWPANEL_SUCCESS (0)

/** Indicates failure. */
#define MEMVIEWPANEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define MEMVIEWPANEL_NULLPTR (-2)

/** Indicates a memory error. */
#define MEMVIEWPANEL_MEMORY (-3)

/** Indicates an invalid index. */
#define MEMVIEWPANEL_INVIDX (-4)

/** Indicates a bad address. */
#define MEMVIEWPANEL_BADADDR (-5)

/** The lowest address accessible. */
#define MEMVIEWPANEL_MINOFF ((SceUInt32)0x08800000)

/** The first address considered non-accessible. */
#define MEMVIEWPANEL_MAXOFF ((SceUInt32)0x0A000000)

/** The default pointer color. */
#define MEMVIEWPANEL_PTRCOLOR ((u32)0xFF0000D0)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _EEditMode {
        EM_None = 0,
        EM_Address,
        EM_UInt32
    }
    /** The EEditMode enumeration is used to specify the current editing mode. 
     */
    EEditMode;

    typedef enum _EValueType {
        VT_None = 0, /** Value type unknown. */
        VT_Instruction, /** MIPS32 Instruction. */
        VT_Pointer, /** 32-bit Pointer. */
        VT_UInt32 /** 32-bit Unsigned Integer Value. */
    }
    /** The EValueType enumeration is used to specify the type of value 
     * displayed in the Value column.
     */
    EValueType;

    typedef struct _MemViewPanel {
        /** Panel Configuration */
        PanelConfig panelConfig;
        /** Current Cursor Position */
        CursorPos cursorPos;
        /** Current Memory Offset */
        SceUInt32 offset;
        /** Minimum Addressable Offset */
        SceUInt32 minOffset;
        /** Maximum Offset */
        SceUInt32 maxOffset;
        /** The text color for values that are pointers. */
        u32 pointerColor;
        /** The Jump Stack */
        JumpStack jumpStack;
        /** The Hex Pad struct */
        HexPad hexPad;
        /** Pointer to the Game struct. */
        GGame* game;
        /** Indicates whether the Panel needs to be redrawn. */
        int dirty;
        /** Indicates whether the Panel is editing an address or value. */
        int editing;
        /** Indicates the current editing mode. */
        EEditMode editMode;
    }
    /** The MemViewPanel struct represents a Memory View Panel. */
    MemViewPanel;

    /** Respond to analog input from the analog stick.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param x unsigned char indicating the horizontal deflection of the
     * analog stick.
     * @param y unsigned char indicating the vertical deflection of the
     * analog stick.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_analog(MemViewPanel* prPanel, 
            unsigned char x, unsigned char y);
    
    /** Respond to the user pressing the Circle Button.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_button_circle(MemViewPanel* prPanel);

    /** Respond to the user pressing the Cross Button.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_button_cross(MemViewPanel* prPanel);

    /** Respond to the user pressing the Left Trigger.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_button_ltrigger(MemViewPanel* prPanel);
    
    /** Respond to the user pressing the Right Trigger.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_button_rtrigger(MemViewPanel* prPanel);
    
    /** Respond to the user pressing the Square Button.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_button_square(MemViewPanel* prPanel);

    /** Respond to the user pressing the Triangle Button.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_button_triangle(MemViewPanel* prPanel);

    /** Move the view cursor down by one row.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_cursor_down(MemViewPanel* prPanel);

    /** Move the view cursor left by one column.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_cursor_left(MemViewPanel* prPanel);

    /** Move the view cursor right by one column.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_cursor_right(MemViewPanel* prPanel);

    /** Move the view cursor up by one row.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_cursor_up(MemViewPanel* prPanel);

    int memviewpanel_dpad_down(MemViewPanel* prPanel);
    
    int memviewpanel_dpad_left(MemViewPanel* prPanel);
    
    int memviewpanel_dpad_right(MemViewPanel* prPanel);
    
    int memviewpanel_dpad_up(MemViewPanel* prPanel);
    
    /** Return a pointer to a PanelConfig struct representing the Panel
     * Configuration.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a PanelConfig struct or NULL is returned.
     */
    PanelConfig* memviewpanel_get_config(MemViewPanel* prPanel);

    /** Return a pointer to a ColorConfig struct representing the Cursor Color
     * Configuration.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* memviewpanel_get_cursorcolor(MemViewPanel* prPanel);

    /** Return a pointer to a CursorPos struct representing the current Cursor
     * Position.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* memviewpanel_get_cursorpos(MemViewPanel* prPanel);

    /** Return a pointer to a GGame struct representing the Game.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a GGame struct or NULL is returned.
     */
    GGame* memviewpanel_get_game(MemViewPanel* prPanel);
    
    /** Return a pointer to a HexPad struct representing the Hex Pad.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a HexPad struct or NULL is returned.
     */
    HexPad* memviewpanel_get_hexpad(MemViewPanel* prPanel);

    /** Return a pointer to a JumpStack struct representing the Jump Stack.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a JumpStack struct or NULL is returned.
     */
    JumpStack* memviewpanel_get_jumpstack(MemViewPanel* prPanel);

    /** Return a pointer to a ColorConfig struct representing the Panel Color
     * of the Hex Pad.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* memviewpanel_get_padcolor(MemViewPanel* prPanel);
    
    /** Return a pointer to a ColorConfig struct representing the Panel Color
     * Configuration.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* memviewpanel_get_panelcolor(MemViewPanel* prPanel);

    /** Return a pointer to a CursorPos struct representing the position of
     * the Memory View Panel on the Debug Screen.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* memviewpanel_get_position(MemViewPanel* prPanel);

    /** Return a pointer to a Dimension struct representing the size of the
     * Memory View Panel on the Debug Screen.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* memviewpanel_get_size(MemViewPanel* prPanel);

    /** Return the currently selected address in a Memory View Panel.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return A SceUInt32 containing the currently selected address or 0 is
     * returned.
     */
    SceUInt32 memviewpanel_get_seladdr(MemViewPanel* prPanel);
    
    /** Initialize a Memory View Panel.
     * 
     * @param prPanel Pointer to the MemViewPanel struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_init(MemViewPanel* prPanel);

    /** Indicate that a Memory View Panel needs to be redrawn.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_invalidate(MemViewPanel* prPanel);

    /** Scroll a Memory View Panel down by a Page.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_page_down(MemViewPanel* prPanel);

    /** Scroll a Memory View Panel up by a Page.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_page_up(MemViewPanel* prPanel);

    /** Draw a Memory View Panel on the Debug Screen.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_redraw(MemViewPanel* prPanel);

    /** Scroll a Memory View Panel down by the indicated number of rows.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param rows int indicating the number of rows to scroll.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_scroll_down(MemViewPanel* prPanel, const int rows);

    /** Scroll a Memory View Panel up by the indicated number of rows.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param rows int indicating the number of rows to scroll.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_scroll_up(MemViewPanel* prPanel, const int rows);

    /** Position a Memory View Panel to where the first visible row is at the 
     * indicated address.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param address SceUInt32 indicating the address to seek to.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_seek(MemViewPanel* prPanel, SceUInt32 address);

    /** Assign the Panel Color Configuration of a Memory View Panel.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param background u32 indicating the background color.
     * @param text u32 indicating the text color.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_set_panelcolor(MemViewPanel* prPanel,
            const u32 background, const u32 text);

    /** Assign the Screen Position of a Memory View Panel.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param x int indicating the column of the left edge.
     * @param y int indicating the row of the top edge.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_set_position(MemViewPanel* prPanel, const int x,
            const int y);

    /** Assign the size of a Memory View Panel.
     * 
     * @param prPanel Pointer to a MemViewPanel struct representing the Memory
     * View Panel.
     * @param width int indicating the width in columns.
     * @param height int indicating the height in rows.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int memviewpanel_set_size(MemViewPanel* prPanel, const int width,
            const int height);

#ifdef	__cplusplus
}
#endif

#endif	/* MEMVIEWPANEL_H */
