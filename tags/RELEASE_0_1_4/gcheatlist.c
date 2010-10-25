#include <stdlib.h>
#include "gcheat.h"
#include "gcheatlist.h"

int gcheatlist_add(GCheatList* prList, GCheat* prCheat) {
    int idx = 0;
    if (prList == NULL || prCheat == NULL) {
        return GCHEATLIST_NULLPTR;
    }
    if (prList->size + 1 > GCHEATLIST_MAX_SIZE) {
        return GCHEATLIST_MEMORY;
    }
    idx = prList->size;
    prList->aprCheat[idx] = prCheat;
    prList->size++;

    return GCHEATLIST_SUCCESS;
}

int gcheatlist_clear(GCheatList* prList) {
    int i = 0;

    if (prList == NULL) {
        return GCHEATLIST_NULLPTR;
    }
    for (i = 0; i < GCHEATLIST_MAX_SIZE; i++) {
        prList->aprCheat[i] = NULL;
    }
    prList->size = 0;
    return GCHEATLIST_SUCCESS;
}

Cheat* gcheatlist_get(GCheatList* prList, int index) {
    Cheat* prCheat = NULL;
    if (prList != NULL) {
        if (index >= 0 && index < prList->size) {
            prCheat = prList->aprCheat[index];
        }
    }
    return prCheat;
}

int gcheatlist_init(GCheatList* prList) {
    return gcheatlist_clear(prList);
}

int gcheatlist_move_down(GCheatList* prList, int index) {
    Cheat* prCheat = NULL;
    if (prList == NULL) {
        return GCHEATLIST_NULLPTR;
    }
    if (index < 0 || index >= prList->size) {
        return GCHEATLIST_INVIDX;
    }
    if (index < prList->size - 1) {
        prCheat = prList->aprCheat[index];
        prList->aprCheat[index] = prList->aprCheat[index + 1];
        prList->aprCheat[index + 1] = prCheat;
    }

    return GCHEATLIST_SUCCESS;
}

int gcheatlist_move_up(GCheatList* prList, int index) {
    GCheat* prCheat = NULL;
    if (prList == NULL) {
        return GCHEATLIST_NULLPTR;
    }
    if (index < 0 || index >= prList->size) {
        return GCHEATLIST_INVIDX;
    }
    if (index > 0) {
        prCheat = prList->aprCheat[index];
        prList->aprCheat[index] = prList->aprCheat[index - 1];
        prList->aprCheat[index - 1] = prCheat;
    }

    return GCHEATLIST_SUCCESS;
}

GCheat* gcheatlist_remove(GCheatList* prList, int index) {
    GCheat* prOld = NULL;
    int i = 0;
    if (prList != NULL) {
        if (index >= 0 && index < prList->size) {
            prOld = prList->aprCheat[index];
            for (i = index; i < prList->size - 1; i++) {
                prList->aprCheat[i] = prList->aprCheat[i + 1];
            }
            prList->aprCheat[prList->size] = NULL;
            prList->size--;
        }
    }
    return prOld;
}

GCheat* gcheatlist_set(GCheatList* prList, int index, GCheat* prCheat) {
    GCheat* prOld = NULL;
    if (prList == NULL || prCheat == NULL) {
        return prOld;
    }
    if (index < 0 || index >= prList->size) {
        return prOld;
    }
    prOld = gcheatlist_get(prList, index);
    if (prOld != NULL) {
        prList->aprCheat[index] = prCheat;
    }
    return prOld;
}
