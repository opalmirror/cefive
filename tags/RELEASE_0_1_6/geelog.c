#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "geelog.h"

int geelog_init(GeeLog* prLog, ELogLevel rLevel, const char* sFile) {
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    if (prLog->initialized == 1) {
        return GEELOG_SUCCESS;
    }
    prLog->rFd = -1;
    prLog->rLevel = rLevel;
    strncpy(prLog->sLogFile, sFile, GEELOG_PATH_MAX);
    prLog->initialized = 1;
    return GEELOG_SUCCESS;
}

const char* geelog_levelname(ELogLevel rLevel) {
    const char* sName = NULL;
    switch (rLevel) {
        case LOG_NONE:
            sName = "NONE";
            break;
        case LOG_ERROR:
            sName = "ERROR";
            break;
        case LOG_WARN:
            sName = "WARNING";
            break;
        case LOG_INFO:
            sName = "INFO";
            break;
        case LOG_DEBUG:
            sName = "DEBUG";
            break;
    }
    return sName;
}

int geelog_log(GeeLog* prLog, ELogLevel rLevel, const char* sMsg) {
    char lbuf[GEELOG_LINE_MAX];
    SceUID fd = -1;
    const char* sLevel = NULL;
    if (prLog == NULL || sMsg == NULL) {
        return GEELOG_NULLPTR;
    }
    if (rLevel > prLog->rLevel) {
        return GEELOG_SUCCESS;
    }
    fd = sceIoOpen(prLog->sLogFile, 
            PSP_O_WRONLY|PSP_O_CREAT|PSP_O_APPEND, 0644);
    if (fd < 0) {
        return GEELOG_IOERROR;
    }
    sLevel = geelog_levelname(rLevel);
    sprintf(lbuf, "%s: %s\n", sLevel, sMsg);
    sceIoWrite(fd, lbuf, strlen(lbuf));
    sceIoClose(fd);
    return GEELOG_SUCCESS;
}

/*
int geelog_logf(GeeLog* prLog, ELogLevel rLevel, const char* sFmt, ...) {
    char lbuf[GEELOG_LINE_MAX];
    char line[GEELOG_LINE_MAX];
    SceUID fd = -1;
    const char* sLevel = NULL;
    if (prLog == NULL || sFmt == NULL) {
        return GEELOG_NULLPTR;
    }
    if (rLevel > prLog->rLevel) {
        return GEELOG_SUCCESS;
    }
    fd = sceIoOpen(prLog->sLogFile, 
            PSP_O_WRONLY|PSP_O_CREAT|PSP_O_APPEND, 0644);
    if (fd < 0) {
        return GEELOG_IOERROR;
    }
    sLevel = geelog_levelname(rLevel);
    
    va_list args;
    va_start(args, sFmt);
    vsprintf(lbuf, sFmt, args);
    va_end(args);
    sprintf(line, "%s: %s\n", sLevel, lbuf);
    sceIoWrite(fd, line, strlen(line));
    sceIoClose(fd);
    return GEELOG_SUCCESS;
}
*/

int geelog_start(GeeLog* prLog) {
    SceUID fd = -1;
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    /*
    fd = sceIoOpenAsync(prLog->sLogFile,
            PSP_O_WRONLY|PSP_O_CREAT|PSP_O_APPEND, 0777);
    if (fd >= 0) {
        prLog->rFd = fd;
        return GEELOG_SUCCESS;
    }
     

    return GEELOG_IOERROR;
     */
    return GEELOG_SUCCESS;
}

int geelog_stop(GeeLog* prLog) {
    int r = 0;
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    /*
    if (prLog->rFd >= 0) {
        r = sceIoCloseAsync(prLog->rFd);
        if (r < 0) {
            return GEELOG_IOERROR;
        }
        prLog->rFd = -1;
    }
    */
    return GEELOG_SUCCESS;
}
