#include "ggame.h"
#include "geelog.h"

int ggame_add_label(GGame* prGame, const SceUInt32 address, const char* name) {
    GlabelMap* prMap = NULL;
    Glabel* prLabel = NULL;
    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    prMap = ggame_get_labelmap(prGame);
    prLabel = glabelmap_add(prMap);
    if (prLabel == NULL) {
        return GGAME_FAILURE;
    }
    if (glabel_set(prLabel, address, name) < 0) {
        return GGAME_FAILURE;
    }
    return GGAME_SUCCESS;
}

Glabel* ggame_find_label(GGame* prGame, const SceUInt32 address) {
    GlabelMap* prMap = NULL;
    if (prGame == NULL) {
        return NULL;
    }
    prMap = ggame_get_labelmap(prGame);
    return glabelmap_find(prMap, address);
}

Gsegment* ggame_find_segment(GGame* prGame, const SceUInt32 address) {
    GsegMap* prMap = NULL;
    if (prGame == NULL) {
        return NULL;
    }
    prMap = ggame_get_segmap(prGame);
    return gsegmap_find(prMap, address);
}

int ggame_get_exportcount(GGame* prGame) {
    int count = GGAME_FAILURE;
    SceModule* prModule = NULL;
    if (prGame != NULL) {
        prModule = ggame_get_module(prGame);
        if (prModule != NULL) {
            count = prModule->ent_size / sizeof(SceLibraryEntryTable);
        }
    }
    return count;
}

SceLibraryEntryTable* ggame_get_exporttable(GGame* prGame) {
    SceModule* prModule = NULL;
    if (prGame != NULL) {
        prModule = ggame_get_module(prGame);
        if (prModule != NULL) {
            return (SceLibraryEntryTable*)prModule->ent_top;
        }
    }
    return NULL;
}

SceLibraryStubTable* ggame_get_importtable(GGame* prGame) {
    SceModule* prModule = NULL;
    if (prGame != NULL) {
        prModule = ggame_get_module(prGame);
        if (prModule != NULL) {
            return (SceLibraryStubTable*)prModule->stub_top;
        }
    }
    return NULL;
}

GlabelMap* ggame_get_labelmap(GGame* prGame) {
    if (prGame == NULL) {
        return NULL;
    }
    return &prGame->labelMap;
}

SceModule* ggame_get_module(GGame* prGame) {
    if (prGame != NULL) {
        return prGame->gameModule;
    }
    return NULL;
}

SceUInt32 ggame_get_module_start(GGame* prGame) {
    SceUInt32 address = 0;
    GstubMap* prStubMap = NULL;
    ModStub* prStub = NULL;
    
    if (prGame != NULL) {
        prStubMap = ggame_get_stubmap(prGame);
        if (prStubMap != NULL) {
            prStub = gstubmap_find(prStubMap, GGAME_NID_MODSTART);
            if (prStub != NULL) {
                address = prStub->pvStub;
            }
        }
    }
    return address;
}

GsegMap* ggame_get_segmap(GGame* prGame) {
    if (prGame == NULL) {
        return NULL;
    }
    return &prGame->segmentMap;
}

GstubMap* ggame_get_stubmap(GGame* prGame) {
    if (prGame == NULL) {
        return NULL;
    }
    return &prGame->stubMap;
}

int ggame_init(GGame* prGame) {
    GsegMap* prMap = NULL;
    GstubMap* prStubs = NULL;
    GlabelMap* prLabels = NULL;
    
    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    sprintf(prGame->sGameId, "");
    sprintf(prGame->sDataPath, GGAME_DEF_DATAPATH);
    prGame->gameModule = NULL;
    prMap = ggame_get_segmap(prGame);
    if (gsegmap_init(prMap) < 0) {
        return GGAME_FAILURE;
    }
    prStubs = ggame_get_stubmap(prGame);
    if (gstubmap_init(prStubs) < 0) {
        return GGAME_FAILURE;
    }
    prLabels = ggame_get_labelmap(prGame);
    if (glabelmap_init(prLabels) < 0) {
        return GGAME_FAILURE;
    }
    return GGAME_SUCCESS;
}

