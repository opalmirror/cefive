/* 
 * File:   gstubmap.h
 * Author: Sir Gee of Five
 *
 * Created on May 29, 2011, 3:05 PM
 */

#ifndef GSTUBMAP_H
#define	GSTUBMAP_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include "modstub.h"

#define GSTUBMAP_SUCCESS (0)
#define GSTUBMAP_FAILURE (-1)
#define GSTUBMAP_NULLPTR (-2)
#define GSTUBMAP_MEMORY (-3)
#define GSTUBMAP_INVIDX (-4)

#define GSTUBMAP_SIZE (512)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GstubMap {
        ModStub stub[GSTUBMAP_SIZE];
        int count;
    }
    GstubMap;

    ModStub* gstubmap_add(GstubMap* prMap);
    
    int gstubmap_clear(GstubMap* prMap);
    
    ModStub* gstubmap_find(GstubMap* prMap, const SceUInt32 nId);
    
    ModStub* gstubmap_get(GstubMap* prMap, const int index);
    
    int gstubmap_init(GstubMap* prMap);

#ifdef	__cplusplus
}
#endif

#endif	/* GSTUBMAP_H */

