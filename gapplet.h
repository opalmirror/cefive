/* 
 * File:   gapplet.h
 * Author: Sir Gee of Five
 *
 * Created on May 7, 2011, 4:32 PM
 */

#ifndef GAPPLET_H
#define	GAPPLET_H

#include "geelog.h"

#define GAPPLET_SUCCESS (0)
#define GAPPLET_FAILURE (-1)
#define GAPPLET_NULLPTR (-2)
#define GAPPLET_MEMORY (-3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Gapplet {
        /** Logger pointer */
        GeeLog* prLog;        
    }
    Gapplet;


#ifdef	__cplusplus
}
#endif

#endif	/* GAPPLET_H */

