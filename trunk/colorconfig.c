#include <stdlib.h>
#include <stdio.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include "colorconfig.h"

int colorconfig_copy(ColorConfig* prDest, ColorConfig* prSrc) {
    if (prDest == NULL || prSrc == NULL) {
        return COLORCONFIG_NULLPTR;
    }
    prDest->background = prSrc->background;
    prDest->text = prSrc->text;
    return COLORCONFIG_SUCCESS;
}

int colorconfig_init(ColorConfig* prConfig) {
    if (prConfig == NULL) {
        return COLORCONFIG_NULLPTR;
    }

    return colorconfig_setcolor(prConfig,
            COLORCONFIG_BACKGROUND, COLORCONFIG_TEXT);
}

int colorconfig_read(ColorConfig* prConfig, SceUID fd) {
    if (prConfig == NULL) {
        return COLORCONFIG_NULLPTR;
    }
    sceIoRead(fd, &prConfig->background, sizeof(u32));
    sceIoRead(fd, &prConfig->text, sizeof(u32));
    return COLORCONFIG_SUCCESS;
}

int colorconfig_setcolor(ColorConfig* prConfig, u32 background, u32 text) {
    if (prConfig == NULL) {
        return COLORCONFIG_NULLPTR;
    }
    prConfig->background = background;
    prConfig->text = text;
    return COLORCONFIG_SUCCESS;
}

int colorconfig_write(ColorConfig* prConfig, SceUID fd) {
    if (prConfig == NULL) {
        return COLORCONFIG_NULLPTR;
    }
    sceIoWrite(fd, &prConfig->background, sizeof(u32));
    sceIoWrite(fd, &prConfig->text, sizeof(u32));
    return COLORCONFIG_SUCCESS;
}