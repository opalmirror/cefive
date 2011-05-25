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

#define GGAME_SUCCESS (0)
#define GGAME_FAILURE (-1)
#define GGAME_NULLPTR (-2)
#define GGAME_MEMORY (-3)
#define GGAME_IOERROR (-4)
#define GGAME_INVPATH (-5)

#define GGAME_GAMEID_LEN (10)
#define GGAME_PATH_LEN (255)
#define GGAME_DEF_DATAPATH "disc0:/UMD_DATA.BIN"
#define GGAME_MODULE_ADDR ((unsigned int)0x08804000)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GGame {
        char sGameId[GGAME_GAMEID_LEN + 1];
        char sDataPath[GGAME_PATH_LEN + 1];
        SceModule* gameModule;
    } GGame;

    int ggame_init(GGame* prGame);
    int ggame_load_gameid(GGame* prGame);

#ifdef	__cplusplus
}
#endif

#endif	/* GGAME_H */

