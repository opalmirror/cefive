#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "ggame.h"

int ggame_init(GGame* prGame) {
    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    sprintf(prGame->sGameId, "");
    sprintf(prGame->sDataPath, GGAME_DEF_DATAPATH);
    return GGAME_SUCCESS;
}

int ggame_load_gameid(GGame* prGame) {
    SceUID fd = -1;
    int br = 0;
    
    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    if (strlen(prGame->sDataPath) < 1) {
        return GGAME_INVPATH;
    }
    fd = sceIoOpen(prGame->sDataPath, PSP_O_RDONLY, 0777);
    if (fd < 0) {
        return GGAME_FAILURE;
    }
    br = sceIoRead(fd, prGame->sGameId, GGAME_GAMEID_LEN);
    if (br != GGAME_GAMEID_LEN) {
        sceIoClose(fd);
        return GGAME_IOERROR;
    }
    if (sceIoClose(fd) < 0) {
        return GGAME_IOERROR;
    }
    return GGAME_SUCCESS;
}
