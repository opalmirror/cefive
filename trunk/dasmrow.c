#include <psptypes.h>
#include "dasmrow.h"
#include "addresscolumn.h"
#include "dwordcolumn.h"
#include "textcolumn.h"
#include "mips.h"

SceUInt32 dasmrow_get_address(DasmRow* prRow) {
    AddressColumn* prCol = NULL;
    SceUInt32 address = 0;
    if (prRow != NULL) {
        prCol = dasmrow_get_addresscolumn(prRow);
        if (prCol != NULL) {
            address = prCol->value & 0xFFFFFFFF;
        }
    }
    return address;
}

AddressColumn* dasmrow_get_addresscolumn(DasmRow* prRow) {
    AddressColumn* prCol = NULL;
    if (prRow != NULL) {
        prCol = &prRow->rAddress;
    }
    return prCol;
}

TextColumn* dasmrow_get_assemblycolumn(DasmRow* prRow) {
    TextColumn* prCol = NULL;
    if (prRow != NULL) {
        prCol = &prRow->rAssembly;
    }
    return prCol;
}

SceUInt32 dasmrow_get_displaymask(DasmRow* prRow) {
    AddressColumn* prCol = NULL;
    SceUInt32 mask = 0;
    if(prRow == NULL) {
        return 0;
    }
    prCol = dasmrow_get_addresscolumn(prRow);
    if (prCol == NULL) {
        return 0;
    }
    mask = addresscolumn_get_displaymask(prCol);
    return mask;
}

SceUInt32 dasmrow_get_vaddr(DasmRow* prRow) {
    AddressColumn* prCol = NULL;
    SceUInt32 vaddr = 0;
    if (prRow != NULL) {
        prCol = dasmrow_get_addresscolumn(prRow);
        if (prCol != NULL) {
            vaddr = addresscolumn_get_vaddr(prCol);
        }
    }
    return vaddr;
}

SceUInt32 dasmrow_get_value(DasmRow* prRow) {
    DwordColumn* prCol = NULL;
    SceUInt32 value = 0;
    if (prRow != NULL) {
        prCol = dasmrow_get_valuecolumn(prRow);
        if (prCol != NULL) {
            value = prCol->value & 0xFFFFFFFF;
        }
    }
    return value;
}

DwordColumn* dasmrow_get_valuecolumn(DasmRow* prRow) {
    DwordColumn* prCol = NULL;
    if (prRow != NULL) {
        prCol = &prRow->rValue;
    }
    return prCol;
}

