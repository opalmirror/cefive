#include "modstub.h"

int modstub_copy(ModStub* prDest, ModStub* prSrc) {
    if ((prDest == NULL) || (prSrc == NULL)) {
        return MODSTUB_NULLPTR;
    }
    prDest->stubType = prSrc->stubType;
    prDest->nId = prSrc->nId;
    prDest->pvStub = prSrc->pvStub;
    if (prSrc->name != NULL) {
        strncpy(prDest->name, prSrc->name, MODSTUB_NAME_LEN);
    }
    if (prSrc->module != NULL) {
        strncpy(prDest->module, prSrc->module, MODSTUB_MODNAME_LEN);
    }
    return MODSTUB_SUCCESS;
}

int modstub_init(ModStub* prStub) {
    if (prStub == NULL) {
        return MODSTUB_NULLPTR;
    }
    prStub->stubType = MST_Function;
    prStub->nId = 0;
    prStub->pvStub = NULL;
    memset(prStub->name, 0, MODSTUB_NAME_LEN + 1);
    memset(prStub->module, 0, MODSTUB_MODNAME_LEN + 1);
    return MODSTUB_SUCCESS;
}

int modstub_set(ModStub* prStub, EStubType stubType, SceUInt32 nId,
        void* pvStub, const char* name, const char* modname) {
    if (prStub == NULL) {
        return MODSTUB_NULLPTR;
    }
    prStub->stubType = stubType;
    prStub->nId = nId;
    prStub->pvStub = pvStub;
    if (name != NULL) {
        strncpy(prStub->name, name, MODSTUB_NAME_LEN);
    }
    if (modname != NULL) {
        strncpy(prStub->module, modname, MODSTUB_MODNAME_LEN);
    }
    return MODSTUB_SUCCESS;
}
