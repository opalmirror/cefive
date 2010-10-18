/* 
 * File:   cefive.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 11:43 AM
 */

#ifndef _CEFIVE_H
#define	_CEFIVE_H

#include <psploadcore.h>
#include "cefiveui.h"
#include "cheatengine.h"
#include "searchengine.h"
#include "cefiveconfig.h"

#define CEFIVE_SUCCESS      (0)
#define CEFIVE_FAILURE      (-1)
#define CEFIVE_MEMORY       (-2)
#define CEFIVE_NULLPTR      (-3)
#define CEFIVE_GAMEID_LEN   (10)
#define CEFIVE_VERSION_MAJ  0
#define CEFIVE_VERSION_MIN  1

typedef enum _ECEState {
    CES_Fault,
            CES_Stopped,
            CES_Starting,
            CES_Started,
            CES_Running,
            CES_UIRequest,
            CES_UIShowing,
            CES_UIDismissed,
            CES_CheatUpdate,
            CES_Stopping
}CEState;

typedef enum _ECEStartState {
    CESS_Fault, CESS_WaitKernelLib, CESS_WaitGameId, CESS_WaitGameModule, CESS_WaitCheatLoad, CESS_InitVram,
            CESS_InitCtrl, CESS_WaitVram, CESS_Finished
}ECEStartState;

typedef struct _CEFive {
    CEFiveConfig    rConfig;
    CheatEngine     rCheatEngine;
    SearchEngine    rSearchEngine;
    CEState         rRunState;
    ECEStartState   rStart;
    SceModule*      prKernelLib;
    SceModule*      prGame;
    void*           prVideoRam;
    char            sGameId[CEFIVE_GAMEID_LEN + 1];
}CEFive;


int cefive_init(CEFive* prCe);
int cefive_run(CEFive* prCe);
int cefive_start(CEFive* prCe);
int cefive_stop(CEFive* prCe);

#endif	/* _CEFIVE_H */

