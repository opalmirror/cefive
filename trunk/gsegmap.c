#include "gsegmap.h"

Gsegment* gsegmap_add(GsegMap* prMap) {
    int count = 0;
    if (prMap == NULL) {
        return NULL;
    }
    count = prMap->count;
    prMap->count++;
    return &prMap->segment[count];
}

int gsegmap_clear(GsegMap* prMap) {
    Gsegment* prSeg = NULL;
    int i = 0;
    if (prMap == NULL) {
        return GSEGMAP_NULLPTR;
    }
    for (i = 0; i < GSEGMAP_SIZE; i++) {
        prSeg = &prMap->segment[i];
        if (gsegment_init(prSeg) < 0) {
            return GSEGMAP_FAILURE;
        }
    }
    prMap->count = 0;
    return GSEGMAP_SUCCESS;
}

Gsegment* gsegmap_find(GsegMap* prMap, const SceUInt32 address) {
    Gsegment* prSeg = NULL;
    int i = 0;
    if (prMap == NULL) {
        return NULL;
    }
    for (i = 0; i < prMap->count; i++) {
        prSeg = &prMap->segment[i];
        if ((address >= prMap->segment[i].startaddr) && 
                (address < prMap->segment[i].endaddr)) {
            return &prMap->segment[i];
        }
    }
    return NULL;
}

Gsegment* gsegmap_get(GsegMap* prMap, const int index) {
    if (prMap == NULL) {
        return NULL;
    }
    if ((index < 0) || (index >= prMap->count)) {
        return NULL;
    }
    return &prMap->segment[index];
}

int gsegmap_init(GsegMap* prMap) {
    if (prMap == NULL) {
        return GSEGMAP_NULLPTR;
    }
    if (gsegmap_clear(prMap) < 0) {
        return GSEGMAP_FAILURE;
    }
    return GSEGMAP_SUCCESS;
}
