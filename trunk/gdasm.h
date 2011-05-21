/* 
 * File:   gdasm.h
 * Author: Sir Gee of Five
 *
 * Created on May 2, 2011, 11:21 AM
 */

#ifndef GDASM_H
#define	GDASM_H

#include <psptypes.h>
#include "dasmconfig.h"
#include "geelog.h"
#include "cursorpos.h"
#include "colorconfig.h"
#include "dasmtable.h"
#include "appletconfig.h"
#include "panelconfig.h"
#include "dimension.h"

/** Indicates success. */
#define GDASM_SUCCESS (0)
/** Indicates failure. */
#define GDASM_FAILURE (-1)
/** Indicates a NULL pointer. */
#define GDASM_NULLPTR (-2)
/** Indicates a memory error. */
#define GDASM_MEMORY (-3)
/** Indicates an invalid address. */
#define GDASM_BADADDR (-4)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Gdasm {
        /** Dasm Configuration struct */
        DasmConfig rConfig;
        /** Pointer to the GeeLog struct representing the logger. */
        GeeLog* prLog;
        /** Disassembler Table struct */
        DasmTable rTable;
        /** Indication that the Disassembler needs redrawing. */
        int dirty;
        /** Cursor Position struct. */
        CursorPos rCursor;
    }
    /** The Gdasm struct represents the cefive Disassembler Applet.
     */
    Gdasm;

    /** Process the user releasing the Circle button.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_circle_button(Gdasm* prDasm);
    
    /** Process the user releasing the Cross button.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_cross_button(Gdasm* prDasm);
    
    int gdasm_cursor_down(Gdasm* prDasm);
    
    int gdasm_cursor_left(Gdasm* prDasm);
    
    int gdasm_cursor_right(Gdasm* prDasm);
    
    int gdasm_cursor_up(Gdasm* prDasm);
    
    int gdasm_dpad_down(Gdasm* prDasm);
    
    int gdasm_dpad_left(Gdasm* prDasm);
    
    int gdasm_dpad_right(Gdasm* prDasm);
    
    int gdasm_dpad_up(Gdasm* prDasm);
    
    /** Return a pointer to a DasmConfig struct representing the Disassembler
     * configuration.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a DasmConfig struct or NULL is returned.
     */
    DasmConfig* gdasm_get_config(Gdasm* prDasm);
    
    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration for the cursor in the Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* gdasm_get_cursorcolor(Gdasm* prDasm);
    
    /** Return the current cursor column of a Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return int containing the current cursor column, or -1 if an error
     * occured.
     */
    int gdasm_get_cursorcolumn(Gdasm* prDasm);
    
    /** Return a pointer to a CursorPos struct representing the current cursor
     * position for the Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* gdasm_get_cursorpos(Gdasm* prDasm);
    
    /** Return the current cursor row of a Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return int containing the current cursor row, or -1 if an error occured.
     */
    int gdasm_get_cursorrow(Gdasm* prDasm);
    
    /** Return a pointer to a ColorConfig struct representing the Color
     * Configuration for the Panel in the Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* gdasm_get_panelcolor(Gdasm* prDasm);
    
    /** Return a pointer to a DasmTable struct representing the Disassembler
     * Table.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a DasmTable struct or NULL is returned.
     */
    DasmTable* gdasm_get_table(Gdasm* prDasm);
    
    /** Return a pointer to a PanelConfig struct representing the Panel
     * Configuration for the Disassembler Table.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a PanelConfig struct or NULL is returned.
     */
    PanelConfig* gdasm_get_tablepanelconfig(Gdasm* prDasm);

    /** Return a pointer to a CursorPos struct representing the top left corner
     * of the Disassembler Table.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* gdasm_get_tableposition(Gdasm* prDasm);
    
    /** Return a pointer to a Dimension struct representing the size of the
     * Disassembler Table.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* gdasm_get_tablesize(Gdasm* prDasm);
    
    /** Initialize a Gdasm struct to use to represent a Disassembler.
     * 
     * @param prDasm Pointer to the Gdasm struct to initialize.
     * @param prLog Pointer to a GeeLog struct representing a Logger to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_init(Gdasm* prDasm, GeeLog* prLog);
    
    /** Indicate that the Disassembler needs to be redrawn.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_invalidate(Gdasm* prDasm);

    /** Add a log statement to the Logger of a Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @param rLevel ELogLevel value indicating the Log level of the Message.
     * @param sMsg const char pointer to the Message to log.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_log(Gdasm* prDasm, ELogLevel rLevel, const char* sMsg);
    
    /** Draw the Disassembler on the current debug screen.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_redraw(Gdasm* prDasm);
    
    /** Place the Dissasembler to where the indicated address is the first
     * address displayed on the page.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @param address SceUInt32 containing the address to seek to.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_seek(Gdasm* prDasm, SceUInt32 address);
    
    /** Assign an Applet Configuration to a Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @param prCfg Pointer to the AppletConfig struct to assign.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_set_appletconfig(Gdasm* prDasm, AppletConfig* prCfg);
    
    /** Assign a trace logger to a Disassembler.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @param prLog Pointer to a GeeLog struct representing the Logger to
     * assign.
     * @return 0 indicates success, less than 0 indicates failure. 
     */
    int gdasm_set_logger(Gdasm* prDasm, GeeLog* prLog);

    /** Assign the position of the Disassembler Table in the Disassembler by
     * specifying the top and left corner.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @param x int containing the column of the left edge.
     * @param y int containing the row of the top edge.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_set_tableposition(Gdasm* prDasm, int x, int y);
    
    /** Assign the size of the Disassembler Table in the Disassembler by
     * specifying the width and height.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @param width int containing the width of the Table in columns.
     * @param height int containing the height of the Table in rows.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_set_tablesize(Gdasm* prDasm, int width, int height);
    
    /** Process the user releasing the Square button.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_square_button(Gdasm* prDasm);
    
    /** Process the user releasing the Triangle button.
     * 
     * @param prDasm Pointer to a Gdasm struct representing the Disassembler.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int gdasm_triangle_button(Gdasm* prDasm);

#ifdef	__cplusplus
}
#endif

#endif	/* GDASM_H */