int dasmrow_init(DasmRow* prRow) {
    AddressColumn *prAddress = NULL;
    DwordColumn *prValue = NULL;
    TextColumn *prAssm = NULL;
    int r = 0;
    if(prRow == NULL) {
        return DASMROW_NULLPTR;
    }
    prAddress = dasmrow_get_addresscolumn(prRow);
    r = addresscolumn_init(prAddress);
    if (r != ADDRESSCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    prValue = dasmrow_get_valuecolumn(prRow);
    r = dwordcolumn_init(prValue);
    if (r != DWORDCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    prAssm = dasmrow_get_assemblycolumn(prRow);
    r = textcolumn_init(prAssm);
    if (r != TEXTCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    return DASMROW_SUCCESS;
}

int dasmrow_redraw(DasmRow* prRow) {
    AddressColumn *prAddress = NULL;
    DwordColumn *prValue = NULL;
    TextColumn *prAssm = NULL;
    int r = 0;
    if(prRow == NULL) {
        return DASMROW_NULLPTR;
    }
    prAddress = dasmrow_get_addresscolumn(prRow);
    r = addresscolumn_redraw(prAddress);
    if (r != ADDRESSCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    prValue = dasmrow_get_valuecolumn(prRow);
    r = dwordcolumn_redraw(prValue);
    if (r != DWORDCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    prAssm = dasmrow_get_assemblycolumn(prRow);
    r = textcolumn_redraw(prAssm);
    if (r != TEXTCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    
    return DASMROW_SUCCESS;
}

int dasmrow_set_address(DasmRow* prRow, SceUInt32 address) {
    AddressColumn* prCol = NULL;
    int r = 0;
    if (prRow == NULL) {
        return DASMROW_NULLPTR;
    }
    prCol = dasmrow_get_addresscolumn(prRow);
    if (prCol == NULL) {
        return DASMROW_FAILURE;
    }
    r = addresscolumn_setvalue(prCol, address);
    if (r != ADDRESSCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    return DASMROW_SUCCESS;
}

int dasmrow_set_assembly(DasmRow* prRow, const char* sAssm) {
    TextColumn* prCol = NULL;
    int r = 0;
    if ((prRow == NULL) || (sAssm == NULL)) {
        return DASMROW_NULLPTR;
    }
    prCol = dasmrow_get_assemblycolumn(prRow);
    if (prCol == NULL) {
        return DASMROW_FAILURE;
    }
    r = textcolumn_settext(prCol, sAssm);
    if (r != TEXTCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    return DASMROW_SUCCESS;
}

int dasmrow_set_color(DasmRow* prRow, ColorConfig* prColor) {
    ColorConfig* prDest = NULL;
    AddressColumn *prAddr = NULL;
    DwordColumn* prValue = NULL;
    TextColumn* prAssm = NULL;
    int r = 0;
    if ((prRow == NULL) || (prColor == NULL)) {
        return DASMROW_NULLPTR;
    }
    prAddr = dasmrow_get_addresscolumn(prRow);
    if (prAddr == NULL) {
        return DASMROW_FAILURE;
    }
    prDest = addresscolumn_get_displaycolor(prAddr);
    if (prDest == NULL) {
        return DASMROW_FAILURE;
    }
    r = colorconfig_copy(prDest, prColor);
    if (r != COLORCONFIG_SUCCESS) {
        return DASMROW_FAILURE;
    }
    prValue = dasmrow_get_valuecolumn(prRow);
    if (prValue == NULL) {
        return DASMROW_FAILURE;
    }
    prDest = dwordcolumn_get_displaycolor(prValue);
    if (prDest == NULL) {
        return DASMROW_FAILURE;
    }
    r = colorconfig_copy(prDest, prColor);
    if (r != COLORCONFIG_SUCCESS) {
        return DASMROW_FAILURE;
    }
    prAssm = dasmrow_get_assemblycolumn(prRow);
    if (prAssm == NULL) {
        return DASMROW_FAILURE;
    }
    prDest = textcolumn_get_displaycolor(prAssm);
    if (prDest == NULL) {
        return DASMROW_FAILURE;
    }
    r = colorconfig_copy(prDest, prColor);
    if (r != COLORCONFIG_SUCCESS) {
        return DASMROW_FAILURE;
    }    
    
    return DASMROW_SUCCESS;
}

int dasmrow_set_displaymask(DasmRow* prRow, SceUInt32 mask) {
    AddressColumn* prCol = NULL;
    int r = 0;
    if (prRow == NULL) {
        return DASMROW_NULLPTR;
    }
    prCol = dasmrow_get_addresscolumn(prRow);
    if (prCol == NULL) {
        return DASMROW_FAILURE;
    }
    r = addresscolumn_set_displaymask(prCol, mask);
    if (r != ADDRESSCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    return DASMROW_SUCCESS;
}

int dasmrow_set_value(DasmRow* prRow, SceUInt32 value) {
    DwordColumn* prCol = NULL;
    int r = 0;
    if (prRow == NULL) {
        return DASMROW_NULLPTR;
    }
    prCol = dasmrow_get_valuecolumn(prRow);
    if (prCol == NULL) {
        return DASMROW_FAILURE;
    }
    r = dwordcolumn_setvalue(prCol, value);
    if (r != DWORDCOLUMN_SUCCESS) {
        return DASMROW_FAILURE;
    }
    
    return DASMROW_SUCCESS;
}

int dasmrow_update(DasmRow* prRow) {
    if (prRow == NULL) {
        return DASMROW_NULLPTR;
    }
    SceUInt32 uiAddress = 0;
    SceUInt32 uiVaddr = 0;
    SceUInt32* puiValue = NULL;
    int r = 0;
    char sBuf[81];
    
    /* Get the Kernel Address from the Row. */
    uiAddress = dasmrow_get_address(prRow);
    /* Get the Virtual Address from the Row. */
    uiVaddr = dasmrow_get_vaddr(prRow);
    if (uiAddress != 0) {
        /* Get the value of the indicated address. */
        puiValue = (SceUInt32 *)uiAddress;
        /* Set the value of the Row. */
        r = dasmrow_set_value(prRow, *puiValue);
        if (r != DASMROW_SUCCESS) {
            return DASMROW_FAILURE;
        }
        /* Decode the value as a MIPS Instruction. */
        mipsDecode(sBuf, *puiValue, uiVaddr);
        /* Assign the Assembly Language to the Row. */
        r = dasmrow_set_assembly(prRow, sBuf);
        if (r != DASMROW_SUCCESS) {
            return DASMROW_FAILURE;
        }
    }
    return DASMROW_SUCCESS;
}
