#include "glabel.h"

int glabel_copy(Glabel* prDest, Glabel* prSrc) {
    if ((prDest == NULL) || (prSrc == NULL)) {
        return GLABEL_NULLPTR;
    }
    prDest->address = prSrc->address;
    strncpy(prDest->text, prSrc->text, GLABEL_TEXT_LEN);
    return GLABEL_SUCCESS;
}

int glabel_init(Glabel* prLabel) {
    if (prLabel == NULL) {
        return GLABEL_NULLPTR;
    }
    prLabel->address = 0;
    memset(prLabel->text, 0, GLABEL_TEXT_LEN + 1);
    return GLABEL_SUCCESS;
}

int glabel_set(Glabel* prLabel, const SceUInt32 address, const char* text) {
    if (prLabel == NULL) {
        return GLABEL_NULLPTR;
    }
    prLabel->address = address;
    strncpy(prLabel->text, text, GLABEL_TEXT_LEN);
    return GLABEL_SUCCESS;
}
