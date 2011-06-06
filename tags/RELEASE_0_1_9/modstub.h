/* 
 * File:   modstub.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 6:31 PM
 */

#ifndef MODSTUB_H
#define	MODSTUB_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>

#define MODSTUB_SUCCESS (0)
#define MODSTUB_FAILURE (-1)
#define MODSTUB_NULLPTR (-2)

#define MODSTUB_NAME_LEN (63)
#define MODSTUB_MODNAME_LEN (63)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _EStubType {
        MST_Function = 0, MST_Variable
    }
    EStubType;
    
    typedef struct _ModStub {
        EStubType stubType;
        SceUInt32 nId;
        void* pvStub;
        char name[MODSTUB_NAME_LEN + 1];
        char module[MODSTUB_MODNAME_LEN + 1];
    }
    ModStub;

    int modstub_copy(ModStub* prDest, ModStub* prSrc);
    
    int modstub_init(ModStub* prStub);
    
    int modstub_set(ModStub* prStub, EStubType stubType, SceUInt32 nId, 
            void* pvStub, const char* name, const char* modname);

#ifdef	__cplusplus
}
#endif

#endif	/* MODSTUB_H */

