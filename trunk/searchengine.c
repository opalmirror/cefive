#include <pspkerneltypes.h>
#include "searchengine.h"

static int find_next_byte(SearchEngine* prEngine) {
    int r = SEARCHENGINE_NOT_FOUND;
    SceUChar8 *pucVal = NULL;
    SceUChar8 qVal = (SceUChar8)0;
    SceUInt32 position = 0;

    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    position = searchengine_tell(prEngine);
    qVal = (SceUChar8)(prEngine->rQuery.value & 0xFF);
    pucVal = (SceUChar8*)position;
    if (qVal == *pucVal) {
        searchengine_add_result(prEngine, pucVal, *pucVal);
        r = SEARCHENGINE_SUCCESS;
    }
    r = searchengine_seek(prEngine, position + 1);

    return r;
}

static int find_next_word(SearchEngine* prEngine) {
    int r = SEARCHENGINE_NOT_FOUND;
    SceUShort16 *punVal = NULL;
    SceUShort16 qVal = (SceUShort16)0;
    SceUInt32 position = 0;

    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    position = searchengine_tell(prEngine);
    qVal = (SceUShort16)(prEngine->rQuery.value & 0xFFFF);
    punVal = (SceUShort16*)position;
    if (qVal == *punVal) {
        searchengine_add_result(prEngine, punVal, *punVal);
        r = SEARCHENGINE_SUCCESS;
    }
    r = searchengine_seek(prEngine, position + 2);
    return r;
}

static int find_next_dword(SearchEngine* prEngine) {
    int r = SEARCHENGINE_NOT_FOUND;
    SceUInt32 *puiVal = NULL;
    SceUInt32 qVal = 0;
    SceUInt32 position = 0;

    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    position = searchengine_tell(prEngine);
    qVal = prEngine->rQuery.value;
    puiVal = (SceUInt32*)position;
    if (qVal == *puiVal) {
        searchengine_add_result(prEngine, puiVal, *puiVal);
        r = SEARCHENGINE_SUCCESS;
    }
    r = searchengine_seek(prEngine, position + 4);
    return r;
}

static int find_next_immediate(SearchEngine *prEngine) {
    int r = SEARCHENGINE_NOT_FOUND;
    SceUInt32 *puiVal = NULL;
    SceUInt32 qVal = 0;
    SceUInt32 position = 0;
    SceShort16 insval = 0;
    SceShort16 qimval = 0;
    
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    position = searchengine_tell(prEngine);
    qVal = prEngine->rQuery.value;
    qimval = (SceShort16)(qVal & 0xFFFF);
    puiVal = (SceUInt32*)position;
    insval = (SceShort16)(*puiVal & 0xFFFF);
    if (qimval == insval) {
        searchengine_add_result(prEngine, puiVal, *puiVal);
        r = SEARCHENGINE_SUCCESS;
    }
    r = searchengine_seek(prEngine, position + 4);
    return r;
}

int searchengine_add_result(SearchEngine* prEngine, unsigned int address,
        unsigned int value) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    if (prEngine->result_count >= SEARCHENGINE_MAXRESULTS) {
        prEngine->rState = ESS_Finished;
        return SEARCHENGINE_SUCCESS;
    }
    SearchResult *prResult = &(prEngine->arResult[prEngine->result_count]);
    prResult->address = address & ~0x40000000;
    prResult->value = value;
    prEngine->result_count++;
    return SEARCHENGINE_SUCCESS;
}

int searchengine_cancel(SearchEngine* prEngine) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    if (prEngine->rState != ESS_Searching) {
        return SEARCHENGINE_FAILURE;
    }
    prEngine->rState = ESS_Finished;
    return SEARCHENGINE_SUCCESS;
}

int searchengine_cycle_mode(SearchEngine* prEngine) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    ESearchMode rMode = prEngine->rQuery.searchMode;
    if (rMode == ESM_Byte) {
        rMode = ESM_Immediate;
    } else {
        rMode++;
    }
    prEngine->rQuery.searchMode = rMode;
    return SEARCHENGINE_SUCCESS;
}

int searchengine_cycle_size(SearchEngine* prEngine) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    ESearchSize rSize = prEngine->rQuery.searchSize;
    if (rSize == ESZ_Dword) {
        rSize = ESZ_Byte;
    } else {
        rSize++;
    }
    prEngine->rQuery.searchSize = rSize;
    return SEARCHENGINE_SUCCESS;
}

