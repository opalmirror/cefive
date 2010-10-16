#include <pspkerneltypes.h>
#include <pspmodulemgr.h>
#include <pspdebug.h>
#include <stdio.h>
#include "colorconfig.h"
#include "appletconfig.h"
#include "gameinfo.h"

int gameinfo_load(GameInfo *prInfo) {
    SceModule* prModule = NULL;
    SceLibraryEntryTable* prEntTable = NULL;
    SceLibraryStubTable* prStubTable = NULL;
    SceLibraryStubTable* prCur = NULL;
    int stubsz = 0;
    int stubct = 0;
    unsigned int tablesz = 0;
    unsigned int tablep = 0;
    int i = 0;

    if (prInfo == NULL) {
        return GAMEINFO_MEMORY;
    }
    if (prInfo->loaded == 1) {
        return;
    }

    prInfo->textEnd = 0x09FFFFFF;
    prModule = sceKernelFindModuleByAddress(0x08804000);
    if (prModule != NULL) {
        prInfo->prModule = prModule;
        prEntTable = (SceLibraryEntryTable*)prModule->ent_top;
        if (prEntTable != NULL) {
            prInfo->prLibTable = prEntTable;
            prInfo->libEntryCount = prModule->ent_size / sizeof(SceLibraryEntryTable);
        }
        prStubTable = (SceLibraryStubTable*)prModule->stub_top;
        if (prStubTable != NULL) {
            tablesz = prModule->stub_size;
            while (tablep < tablesz) {
                prCur = (SceLibraryStubTable*)(prModule->stub_top + tablep);
                stubsz = prCur->len * 4;
                prInfo->aprStubTable[stubct] = prCur;
                stubct++;
                tablep += stubsz;
            }
            prInfo->libStubCount = stubct;
        }
        for (i = 0; i < prInfo->libStubCount; i++) {
            prCur = prInfo->aprStubTable[i];
            if (prCur->stubtable < prInfo->textEnd) {
                prInfo->textEnd = prCur->stubtable;
            }
        }
    }
    prInfo->loaded = 1;
    return GAMEINFO_SUCCESS;
}

int gameinfoRedraw(GameInfo *prInfo) {
    AppletConfig* prCfg = NULL;
    ColorConfig* prColor = NULL;
    if (prInfo == NULL) {
        return GAMEINFO_MEMORY;
    }
    prCfg = prInfo->prApCfg;
    if (prInfo->loaded == 0) {
        if (gameinfo_load(prInfo) != GAMEINFO_SUCCESS) {
            return GAMEINFO_FAILURE;
        }
    }
    if (prCfg != NULL) {
        prColor = appletconfig_get_panelcolor(prCfg);
    } else {
        prColor = &prInfo->config.color;
    }
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenSetXY(prInfo->config.position.x, prInfo->config.position.y);
    SceModule *prMod = prInfo->prModule;
    if (prMod != NULL) {
        pspDebugScreenKprintf("Attribute: 0x%04X Version: %d.%d\n",
                prMod->attribute, prMod->version[0], prMod->version[1]);
        pspDebugScreenKprintf("Module Name: %s\n", prMod->modname);
        /*
        pspDebugScreenKprintf("unknown1: 0x%08X unknown2: 0x%08X\n",
                prMod->unknown1, prMod->unknown2);
         */
        pspDebugScreenKprintf("Module Id: 0x%08X\n", prMod->modid);
        /*
        pspDebugScreenKprintf("unknown3[4]: 0x%08X 0x%08X 0x%08X 0x%08X\n",
                prMod->unknown3[0], prMod->unknown3[1], prMod->unknown3[2],
                prMod->unknown3[3]);
         */
        pspDebugScreenKprintf("Ent Top: 0x%08X Ent Size: 0x%08X\n",
                prMod->ent_top, prMod->ent_size);
        pspDebugScreenKprintf("Stub Top: 0x%08X Stub Size: 0x%08X\n",
                prMod->stub_top, prMod->stub_size);
        /*
        pspDebugScreenKprintf("unknown4[4]: 0x%08X 0x%08X 0x%08X 0x%08X\n",
                prMod->unknown4[0], prMod->unknown4[1], prMod->unknown4[2],
                prMod->unknown4[3]);
         */
        pspDebugScreenKprintf("GP Value: 0x%08X Entry Address: 0x%08X\n",
                prMod->gp_value, prMod->entry_addr);
        pspDebugScreenKprintf("Text Address: 0x%08X Text Size: 0x%08X\n",
                prMod->text_addr, prMod->text_size);
        pspDebugScreenKprintf("Data Size: 0x%08X BSS Size: 0x%08X\n",
                prMod->data_size, prMod->bss_size);
        pspDebugScreenKprintf("nsegment: 0x%08X\n", prMod->nsegment);
        int i = 0;
        for (i = 0; i < 4; i++) {
            pspDebugScreenKprintf("Segment %d: 0x%08X (0x%08X)\n",
                    i, prMod->segmentaddr[i], prMod->segmentsize[i]);
        }
    } else {
        pspDebugScreenKprintf("Could not find SceModule pointer.\n");
    }
    pspDebugScreenKprintf(".text ends at 0x%08X.\n", prInfo->textEnd);
    pspDebugScreenKprintf("Library Entry Count: %d, Stub Count: %d\n",
            prInfo->libEntryCount, prInfo->libStubCount);
    SceLibraryEntryTable* prEnt = prInfo->prLibTable;
    pspDebugScreenKprintf(
            "Exported Library Version: %d.%d, Attributes: 0x%04X\n",
            prEnt->version[0], prEnt->version[1], prEnt->attribute);
    pspDebugScreenKprintf(
            "Exported Variables: %d, Functions: %d, Entry Table: 0x%08X\n",
            prEnt->vstubcount, prEnt->stubcount, prEnt->entrytable);
    pspDebugScreenKprintf("Import 0 Address: 0x%08X\n", prInfo->aprStubTable[0]);
    pspDebugScreenKprintf("Import 1 Address: 0x%08X\n", prInfo->aprStubTable[1]);
    pspDebugScreenKprintf("Import 2 Address: 0x%08X\n", prInfo->aprStubTable[2]);
    pspDebugScreenKprintf("Import 3 Address: 0x%08X\n", prInfo->aprStubTable[3]);
    return GAMEINFO_SUCCESS;
}
