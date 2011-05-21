#include <psptypes.h>
#include <stdio.h>
#include "dasmmodel.h"
#include "dmodelrow.h"
#include "mips.h"

SceUInt32 dasmmodel_get_address(DasmModel* prModel) {
    DmodelRow* prRow = NULL;
    SceUInt32 addr = 0;
    
    if (prModel != NULL) {
        prRow = dasmmodel_get_row(prModel, 0);
        if (prRow != NULL) {
            addr = prRow->uiAddress;
        }
    }
    return addr;
}

DmodelRow* dasmmodel_get_row(DasmModel* prModel, int index) {
    DmodelRow* prRow = NULL;
    if (prModel != NULL) {
        if ((index >= 0) && (index < prModel->iRows)) {
            prRow = &prModel->arRow[index];
        }
    }
    return prRow;
}

int dasmmodel_get_value(char* sDest, DasmModel* prModel, int row, int col) {
    DmodelRow *prRow = NULL;
    int r = 0;
    SceUInt32 vaddr = 0;
    
    if (prModel == NULL) {
        return DASMMODEL_NULLPTR;
    }
    if ((row < 0) || (row >= prModel->iRows)) {
        return DASMMODEL_INVIDX;
    }
    if ((col < 0) || (col >= DASMMODEL_COLS)) {
        return DASMMODEL_INVIDX;
    }
    
    prRow = dasmmodel_get_row(prModel, row);
    if (prRow == NULL) {
        return DASMMODEL_FAILURE;
    }
    
    switch (col) {
        case 0:
            vaddr = prRow->uiAddress & prModel->uiAddrMask;
            if (prModel->addrPrefix != 0) {
                sprintf(sDest, "0x%08X", vaddr);
            } else {
                sprintf(sDest, "%08X", vaddr);
            }
            break;
        case 1:
            sprintf(sDest, "%08X", prRow->uiValue);
            break;
        case 2:
            strcpy(sDest, prRow->sAssembly);
            break;
    }
    
    return DASMMODEL_SUCCESS;
}

int dasmmodel_init(DasmModel* prModel) {
    DmodelRow* prRow = NULL;
    int i = 0;
    int r = 0;
    
    if (prModel == NULL) {
        return DASMMODEL_NULLPTR;
    }
    
    /* Initialize the Row Array. */
    for (i = 0; i < DASMMODEL_MAXROWS; i++) {
        prRow = dasmmodel_get_row(prModel, i);
        if (prRow == NULL) {
            return DASMMODEL_FAILURE;
        }
        r = dmodelrow_init(prRow);
        if (r != DMODELROW_SUCCESS) {
            return DASMMODEL_FAILURE;
        }
    }
    
    /* Initialize the default number of model rows. */
    prModel->iRows = DASMMODEL_DEFROWS;
    
    /* Initialize the default virtual address mask. */
    prModel->uiAddrMask = DASMMODEL_DEFVAMASK;
    
    /* Initialize the address prefixing. */
    prModel->addrPrefix = 1;
    
    return DASMMODEL_SUCCESS;
}

int dasmmodel_set_address(DasmModel* prModel, SceUInt32 address) {
    DmodelRow* prRow = NULL;
    int i = 0;
    int r = 0;
    SceUInt32 raddr = 0;
    SceUInt32 vaddr = 0;
    SceUInt32* pVal = NULL;
    char sAssm[DMODELROW_ASSMLEN + 1];
    SceUInt32 oldaddr = 0;

    if (prModel == NULL) {
        return DASMMODEL_NULLPTR;
    }
    /* Don't update for nothing.  If the address hasn't changed, return. */
    oldaddr = dasmmodel_get_address(prModel);
    if (oldaddr == address) {
        return DASMMODEL_SUCCESS;
    }
    /* Update all of the visible rows in the Model. */
    for (i = 0; i < prModel->iRows; i++) {
        /* Compute the address of the Row. */
        raddr = address + (i * 4);
        prRow = dasmmodel_get_row(prModel, i);
        if (prRow == NULL) {
            return DASMMODEL_FAILURE;
        }
        /* Assign the Kernel Address of the Row. */
        if (dmodelrow_set_address(prRow, raddr) != DMODELROW_SUCCESS) {
            return DASMMODEL_FAILURE;
        }
        /* Assign the Value of the Row.*/
        pVal = (SceUInt32*)raddr;
        if (dmodelrow_set_value(prRow, *pVal) != DMODELROW_SUCCESS) {
            return DASMMODEL_FAILURE;
        }
        /* Assign the Assembly Language to the Row. */
        vaddr = raddr & prModel->uiAddrMask;
        mipsDecode(sAssm, *pVal, vaddr);
        if (dmodelrow_set_assembly(prRow, sAssm) != DMODELROW_SUCCESS) {
            return DASMMODEL_FAILURE;
        }
    }
    return DASMMODEL_SUCCESS;
}
