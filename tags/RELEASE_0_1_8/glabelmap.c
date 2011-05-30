#include "glabelmap.h"

Glabel* glabelmap_add(GlabelMap* prMap) {
    int count = 0;
    if (prMap == NULL) {
        return NULL;
    }
    count = prMap->count;
    if (count + 1 >= GLABELMAP_SIZE) {
        return NULL;
    }
    prMap->count++;
    return &prMap->label[count];
}

int glabelmap_clear(GlabelMap* prMap) {
    Glabel* prLabel = NULL;
    int i = 0;
    if (prMap == NULL) {
        return GLABELMAP_NULLPTR;
    }
    for (i = 0; i < GLABELMAP_SIZE; i++) {
        prLabel = &prMap->label[i];
        if (glabel_init(prLabel) < 0) {
            return GLABELMAP_FAILURE;
        }
    }
    prMap->count = 0;
    return GLABELMAP_SUCCESS;
}

Glabel* glabelmap_find(GlabelMap* prMap, const SceUInt32 address) {
    int i = 0;
    if (prMap == NULL) {
        return NULL;
    }
    for (i = 0; i < prMap->count; i++) {
        if (prMap->label[i].address == address) {
            return &prMap->label[i];
        }
    }
    return NULL;
}

Glabel* glabelmap_get(GlabelMap* prMap, const int index) {
    if (prMap == NULL) {
        return NULL;
    }
    if ((index < 0) || (index >= prMap->count)) {
        return NULL;
    }
    return &prMap->label[index];
}

int glabelmap_init(GlabelMap* prMap) {
    if (prMap == NULL) {
        return GLABELMAP_NULLPTR;
    }
    if (glabelmap_clear(prMap) < 0) {
        return GLABELMAP_FAILURE;
    }
    return GLABELMAP_SUCCESS;
}