int ggame_load_exports(GGame* prGame) {
    SceModule* prModule = NULL;
    SceLibraryEntryTable* prEnt = NULL;
    SceUInt32* prFnid = NULL;
    SceUInt32* prVnid = NULL;
    SceUInt32* prFstub = NULL;
    SceUInt32* prVstub = NULL;
    GstubMap* prSmap = NULL;
    ModStub* prStub = NULL;
    SceUInt32 nId = 0;
    void* pvStub = NULL;
    int vars = 0;
    int funcs = 0;
    int offset = 0;
    int i = 0;
    char *sName = NULL;
    
    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    prModule = ggame_get_module(prGame);
    if (prModule == NULL) {
        return GGAME_INVMODULE;
    }
    prEnt = ggame_get_exporttable(prGame);
    if (prEnt == NULL) {
        return GGAME_FAILURE;
    }
    vars = prEnt->vstubcount;
    funcs = prEnt->stubcount;
    prSmap = ggame_get_stubmap(prGame);
    for (i = 0; i < funcs; i++) {
        offset = i * sizeof(SceUInt32);
        prFnid = (SceUInt32*)(prEnt->entrytable + offset);
        nId = *prFnid;
        offset = (i + funcs + vars) * sizeof(SceUInt32);
        prFstub = (SceUInt32*)(prEnt->entrytable + offset);
        pvStub = *prFstub;
        prStub = gstubmap_add(prSmap);
        if (prStub != NULL) {
            sName = NULL;
            if (nId == GGAME_NID_MODSTART) {
                sName = "module_start";
                ggame_add_label(prGame, pvStub, sName);
            }
            if (nId == GGAME_NID_MODSTOP) {
                sName = "module_stop";
                ggame_add_label(prGame, pvStub, sName);
            }
            if (modstub_set(prStub, MST_Function, nId, pvStub, sName, 
                    prModule->modname) < 0) {
                return GGAME_FAILURE;
            }
        }
    }
    for (i = 0; i < vars; i++) {
        offset = (i + funcs) * sizeof(SceUInt32);
        prVnid = (SceUInt32*)(prEnt->entrytable + offset);
        nId = *prVnid;
        offset = (i + funcs + vars + funcs) * sizeof(SceUInt32);
        prVstub = (SceUInt32*)(prEnt->entrytable + offset);
        pvStub = *prVstub;
        prStub = gstubmap_add(prSmap);
        if (prStub != NULL) {
            sName = NULL;
            if (nId == GGAME_NID_MODINFO) {
                sName = "module_info";
                ggame_add_label(prGame, pvStub, sName);
            }
            if (modstub_set(prStub, MST_Variable, nId, pvStub, sName,
                    prModule->modname) < 0) {
                return GGAME_FAILURE;
            }
        }
    }
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

int ggame_load_imports(GGame* prGame) {
    SceModule* prModule = NULL;
    SceLibraryStubTable* prTable = NULL;
    GstubMap* prSmap = NULL;
    GsegMap* prMap = NULL;
    Gsegment* prSegment = NULL;
    ModStub* prStub = NULL;
    void* pvStubTop = NULL;
    SceUInt32* prNid = NULL;
    SceUInt32 nId = 0;
    SceUInt32 textEnd = 0;
    void* pvStub = NULL;
    unsigned int uiStubSize = 0;
    unsigned int uiStubRead = 0;
    int i = 0;
    char sSegName[64];
    char sLabel[GLABEL_TEXT_LEN + 1];

    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    prModule = ggame_get_module(prGame);
    if (prModule == NULL) {
        return GGAME_INVMODULE;
    }
    prSmap = ggame_get_stubmap(prGame);
    prMap = ggame_get_segmap(prGame);
    pvStubTop = prModule->stub_top;
    uiStubSize = prModule->stub_size;
    textEnd = pvStubTop;
    while (uiStubRead < uiStubSize) {
        prTable = (SceLibraryStubTable*)(pvStubTop + uiStubRead);
        prNid = (SceUInt32*)prTable->nidtable;
        for (i = 0; i < prTable->stubcount; i++) {
            nId = prNid[i];
            pvStub = prTable->stubtable + (i * 8);
            if (pvStub < textEnd) {
                textEnd = pvStub;
            }
            prStub = gstubmap_add(prSmap);
            if (prStub != NULL) {
                if (modstub_set(prStub, MST_Function, nId, pvStub, NULL, 
                        prTable->libname) < 0) {
                    return GGAME_FAILURE;
                }
                switch (nId) {
                    case 0x05572A5F:
                        sprintf(sLabel, "sceKernelExitGame");
                        break;
                    case 0x090CCB3F:
                        sprintf(sLabel, "sceKernelPowerTick");
                        break;
                    case 0x13A5ABEF:
                        sprintf(sLabel, "sceKernelPrintf");
                        break;
                    case 0x172D316E:
                        sprintf(sLabel, "sceKernelStdin");
                        break;
                    case 0x20E911AA:
                        sprintf(sLabel, "sceKernelUnloadModule");
                        break;
                    case 0x27E22EC2:
                        sprintf(sLabel, "sceKernelResumeDispatchThread");
                        break;
                    case 0x27EB27B8:
                        sprintf(sLabel, "sceIoLseek");
                        break;
                    case 0x293B45B8:
                        sprintf(sLabel, "sceKernelGetThreadId");
                        break;
                    case 0x2E0911AA:
                        sprintf(sLabel, "sceKernelUnloadModule");
                        break;
                    case 0x3AEE7261:
                        sprintf(sLabel, "sceKernelPowerUnlock");
                        break;
                    case 0x446D8DE6:
                        sprintf(sLabel, "sceKernelCreateThread");
                        break;
                    case 0x4AC57943:
                        sprintf(sLabel, "sceKernelRegisterExitCallback");
                        break;
                    case 0x50F0C1EC:
                        sprintf(sLabel, "sceKernelStartModule");
                        break;
                    case 0x7591C7DB:
                        sprintf(sLabel, "sceKernelSetCompiledSdkVersion");
                        break;
                    case 0x977DE386:
                        sprintf(sLabel, "sceKernelLoadModule");
                        break;
                    case 0xA14F40B2:
                        sprintf(sLabel, "sceKernelVolatileMemTryLock");
                        break;
                    case 0xA569E425:
                        sprintf(sLabel, "sceKernelVolatileMemUnlock");
                        break;
                    case 0xA6BAB2E9:
                        sprintf(sLabel, "sceKernelStdout");
                        break;
                    case 0xAA73C935:
                        sprintf(sLabel, "sceKernelExitThread");
                        break;
                    case 0xBD2F1094:
                        sprintf(sLabel, "sceKernelLoadExec");
                        break;
                    case 0xD1FF982A:
                        sprintf(sLabel, "sceKernelStopModule");
                        break;
                    case 0xD8B73127:
                        sprintf(sLabel, "sceKernelGetModuleIdByAddress");
                        break;
                    case 0xEADB1BD7:
                        sprintf(sLabel, "sceKernelPowerLock");
                        break;
                    case 0xF0A26395:
                        sprintf(sLabel, "sceKernelGetModuleId");
                        break;
                    case 0xF475845D:
                        sprintf(sLabel, "sceKernelStartThread");
                        break;
                    case 0xF77D77CB:
                        sprintf(sLabel, "sceKernelSetCompilerVersion");
                        break;
                    case 0xF78BA90A:
                        sprintf(sLabel, "sceKernelStderr");
                        break;
                    default:
                        sprintf(sLabel, "%s_%08X", prTable->libname, nId);
                }
                
                ggame_add_label(prGame, pvStub, sLabel);
            }
        }
        prSegment = gsegmap_add(prMap);
        if (prSegment != NULL) {
            sprintf(sSegName, ".%s.text", prTable->libname);
            if (gsegment_set(prSegment, prTable->stubtable, 
                    prTable->stubtable + (prTable->stubcount * 8) + 4, 
                    sSegName, SET_Text) < 0) {
                return GGAME_FAILURE;
            }
        }
        uiStubRead += (prTable->len * 4);
    }
    prSegment = gsegmap_add(prMap);
    if (prSegment != NULL) {
        if (gsegment_set(prSegment, 0x08804000, textEnd, ".text", SET_Text) < 0)
        {
            return GGAME_FAILURE;
        }
    }
    return GGAME_SUCCESS;
}

int ggame_load_module(GGame* prGame) {
    const char* sFunc = "ggame_load_module";
    SceUInt32 entTop = 0;
    SceUInt32 entSz = 0;
    SceUInt32 entEnd = 0;
    SceUInt32 infTop = 0;
    SceUInt32 infEnd = 0;
    GsegMap* prMap = NULL;
    Gsegment* prSeg = NULL;
    GstubMap* prSmap = NULL;
    ModStub* prStub = NULL;
    char sMsg[GEELOG_LINELEN + 1];
    
    if (prGame == NULL) {
        return GGAME_NULLPTR;
    }
    prMap = ggame_get_segmap(prGame);
    prGame->gameModule = sceKernelFindModuleByAddress(GGAME_MODULE_ADDR);
    if (prGame->gameModule == NULL) {
        return GGAME_FAILURE;
    }
    entTop = prGame->gameModule->ent_top;
    entSz = prGame->gameModule->ent_size;
    entEnd = entTop + entSz + 4;
    prSeg = gsegmap_add(prMap);
    if (prSeg == NULL) {
        return GGAME_FAILURE;
    }
    if (gsegment_set(prSeg, entTop, entEnd, ".lib.ent.top", SET_RoData) < 0) 
    {
        return GGAME_FAILURE;
    }    
    
    entTop = prGame->gameModule->stub_top;
    entSz = prGame->gameModule->stub_size;
    entEnd = entTop + entSz + 4;
    prSeg = gsegmap_add(prMap);
    if (prSeg == NULL) {
        return GGAME_FAILURE;
    }
    if (gsegment_set(prSeg, entTop, entEnd, ".lib.stub.top", SET_RoData) < 0)
    {
        return GGAME_FAILURE;
    }
    
    if (ggame_load_exports(prGame) < 0) {
        return GGAME_FAILURE;
    }
    if (ggame_load_imports(prGame) < 0) {
        return GGAME_FAILURE;
    }
    prSmap = ggame_get_stubmap(prGame);
    prStub = gstubmap_find(prSmap, GGAME_NID_MODINFO);
    if (prStub != NULL) {
        infTop = prStub->pvStub;
        infEnd = infTop + sizeof(SceModuleInfo) + 4;
        prSeg = gsegmap_add(prMap);
        if (prSeg != NULL) {
            gsegment_set(prSeg, infTop, infEnd, ".rodata.sceModuleInfo", 
                    SET_RoData);
        }
    }
    return GGAME_SUCCESS;
}
