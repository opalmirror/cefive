/* 
 * File:   gcheatengine.h
 * Author: Sir Gee of Five
 *
 * Created on October 19, 2010, 1:06 PM
 */

#ifndef GCHEATENGINE_H
#define	GCHEATENGINE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "gcheat.h"
#include "gcheatlist.h"

#define GCHEATENGINE_SUCCESS            (0)
#define GCHEATENGINE_FAILURE            (-1)
#define GCHEATENGINE_MEMORY             (-2)
#define GCHEATENGINE_NULLPTR            (-3)
#define GCHEATENGINE_INVIDX             (-4)
    
    typedef enum _EGCEState {
        GCE_Fault, GCE_Stopped, GCE_Idle, GCE_Active, GCE_Stopping
    }EGCEState;
    
    typedef struct _GCheatEngine {
        EGCEState rState;
        GCheatList* prCheatList;
    }GCheatEngine;
    
    int gcheatengine_add_cheat(GCheatEngine* prEngine, GCheat* prCheat);
    int gcheatengine_clear_cheats(GCheatEngine* prEngine);
    GCheat* gcheatengine_get_cheat(GCheatEngine* prEngine, int index);
    GCheatList* gcheatengine_get_cheatlist(GCheatEngine* prEngine);
    int gcheatengine_init(GCheatEngine* prEngine);
    int gcheatengine_run(GCheatEngine* prEngine);
    int gcheatengine_start(GCheatEngine* prEngine);
    int gcheatengine_stop(GCheatEngine* prEngine);

#ifdef	__cplusplus
}
#endif

#endif	/* GCHEATENGINE_H */

