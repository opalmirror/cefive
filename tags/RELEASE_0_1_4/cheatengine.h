#ifndef _CHEATENGINE_H_
#define _CHEATENGINE_H_

#include <pspkerneltypes.h>
#include "cheat.h"
#include "block.h"
#include "cefiveconfig.h"

#define CHEATENGINE_SUCCESS     (0)
#define CHEATENGINE_FAILURE     (-1)
#define CHEATENGINE_MEMORY      (-2)
#define CHEATENGINE_NULLPTR     (-3)
#define CHEATENGINE_FILEIO      (-4)

#define CHEATENGINE_CHEAT_MAX   512
#define CHEATENGINE_BLOCK_MAX   8192

typedef struct _CheatEngine {
    Cheat*      cheatlist;
    Block*      blocklist;
    CEFiveConfig* prConfig;
    int         cheat_count;
    int         block_count;
    SceUInt32   base_address;
    int         trigger_active;     // Whether the cheats are active.
} CheatEngine;

int cheatengineActivateCheats(CheatEngine* prEng);
Block* cheatengineAddBlock(CheatEngine *, unsigned char, unsigned int, unsigned int);
int cheatengineApplyBlock(CheatEngine *prEng, int index);
int cheatengineApplyCheat(CheatEngine *prEng, int index);
int cheatengineApplyUChar8Block(Block* prBlock);
int cheatengineApplyUInt32Block(Block* prBlock);
int cheatengineApplyUShort16Block(Block* prBlock);
int cheatengineDeactivateCheats(CheatEngine* prEng);
Block* cheatengineGetBlock(CheatEngine *, int index);
Cheat* cheatengineGetCheat(CheatEngine *, int index);
int cheatengineInit(CheatEngine* prEng, CEFiveConfig* prCfg, Cheat* arCheat,
        Block* arBlock);
int cheatengineLoadCheats(CheatEngine* prEng);
int cheatengineRefresh(CheatEngine* prEng);
int cheatengineResetBlock(CheatEngine *prEng, int index);
int cheatengineResetCheat(CheatEngine *prEng, int index);
int cheatengineResetUChar8Block(Block* prBlock);
int cheatengineResetUInt32Block(Block* prBlock);
int cheatengineResetUShort16Block(Block* prBlock);
int cheatengineSetCheatConstant(CheatEngine* prEng, int index);
int cheatengineSetCheatInactive(CheatEngine* prEng, int index);
int cheatengineSetCheatSelected(CheatEngine* prEng, int index);

#endif
