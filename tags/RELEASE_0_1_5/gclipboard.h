/* 
 * File:   gclipboard.h
 * Author: Sir Gee of Five
 *
 * Created on November 2, 2010, 10:26 AM
 */

#ifndef GCLIPBOARD_H
#define	GCLIPBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <pspkerneltypes.h>
    
#define GCLIPBOARD_SUCCESS      (0)
#define GCLIPBOARD_FAILURE      (-1)
#define GCLIPBOARD_MEMORY       (-2)
#define GCLIPBOARD_NULLPTR      (-3)

    typedef struct _GClipboard {
        SceUInt32 address;
        SceUInt32 value;
    };

#ifdef	__cplusplus
}
#endif

#endif	/* GCLIPBOARD_H */

