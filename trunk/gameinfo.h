/* 
 * File:   gameinfo.h
 * Author: Sir Gee of Five
 *
 * Created on September 23, 2010, 4:35 PM
 */

#ifndef _GAMEINFO_H
#define	_GAMEINFO_H

#include <pspkerneltypes.h>
#include <pspmodulemgr.h>
#include <psploadcore.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "appletconfig.h"

#define GAMEINFO_SUCCESS    (0)
#define GAMEINFO_FAILURE    (-1)
#define GAMEINFO_MEMORY     (-2)

#define GAMEINFO_MAXSTUBCT  64

typedef struct _GameInfoConfig {
    ColorConfig color;
    CursorPos position;
}GameInfoConfig;

typedef struct _GameInfo {
    GameInfoConfig config;
    AppletConfig* prApCfg;
    int module_count;
    int loaded;
    SceUID arUID[64];
    SceModule* prModule;
    SceLibraryEntryTable* prLibTable;
    SceLibraryStubTable* aprStubTable[GAMEINFO_MAXSTUBCT];
    int libEntryCount;
    int libStubCount;
    void* textEnd;
    char* sGameId;
}GameInfo;

int gameinfo_load(GameInfo *prInfo);
int gameinfoRedraw(GameInfo *prInfo);

#endif	/* _GAMEINFO_H */