int searchengine_find_next(SearchEngine* prEngine) {
    SearchQuery* prQuery = NULL;
    SceUInt32 pos = 0;
    int r = SEARCHENGINE_NOT_FOUND;
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    prQuery = &prEngine->rQuery;
    pos = prEngine->position;
    if (pos < prQuery->startAddr) {
        searchengine_seek(prEngine, prQuery->startAddr);
    }
    if (pos > prQuery->endAddr) {
        prEngine->rState = ESS_Finished;
        return SEARCHENGINE_SUCCESS;
    }
    ESearchSize rSS = prQuery->searchSize;
    ESearchMode rSM = prQuery->searchMode;
    if (rSM == ESM_Byte) {
        switch (rSS) {
            case ESZ_Byte:
                r = find_next_byte(prEngine);
                break;
            case ESZ_Dword:
                r = find_next_dword(prEngine);
                break;
            case ESZ_Word:
                r = find_next_word(prEngine);
                break;
        }
    }
    if (rSM == ESM_Immediate) {
        r = find_next_immediate(prEngine);
    }
    if (r == SEARCHENGINE_BAD_ADDR) {
        prEngine->rState = ESS_Finished;
        r = SEARCHENGINE_SUCCESS;
    }

    return r;
}

SearchResult* searchengine_get_result(SearchEngine* prEngine, int index) {
    SearchResult* prResult = NULL;
    if (prEngine != NULL) {
        if (index >= 0 && index < prEngine->result_count) {
            prResult = &(prEngine->arResult[index]);
        }
    }
    return prResult;
}

int searchengine_init(SearchEngine* prEngine) {
    SearchResult* prResult = NULL;
    int i = 0;
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    prEngine->rConfig.max_position = 0x49FFFFFF;
    prEngine->rConfig.min_position = 0x48800000;
    prEngine->position = prEngine->rConfig.min_position;
    prEngine->rState = ESS_Stopped;
    prEngine->result_count = 0;
    prEngine->rQuery.searchMode = ESM_Byte;
    prEngine->rQuery.searchSize = ESZ_Dword;
    prEngine->rQuery.startAddr = prEngine->rConfig.min_position;
    prEngine->rQuery.endAddr = prEngine->rConfig.max_position;
    for (i = 0; i < SEARCHENGINE_MAXRESULTS; i++) {
        prResult = &(prEngine->arResult[i]);
        prResult->address = 0;
        prResult->value = 0;
        prResult->text[0] = (char)0;
    }
    prEngine->rConfig.iterations_per_cycle = SEARCHENGINE_DEFIPC;
    return SEARCHENGINE_SUCCESS;
}

int searchengine_get_result_count(SearchEngine* prEngine) {
    int count = 0;
    if (prEngine != NULL) {
        count = prEngine->result_count;
    }
    return count;
}

int searchengine_reset(SearchEngine* prEngine) {
    SearchResult* prResult = NULL;
    int i = 0;
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    prEngine->result_count = 0;
    for (i = 0; i < SEARCHENGINE_MAXRESULTS; i++) {
        prResult = &(prEngine->arResult[i]);
        prResult->address = 0;
        prResult->value = 0;
        prResult->text[0] = (char)0;
    }
    prEngine->position = prEngine->rConfig.min_position;
    prEngine->rState = ESS_Idle;

    return SEARCHENGINE_SUCCESS;
}

int searchengine_run(SearchEngine* prEngine) {
    ESearchState rState = ESS_Fault;
    int ipc = 0;
    int i = 0;
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    ipc = prEngine->rConfig.iterations_per_cycle;

    for (i = 0; i < ipc; i++) {
        rState = prEngine->rState;
        switch (rState) {
            case ESS_Fault:
                break;
            case ESS_Stopped:
                break;
            case ESS_Idle:
                break;
            case ESS_Searching:
                searchengine_find_next(prEngine);
                break;
            case ESS_Finished:
                break;
        }
    }

    return SEARCHENGINE_SUCCESS;
}

int searchengine_search(SearchEngine* prEngine) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    if (prEngine->rState != ESS_Idle) {
        // Cannot start a search unless the Engine is idle.
        return SEARCHENGINE_FAILURE;
    }
    searchengine_reset(prEngine);
    prEngine->rState = ESS_Searching;

    return SEARCHENGINE_SUCCESS;
}

int searchengine_seek(SearchEngine* prEngine, SceUInt32 position) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    if (position < prEngine->rConfig.min_position ||
            position > prEngine->rConfig.max_position) {
        return SEARCHENGINE_BAD_ADDR;
    }
    prEngine->position = position;

    return SEARCHENGINE_SUCCESS;
}

int searchengine_start(SearchEngine* prEngine) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }
    prEngine->rState = ESS_Idle;

    return SEARCHENGINE_SUCCESS;
}

int searchengine_stop(SearchEngine* prEngine) {
    if (prEngine == NULL) {
        return SEARCHENGINE_MEMORY;
    }

    prEngine->rState = ESS_Stopped;
    return SEARCHENGINE_SUCCESS;
}

SceUInt32 searchengine_tell(SearchEngine *prEngine) {
    SceUInt32 pos = 0;
    if (prEngine != NULL) {
        pos = prEngine->position;
    }
    return pos;
}