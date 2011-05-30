#include "gsegment.h"

int gsegment_copy(Gsegment* pDest, Gsegment* pSrc) {
    if ((pDest == NULL) || (pSrc == NULL)) {
        return GSEGMENT_NULLPTR;
    }
    pDest->segmentType = pSrc->segmentType;
    pDest->startaddr = pSrc->startaddr;
    pDest->endaddr = pSrc->endaddr;
    strncpy(pDest->name, pSrc->name, GSEGMENT_NAME_LEN);
    return GSEGMENT_SUCCESS;
}

int gsegment_init(Gsegment* pSeg) {
    if (pSeg == NULL) {
        return GSEGMENT_NULLPTR;
    }
    pSeg->segmentType = SET_None;
    pSeg->startaddr = 0;
    pSeg->endaddr = 0;
    memset(pSeg->name, 0, GSEGMENT_NAME_LEN + 1);
    return GSEGMENT_SUCCESS;
}

int gsegment_set(Gsegment* pSeg, const SceUInt32 startaddr,
        const SceUInt32 endaddr, const char* name, ESegmentType segType) {
    if (pSeg == NULL) {
        return GSEGMENT_NULLPTR;
    }
    pSeg->segmentType = segType;
    pSeg->startaddr = startaddr;
    pSeg->endaddr = endaddr;
    strncpy(pSeg->name, name, GSEGMENT_NAME_LEN);
    return GSEGMENT_SUCCESS;
}
