/* 
 * File:   buttonstate.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 2:24 PM
 */

#ifndef BUTTONSTATE_H
#define	BUTTONSTATE_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _ButtonState {
        /** Cross Button state */
        int cross;
        /** Circle Button state */
        int circle;
        /** Square Button state */
        int square;
        /** Triangle Button state */
        int triangle;
        /** Left Trigger state */
        int ltrigger;
        /** Right Trigger state */
        int rtrigger;
        /** Start Button state */
        int start;
        /** Select Button state */
        int select;
        /** D-Pad Up state */
        int up;
        /** D-Pad Down state */
        int down;
        /** D-Pad Left state */
        int left;
        /** D-Pad Right state */
        int right;
    }
    /** The ButtonState struct is used to represent the current hardware
     * Button state in CeFive.
     */
    ButtonState;

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONSTATE_H */
