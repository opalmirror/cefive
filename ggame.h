/* 
 * File:   ggame.h
 * Author: Sir Gee of Five
 *
 * Created on May 14, 2011, 2:19 PM
 */

#ifndef GGAME_H
#define	GGAME_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include <psploadcore.h>
#include "gsegmap.h"
#include "gsegment.h"
#include "gstubmap.h"
#include "modstub.h"

#define GGAME_SUCCESS (0)
#define GGAME_FAILURE (-1)
#define GGAME_NULLPTR (-2)
#define GGAME_MEMORY (-3)
#define GGAME_IOERROR (-4)
#define GGAME_INVPATH (-5)
#define GGAME_INVMODULE (-6)

#define GGAME_GAMEID_LEN (10)
#define GGAME_PATH_LEN (255)
#define GGAME_DEF_DATAPATH "disc0:/UMD_DATA.BIN"
#define GGAME_MODULE_ADDR ((unsigned int)0x08804000)
#define GGAME_STUBTABLE_SIZE (128)

#define GGAME_NID_MODSTART ((SceUInt32)0xD632ACDB)
#define GGAME_NID_MODSTOP ((SceUInt32)0xCEE8593C)
#define GGAME_NID_MODINFO ((SceUInt32)0xF01D73A7)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GGame {
        char sGameId[GGAME_GAMEID_LEN + 1];
        char sDataPath[GGAME_PATH_LEN + 1];
        SceModule* gameModule;
        SceLibraryStubTable* stubtable[GGAME_STUBTABLE_SIZE];
        SceUInt32 moduleStart;
        SceUInt32 moduleStop;
        SceUInt32 moduleInfo;
        int exportCount;
        GsegMap segmentMap;
        GstubMap stubMap;
    } GGame;

    Gsegment* ggame_find_segment(GGame* prGame, const SceUInt32 address);
    
    int ggame_get_exportcount(GGame* prGame);
    
    SceLibraryEntryTable* ggame_get_exporttable(GGame* prGame);
    
    SceLibraryStubTable* ggame_get_importtable(GGame* prGame);
    
    SceModule* ggame_get_module(GGame* prGame);
    
    SceUInt32 ggame_get_module_start(GGame* prGame);
    
    /** Return a pointer to a GsegMap struct representing the Segment Map.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @return A pointer to a GsegMap struct or NULL is returned.
     */
    GsegMap* ggame_get_segmap(GGame* prGame);
    
    GstubMap* ggame_get_stubmap(GGame* prGame);
    
    int ggame_init(GGame* prGame);
    
    int ggame_load_exports(GGame* prGame);
    
    int ggame_load_gameid(GGame* prGame);

    int ggame_load_module(GGame* prGame);
    
#ifdef	__cplusplus
}
#endif

#endif	/* GGAME_H */

