#include "geelogmsg.h"
#include "geelog.h"
#include <pspthreadman.h>
#include <string.h>

int geelogmsg_init(GeeLogMsg* prMsg) {
    if (prMsg == NULL) {
        return GEELOGMSG_NULLPTR;
    }
    memset(&prMsg->rHeader, 0, sizeof(SceKernelMsgPacket));
    prMsg->rLevel = LOG_NONE;
    return GEELOGMSG_SUCCESS;
}
