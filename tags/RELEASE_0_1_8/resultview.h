/* 
 * File:   resultview.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 11:07 AM
 */

#ifndef RESULTVIEW_H
#define	RESULTVIEW_H

#include <stdio.h>
#include <psptypes.h>
#include <pspdebug.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"
#include "panelconfig.h"
#include "resultmodel.h"

/** Indicates success. */
#define RESULTVIEW_SUCCESS (0)

/** Indicates failure. */
#define RESULTVIEW_FAILURE (-1)

/** Indicates a NULL pointer. */
#define RESULTVIEW_NULLPTR (-2)

/** Indicates a memory error. */
#define RESULTVIEW_MEMORY (-3)

/** Indicates an invalid index. */
#define RESULTVIEW_INVIDX (-4)

/** Indicates an invalid ResultModel pointer. */
#define RESULTVIEW_INVMODEL (-5)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _ResultView {
        /** Panel Configuration struct */
        PanelConfig panelConfig;
        /** Cursor Position struct */
        CursorPos cursorPos;
        /** Result Model struct pointer */
        ResultModel* resultModel;
        /** Current Page index */
        int pageIndex;
        /** Indicates that the view needs to be redrawn. */
        int dirty;
    }
    /** The ResultView struct is used to represent a Result View. 
     * The Result View is a User Interface element used to browse Search 
     * Results provided by a ResultModel.
     */
    ResultView;

    /** Respond to the Circle button being pressed.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_button_circle(ResultView* prView);
    
    /** Respond to the Cross button being pressed.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_button_cross(ResultView* prView);
    
    /** Respond to the Left Trigger being pressed.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_button_ltrigger(ResultView* prView);
    
    /** Respond to the Right Trigger being pressed.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_button_rtrigger(ResultView* prView);
    
    /** Respond to the Square button being pressed.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_button_square(ResultView* prView);
    
    /** Respond to the Triangle button being pressed.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_button_triangle(ResultView* prView);
    
    /** Respond to the D-Pad being pressed down.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_dpad_down(ResultView* prView);
    
    /** Respond to the D-Pad being pressed left.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_dpad_left(ResultView* prView);
    
    /** Respond to the D-Pad being pressed right.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_dpad_right(ResultView* prView);
    
    /** Respond to the D-Pad being pressed up.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_dpad_up(ResultView* prView);

    /** Return a pointer to a ColorConfig struct representing the Cursor Color
     * Configuration.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* resultview_get_cursorcolor(ResultView* prView);
    
    /** Return a pointer to a CursorPos struct representing the Cursor position.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* resultview_get_cursorpos(ResultView* prView);
    
    /** Return a pointer to a ResultModel struct representing the Result Model.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a ResultModel struct or NULL is returned.
     */
    ResultModel* resultview_get_model(ResultView* prView);
    
    /** Return a pointer to a ColorConfig struct representing the Panel Color
     * Configuration.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* resultview_get_panelcolor(ResultView* prView);
    
    /** Return a pointer to a PanelConfig struct representing the Panel 
     * Configuration.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a PanelConfig struct or NULL is returned.
     */
    PanelConfig* resultview_get_panelconfig(ResultView* prView);
    
    /** Return a pointer to a CursorPos struct representing the position of
     * the Result View on the Debug Screen.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a CursorPos struct or NULL is returned.
     */
    CursorPos* resultview_get_position(ResultView* prView);
    
    /** Return a pointer to a Dimension struct representing the size of the
     * Result View on the Debug Screen.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return A pointer to a Dimension struct or NULL is returned.
     */
    Dimension* resultview_get_size(ResultView* prView);
    
    /** Initialize a Result View.
     * 
     * @param prView Pointer to the ResultView struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_init(ResultView* prView);

    /** Scroll the Result View down by one page.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_page_down(ResultView* prView);
    
    /** Scroll the Result View up by one page.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_page_up(ResultView* prView);
    
    /** Draw a Result View on the Debug Screen.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_redraw(ResultView* prView);

    /** Assign the Cursor Color of a Result Panel.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_set_cursorcolor(ResultView* prView, 
            const u32 background, const u32 text);
    
    /** Assign the current Cursor position of a Result View.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @param x int indicating the column.
     * @param y int indicating the row.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_set_cursorpos(ResultView* prView, const int x, const int y);
    
    /** Assign the Panel Color of a Result Panel.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @param background u32 representing the background color.
     * @param text u32 representing the text color.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_set_panelcolor(ResultView* prView,
            const u32 background, const u32 text);

    /** Assign the position of a Result Panel on the Debug Screen.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @param x int indicating the column.
     * @param y int indicating the row.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_set_position(ResultView* prView, const int x, const int y);

    /** Assign the size of a Result Panel on the Debug Screen.
     * 
     * @param prView Pointer to a ResultView struct representing the Result
     * View.
     * @param width int indicating the width in columns.
     * @param height int indicating the height in rows.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultview_set_size(ResultView* prView, 
            const int width, const int height);
    
#ifdef	__cplusplus
}
#endif

#endif	/* RESULTVIEW_H */
