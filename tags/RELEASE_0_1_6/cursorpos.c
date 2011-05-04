#include <stdlib.h>
#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "cursorpos.h"

int cursorpos_copy(CursorPos* prDest, CursorPos* prSrc) {
    if (prDest == NULL || prSrc == NULL) {
        return CURSORPOS_NULLPTR;
    }
    prDest->x = prSrc->x;
    prDest->y = prSrc->y;
    return CURSORPOS_SUCCESS;
}

int cursorpos_init(CursorPos* prPos) {
    if (prPos == NULL) {
        return CURSORPOS_NULLPTR;
    }
    prPos->x = 0;
    prPos->y = 0;
    return CURSORPOS_SUCCESS;
}

int cursorpos_read(CursorPos* prPos, SceUID fd) {
    int r = 0;
    if (prPos == NULL) {
        return CURSORPOS_NULLPTR;
    }
    if (fd < 0) {
        return CURSORPOS_IOERROR;
    }
    r = sceIoRead(fd, &prPos->x, sizeof(int));
    if (r != sizeof(int)) {
        return CURSORPOS_IOERROR;
    }
    r = sceIoRead(fd, &prPos->y, sizeof(int));
    if (r != sizeof(int)) {
        return CURSORPOS_IOERROR;
    }
    
    return CURSORPOS_SUCCESS;
}

int cursorpos_set(CursorPos* prPos, int x, int y) {
    if (prPos == NULL) {
        return CURSORPOS_NULLPTR;
    }
    prPos->x = x;
    prPos->y = y;
    return CURSORPOS_SUCCESS;
}

int cursorpos_write(CursorPos* prPos, SceUID fd) {
    int r = 0;
    if (prPos == NULL) {
        return CURSORPOS_NULLPTR;
    }
    if (fd < 0) {
        return CURSORPOS_IOERROR;
    }
    r = sceIoWrite(fd, &prPos->x, sizeof(int));
    if (r != sizeof(int)) {
        return CURSORPOS_IOERROR;
    }
    r = sceIoWrite(fd, &prPos->y, sizeof(int));
    if (r != sizeof(int)) {
        return CURSORPOS_IOERROR;
    }
    return CURSORPOS_SUCCESS;
}
