#include "cesearch.h"

static int cesearch_run(SceSize rLen, void* argv);
static int cesearch_step(CESearch* prSearch);
static int find_byte(CESearch* prSearch);
static int find_half(CESearch* prSearch);
static int find_word(CESearch* prSearch);

SearchQuery* cesearch_get_query(CESearch* prSearch) {
    if (prSearch == NULL) {
        return NULL;
    }
    return &prSearch->searchQuery;
}

ResultModel* cesearch_get_resultmodel(CESearch* prSearch) {
    if (prSearch == NULL) {
        return NULL;
    }
    return &prSearch->resultModel;
}

int cesearch_init(CESearch* prSearch) {
    if (prSearch == NULL) {
        return CESEARCH_NULLPTR;
    }
    if (resultmodel_init(&prSearch->resultModel) < 0) {
        return CESEARCH_FAILURE;
    }
    if (searchquery_init(&prSearch->searchQuery) < 0) {
        return CESEARCH_FAILURE;
    }
    prSearch->eventId = sceKernelCreateEventFlag(CESEARCH_EVENT_NAME, 
            PSP_EVENT_WAITMULTIPLE, 0, NULL);
    if (prSearch->eventId < 0) {
        return CESEARCH_FAILURE;
    }
    return CESEARCH_SUCCESS;
}

static int cesearch_run(SceSize rLen, void* argv) {
    CESearch* prSearch = NULL;
    SearchQuery* prQuery = NULL;
    
    if (rLen != sizeof(CESearch)) {
        return CESEARCH_FAILURE;
    }
    prSearch = (CESearch*) argv;
    prQuery = &prSearch->searchQuery;
    prSearch->position = prQuery->startAddress;
    
    prSearch->searchState = SES_Running;
    
    while (prSearch->searchState == SES_Running) {
        if (cesearch_step(prSearch) < 0) {
            prSearch->searchState = SES_Error;
            continue;
        }
    }
    return CESEARCH_SUCCESS;
}

int cesearch_start(CESearch* prSearch) {
    
}

static int cesearch_step(CESearch* prSearch) {
    SearchQuery* prQuery = NULL;
    if (prSearch == NULL) {
        return CESEARCH_NULLPTR;
    }
    prQuery = &prSearch->searchQuery;
    if (prSearch->position < prQuery->startAddress) {
        prSearch->position = prQuery->startAddress;
    }
    if (prSearch->position >= prQuery->endAddress) {
        prSearch->searchState = SES_Finished;
        return CESEARCH_SUCCESS;
    }
    switch (prQuery->searchMode) {
        case SMD_Byte:
            switch (prQuery->searchSize) {
                case SSZ_Byte:
                    if (find_byte(prSearch) < 0) {
                        return CESEARCH_FAILURE;
                    }
                    break;
                case SSZ_Half:
                    if (find_half(prSearch) < 0) {
                        return CESEARCH_FAILURE;
                    }
                    break;
                case SSZ_Word:
                    if (find_word(prSearch) < 0) {
                        return CESEARCH_FAILURE;
                    }
                    break;
            }
            break;
        case SMD_Immediate:
            break;
        case SMD_Text:
            break;
    }
    return CESEARCH_SUCCESS;
}

int cesearch_stop(CESearch* prSearch) {
    
}

static int find_byte(CESearch* prSearch) {
    SceUChar8* pVal = NULL;
    SearchQuery* pQuery = NULL;
    SceUChar8 rTest = 0;
    Result* prResult = NULL;
    ResultModel* prModel = NULL;
    
    if (prSearch == NULL) {
        return CESEARCH_NULLPTR;
    }
    pQuery = &prSearch->searchQuery;
    rTest = (SceUChar8)(pQuery->value & 0xFF);
    pVal = (SceUChar8*) prSearch->position;
    if (rTest == *pVal) {
        prModel = &prSearch->resultModel;
        prResult = resultmodel_add(prModel);
        if (prResult == NULL) {
            return CESEARCH_FAILURE;
        }
        prResult->address = prSearch->position;
        prResult->resultType = SRT_UChar8;
        prResult->value = *pVal;
        sceKernelSetEventFlag(prSearch->eventId, CESEARCH_E_HIT);
    }
    prSearch->position += sizeof(SceUChar8);
    return CESEARCH_SUCCESS;
}

static int find_half(CESearch* prSearch) {
    SceUShort16* pVal = NULL;
    SearchQuery* pQuery = NULL;
    SceUShort16 rTest = 0;
    Result* prResult = NULL;
    ResultModel* prModel = NULL;
    
    if (prSearch == NULL) {
        return CESEARCH_NULLPTR;
    }
    pQuery = &prSearch->searchQuery;
    rTest = (SceUShort16)(pQuery->value & 0xFFFF);
    pVal = (SceUShort16*) prSearch->position;
    if (rTest == *pVal) {
        prModel = &prSearch->resultModel;
        prResult = resultmodel_add(prModel);
        if (prResult == NULL) {
            return CESEARCH_FAILURE;
        }
        prResult->address = prSearch->position;
        prResult->resultType = SRT_UShort16;
        prResult->value = *pVal;
        sceKernelSetEventFlag(prSearch->eventId, CESEARCH_E_HIT);
    }
    prSearch->position += sizeof(SceUShort16);
    return CESEARCH_SUCCESS;
}

static int find_word(CESearch* prSearch) {
    SceUInt32* pVal = NULL;
    SearchQuery* pQuery = NULL;
    Result* prResult = NULL;
    ResultModel* prModel = NULL;
    
    if (prSearch == NULL) {
        return CESEARCH_NULLPTR;
    }
    pQuery = &prSearch->searchQuery;
    pVal = (SceUInt32*) prSearch->position;
    if (*pVal == pQuery->value) {
        prModel = &prSearch->resultModel;
        prResult = resultmodel_add(prModel);
        if (prResult == NULL) {
            return CESEARCH_FAILURE;
        }
        prResult->address = prSearch->position;
        prResult->resultType = SRT_UInt32;
        prResult->value = *pVal;
        sceKernelSetEventFlag(prSearch->eventId, CESEARCH_E_HIT);
    }
    prSearch->position += sizeof(SceUInt32);
    return CESEARCH_SUCCESS;
}
