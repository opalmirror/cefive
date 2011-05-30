#include "gstubmap.h"

ModStub* gstubmap_add(GstubMap* prMap) {
    int count = 0;
    if (prMap == NULL) {
        return NULL;
    }
    count = prMap->count;
    if (count + 1 >= GSTUBMAP_SIZE) {
        return NULL;
    }
    prMap->count++;
    return &prMap->stub[count];
}

int gstubmap_clear(GstubMap* prMap) {
    ModStub* prStub = NULL;
    int i = 0;
    if (prMap == NULL) {
        return GSTUBMAP_NULLPTR;
    }
    for (i = 0; i < GSTUBMAP_SIZE; i++) {
        prStub = &prMap->stub[i];
        if (modstub_init(prStub) < 0) {
            return GSTUBMAP_FAILURE;
        }
    }
    prMap->count = 0;
    return GSTUBMAP_SUCCESS;
}

ModStub* gstubmap_find(GstubMap* prMap, const SceUInt32 nId) {
    int i = 0;
    if (prMap == NULL) {
        return NULL;
    }
    for (i = 0; i < prMap->count; i++) {
        if (nId == prMap->stub[i].nId) {
            return &prMap->stub[i];
        }
    }
    return NULL;
}

ModStub* gstubmap_get(GstubMap* prMap, const int index) {
    if (prMap == NULL) {
        return NULL;
    }
    if ((index < 0) || (index >= prMap->count)) {
        return NULL;
    }
    return &prMap->stub[index];
}

int gstubmap_init(GstubMap* prMap) {
    if (prMap == NULL) {
        return GSTUBMAP_NULLPTR;
    }
    if (gstubmap_clear(prMap) < 0) {
        return GSTUBMAP_FAILURE;
    }
    return GSTUBMAP_SUCCESS;
}
