#include <pspkerneltypes.h>
#include "colorconfig.h"

int colorconfig_init(ColorConfig* prConfig) {
    if (prConfig == NULL) {
        return COLORCONFIG_NULLPTR;
    }

    return colorconfig_setcolor(prConfig,
            COLORCONFIG_BACKGROUND, COLORCONFIG_TEXT);
}

int colorconfig_setcolor(ColorConfig* prConfig, u32 background, u32 text) {
    if (prConfig == NULL) {
        return COLORCONFIG_NULLPTR;
    }
    prConfig->background = background;
    prConfig->text = text;
    return COLORCONFIG_SUCCESS;
}
