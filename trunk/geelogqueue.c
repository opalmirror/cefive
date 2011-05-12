#include "geelogqueue.h"
#include "geelogmsg.h"
#include <pspthreadman.h>
#include "geelog.h"

int geelogqueue_hasmsg(GeeLogQueue* prQueue) {
    if (prQueue != NULL) {
        if (prQueue->iHead != prQueue->iTail) {
            return 1;
        }
    }
    char sLine[256];
    sprintf(sLine, "geelogqueue_hasmsg: head = %d, tail = %d.", prQueue->iHead, prQueue->iTail);
    geelog_dlog(LOG_DEBUG, sLine);
    return 0;
}

int geelogqueue_init(GeeLogQueue* prQueue) {
    GeeLogMsg* prMsg = NULL;
    int i = 0;
    if (prQueue == NULL) {
        return GEELOGQUEUE_NULLPTR;
    }
    for (i = 0; i < GEELOGQUEUE_QLEN; i++) {
        prMsg = &prQueue->arMsg[i];
        if (geelogmsg_init(prMsg) != GEELOGMSG_SUCCESS) {
            return GEELOGQUEUE_FAILURE;
        }
    }
    prQueue->iHead = 0;
    prQueue->iTail = 0;
    prQueue->rLock = sceKernelCreateSema(GEELOGQUEUE_MUTEX, 0, 1, 1, NULL);
    return GEELOGQUEUE_SUCCESS;
}

int geelogqueue_newmsg(GeeLogMsg** prMsg, GeeLogQueue* prQueue) {
    char sLine[256];
    int tail = 0;
    if (prQueue == NULL) {
        return GEELOGQUEUE_NULLPTR;
    }
    sceKernelWaitSema(prQueue->rLock, 1, NULL);
    tail = prQueue->iTail;
    tail++;
    if (tail >= GEELOGQUEUE_QLEN) {
        if (prQueue->iHead == 0) {
            sceKernelSignalSema(prQueue->rLock, 1);
            return GEELOGQUEUE_MEMORY;
        }
        tail = 0;
    }
    prQueue->iTail = tail;
    *prMsg = &prQueue->arMsg[prQueue->iTail];
    if (geelogmsg_init(*prMsg) != GEELOGMSG_SUCCESS) {
        sceKernelSignalSema(prQueue->rLock, 1);
        return GEELOGQUEUE_FAILURE;
    }
    sceKernelSignalSema(prQueue->rLock, 1);
    return GEELOGQUEUE_SUCCESS;
}

int geelogqueue_next(GeeLogMsg** prMsg, GeeLogQueue* prQueue) {
    char sLine[256];
    int head = 0;
    if (prQueue == NULL) {
        return GEELOGQUEUE_NULLPTR;
    }
    geelog_dlog(LOG_DEBUG, "geelogqueue_next: Waiting on Mutex.");
    sceKernelWaitSema(prQueue->rLock, 1, NULL);
    head = prQueue->iHead;
    sprintf(sLine, "geelogqueue_next: Head = %d.", head);
    geelog_dlog(LOG_DEBUG, sLine);
    if (head == prQueue->iTail) {
        geelog_dlog(LOG_ERROR, "geelogqueue_next: No current message.");
        *prMsg = NULL;
        sceKernelSignalSema(prQueue->rLock, 1);
        return GEELOGQUEUE_FAILURE;
    }
    head++;
    if (head >= GEELOGQUEUE_QLEN) {
        head = 0;
    }
    *prMsg = &prQueue->arMsg[prQueue->iHead];
    prQueue->iHead = head;
    sceKernelSignalSema(prQueue->rLock, 1);
    geelog_dlog(LOG_DEBUG, "geelogqueue_next: Got Next Message.");
    return GEELOGQUEUE_SUCCESS;
}
