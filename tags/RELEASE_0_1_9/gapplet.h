/* 
 * File:   gapplet.h
 * Author: Sir Gee of Five
 *
 * Created on May 7, 2011, 4:32 PM
 */

#ifndef GAPPLET_H
#define	GAPPLET_H

#define GAPPLET_SUCCESS (0)
#define GAPPLET_FAILURE (-1)
#define GAPPLET_NULLPTR (-2)
#define GAPPLET_MEMORY (-3)

#define GAPPLET_NAME_LEN (63)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Gapplet {
        unsigned int id;
        char name[GAPPLET_NAME_LEN + 1];
        int *(button_circle)(void* pvApp);
        int *(button_cross)(void* pvApp);
        int *(button_ltrigger)(void* pvApp);
        int *(button_rtrigger)(void* pvApp);
        int *(button_square)(void* pvApp);
        int *(button_triangle)(void* pvApp);
        int *(dpad_down)(void* pvApp);
        int *(dpad_left)(void* pvApp);
        int *(dpad_right)(void* pvApp);
        int *(dpad_up)(void* pvApp);
        int *(redraw)(void* pvApp);
    }
    Gapplet;


#ifdef	__cplusplus
}
#endif

#endif	/* GAPPLET_H */

