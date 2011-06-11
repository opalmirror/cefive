#include "cesearch.h"

static int cesearch_run(SceSize rLen, void* argv);

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
    return CESEARCH_SUCCESS;
}

static int cesearch_run(SceSize rLen, void* argv) {
    CESearch* prSearch = NULL;
    if (rLen != sizeof(CESearch)) {
        return CESEARCH_FAILURE;
    }
    prSearch = (CESearch*) argv;
    
    return CESEARCH_SUCCESS;
}

int cesearch_start(CESearch* prSearch) {
    
}

int cesearch_stop(CESearch* prSearch) {
    
}
