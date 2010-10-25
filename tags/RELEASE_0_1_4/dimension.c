#include <stdlib.h>
#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "dimension.h"

int dimension_copy(Dimension* prDest, Dimension* prSrc) {
    if (prDest == NULL || prSrc == NULL) {
        return DIMENSION_NULLPTR;
    }
    prDest->width = prSrc->width;
    prDest->height = prSrc->height;
    return DIMENSION_SUCCESS;
}

int dimension_init(Dimension* prDim) {
    if (prDim == NULL) {
        return DIMENSION_NULLPTR;
    }
    prDim->height = 0;
    prDim->width = 0;
    return DIMENSION_SUCCESS;
}

int dimension_read(Dimension* prDim, SceUID fd) {
    int r = 0;
    if (prDim == NULL) {
        return DIMENSION_NULLPTR;
    }
    if (fd < 0) {
        return DIMENSION_IOERROR;
    }
    r = sceIoRead(fd, &prDim->width, sizeof(int));
    if (r != sizeof(int)) {
        return DIMENSION_IOERROR;
    }
    r = sceIoRead(fd, &prDim->height, sizeof(int));
    if (r != sizeof(int)) {
        return DIMENSION_IOERROR;
    }
    return DIMENSION_SUCCESS;
}

int dimension_set(Dimension* prDim, int width, int height) {
    if (prDim == NULL) {
        return DIMENSION_NULLPTR;
    }
    prDim->width = width;
    prDim->height = height;
    return DIMENSION_SUCCESS;
}

int dimension_write(Dimension* prDim, SceUID fd) {
    int r = 0;
    if (prDim == NULL) {
        return DIMENSION_NULLPTR;
    }
    if (fd < 0) {
        return DIMENSION_IOERROR;
    }
    r = sceIoWrite(fd, &prDim->width, sizeof(int));
    if (r != sizeof(int)) {
        return DIMENSION_IOERROR;
    }
    r = sceIoWrite(fd, &prDim->height, sizeof(int));
    if (r != sizeof(int)) {
        return DIMENSION_IOERROR;
    }
    return DIMENSION_SUCCESS;
}
