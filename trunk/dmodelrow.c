#include <psptypes.h>
#include <stdio.h>
#include "dmodelrow.h"

int dmodelrow_init(DmodelRow* prRow) {
    if (prRow == NULL) {
        return DMODELROW_NULLPTR;
    }
    if (dmodelrow_set_address(prRow, DMODELROW_DEFADDR) != DMODELROW_SUCCESS) {
        return DMODELROW_FAILURE;
    }
    if (dmodelrow_set_value(prRow, DMODELROW_DEFVALUE) != DMODELROW_SUCCESS) {
        return DMODELROW_FAILURE;
    }
    if (dmodelrow_set_assembly(prRow, DMODELROW_DEFASSM) != DMODELROW_SUCCESS) {
        return DMODELROW_FAILURE;
    }
    return DMODELROW_SUCCESS;
}

int dmodelrow_set_address(DmodelRow* prRow, const SceUInt32 address) {
    if (prRow == NULL) {
        return DMODELROW_NULLPTR;
    }
    prRow->uiAddress = address;
    return DMODELROW_SUCCESS;
}

int dmodelrow_set_assembly(DmodelRow* prRow, const char* sAssm) {
    if (prRow == NULL) {
        return DMODELROW_NULLPTR;
    }
    if (sAssm == NULL) {
        return DMODELROW_FAILURE;
    }
    strcpy(prRow->sAssembly, sAssm);
    return DMODELROW_SUCCESS;
}

int dmodelrow_set_value(DmodelRow* prRow, const SceUInt32 value) {
    if (prRow == NULL) {
        return DMODELROW_NULLPTR;
    }
    prRow->uiValue = value;    
    return DMODELROW_SUCCESS;
}
