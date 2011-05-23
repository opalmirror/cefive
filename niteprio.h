/* 
 * File:   niteprio.h
 * Author: Sir Gee of Five
 *
 * Created on May 22, 2011, 1:31 PM
 */

#ifndef NITEPRIO_H
#define	NITEPRIO_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "cheatengine.h"
#include "geelog.h"

#define NITEPRIO_SUCCESS (0)
#define NITEPRIO_FAILURE (-1)
#define NITEPRIO_NULLPTR (-2)
#define NITEPRIO_MEMORY (-3)
#define NITEPRIO_IOERROR (-4)
#define NITEPRIO_INVFILE (-5)

#define NITEPRIO_LINELEN (255)

#ifdef	__cplusplus
extern "C" {
#endif

    int niteprio_import(CheatEngine* prEngine, const char* sFile);


#ifdef	__cplusplus
}
#endif

#endif	/* NITEPRIO_H */

