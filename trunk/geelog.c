#include "geelog.h"

static int log_append(const char* sLine);
static int log_close();
static int log_open();

static GeeLog krLog;

int geelog_dlog(ELogLevel rLevel, const char* sMsg) {
    SceUID rFd = -1;
    char sLine[GEELOG_LINELEN + 1];
    int llen;
    int bw;
    if (sMsg == NULL) {
        return GEELOG_NULLPTR;
    }
    rFd = sceIoOpen("ms0:/seplugins/GDEBUG.log", 
            PSP_O_CREAT|PSP_O_APPEND|PSP_O_WRONLY, 0644);
    if (rFd < 0) {
        return GEELOG_IOERROR;
    }
    sprintf(sLine, "%s: %s\n", geelog_levelname(rLevel), sMsg);
    llen = strlen(sLine);
    bw = sceIoWrite(rFd, sLine, llen);
    if (bw != llen) {
        return GEELOG_IOERROR;
    }
    sceIoClose(rFd);
    return GEELOG_SUCCESS;
}

int geelog_flog(ELogLevel rLevel, const char* sFunc, const char* sMsg) {
    char sLine[GEELOG_LINELEN + 1];
    
    if ((sFunc == NULL) || (sMsg == NULL)) {
        return GEELOG_NULLPTR;
    }
    sprintf(sLine, "%s: %s", sFunc, sMsg);
    return geelog_log(rLevel, sLine);
}

int geelog_init(const char* sLogFile) {
    krLog.rFd = -1;
    krLog.rLevel = LOG_NONE;
    krLog.rMutex = -1;
    if (sLogFile != NULL) {
        strncpy(krLog.sLogFile, sLogFile, GEELOG_PATH_LEN);
    }
    return GEELOG_SUCCESS;
}

const char* geelog_levelname(ELogLevel rLevel) {
    const char* sName = NULL;
    switch (rLevel) {
        case LOG_ERROR:
            sName = GEELOG_N_ERROR;
            break;
        case LOG_WARN:
            sName = GEELOG_N_WARN;
            break;
        case LOG_INFO:
            sName = GEELOG_N_INFO;
            break;
        case LOG_DEBUG:
            sName = GEELOG_N_DEBUG;
            break;
        case LOG_NONE:
            sName = GEELOG_N_NONE;
            break;
    }
    return sName;
}

int geelog_log(ELogLevel rLevel, const char* sMsg) {
    char sLine[GEELOG_LINELEN + 1];
    if (sMsg == NULL) {
        return GEELOG_NULLPTR;
    }
    /* If the Message log level is higher than the Logger log level, discard. */
    if (rLevel > krLog.rLevel) {
        return GEELOG_SUCCESS;
    }
    sprintf(sLine, "%s: %s\n", geelog_levelname(rLevel), sMsg);
    if (log_append(sLine) != GEELOG_SUCCESS) {
        return GEELOG_FAILURE;
    }
    return GEELOG_SUCCESS;
}

int geelog_start(ELogLevel rLevel) {
    if (krLog.rFd != -1) {
        return GEELOG_FAILURE;
    }
    if (strlen(krLog.sLogFile) < 1) {
        return GEELOG_INVPATH;
    }
    krLog.rLevel = rLevel;
    krLog.rMutex = sceKernelCreateSema(GEELOG_MUTEX_NAME, 0, 1, 1, NULL);
    if (log_open() != GEELOG_SUCCESS) {
        return GEELOG_FAILURE;
    }
    return GEELOG_SUCCESS;
}

int geelog_stop() {
    if (log_close() != GEELOG_SUCCESS) {
        return GEELOG_IOERROR;
    }
    sceKernelDeleteSema(krLog.rMutex);
    return GEELOG_SUCCESS;
}

static int log_append(const char* sLine) {
    int llen = 0;
    int bw = 0;
    SceInt64 res = 0;
    if (krLog.rFd < 0) {
        return GEELOG_FAILURE;
    }
    if (sLine == NULL) {
        return GEELOG_NULLPTR;
    }
    sceKernelWaitSema(krLog.rMutex, 1, NULL);
    llen = strlen(sLine);
    bw = sceIoWriteAsync(krLog.rFd, sLine, llen);
    if (sceIoWaitAsync(krLog.rFd, &res) < 0) {
        sceKernelSignalSema(krLog.rMutex, 1);
        return GEELOG_IOERROR;
    }
    sceKernelSignalSema(krLog.rMutex, 1);
    if (bw != llen) {
        return GEELOG_IOERROR;
    }
    
    return GEELOG_SUCCESS;
}

static int log_close() {
    if (krLog.rFd < 0) {
        return GEELOG_SUCCESS;
    }
    sceKernelWaitSema(krLog.rMutex, 1, NULL);
    if (sceIoClose(krLog.rFd) < 0) {
        sceKernelSignalSema(krLog.rMutex, 1);
        return GEELOG_FAILURE;
    }
    krLog.rFd = -1;
    sceKernelSignalSema(krLog.rMutex, 1);
    return GEELOG_SUCCESS;
}

static int log_open() {
    if (krLog.rFd != -1) {
        return GEELOG_FAILURE;
    }
    krLog.rFd = sceIoOpen(krLog.sLogFile, 
            PSP_O_CREAT|PSP_O_APPEND|PSP_O_WRONLY, 0644);
    if (krLog.rFd < 0) {
        return GEELOG_IOERROR;
    }
    return GEELOG_SUCCESS;
}
