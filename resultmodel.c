#include "resultmodel.h"

int result_init(Result* prResult) {
    if (prResult == NULL) {
        return RESULTMODEL_NULLPTR;
    }
    prResult->resultType = SRT_None;
    prResult->address = 0;
    prResult->value = 0;
    prResult->text[0] = (char)0;
    return RESULTMODEL_SUCCESS;
}

Result* resultmodel_add(ResultModel* prModel) {
    int index = 0;
    if (prModel != NULL) {
        index = prModel->count;
        if (index + 1 > RESULTMODEL_SIZE) {
            return NULL;
        }
        prModel->count++;
        return &prModel->result[index];
    }
    return NULL;
}

int resultmodel_clear(ResultModel* prModel) {
    int i = 0;
    if (prModel == NULL) {
        return RESULTMODEL_NULLPTR;
    }
    for (i = 0; i < RESULTMODEL_SIZE; i++) {
        if (result_init(&prModel->result[i]) < 0) {
            return RESULTMODEL_FAILURE;
        }
    }
    prModel->count = 0;
    return RESULTMODEL_SUCCESS;
}

Result* resultmodel_get(ResultModel* prModel, const int index) {
    if (prModel == NULL) {
        return NULL;
    }
    if ((index < 0) || (index >= prModel->count)) {
        return NULL;
    }
    return &prModel->result[index];
}

int resultmodel_init(ResultModel* prModel) {
    if (prModel == NULL) {
        return RESULTMODEL_NULLPTR;
    }
    if (resultmodel_clear(prModel) < 0) {
        return RESULTMODEL_FAILURE;
    }
    return RESULTMODEL_SUCCESS;
}
