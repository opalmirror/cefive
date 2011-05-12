#include "geelog.h"
#include <stdio.h>
#include <string.h>
#include <pspiofilemgr.h>

/** Return a string containing a human readable Log Level name for the specified
 * Log Level.
 * 
 * @param rLevel ELogLevel indicating the Log Level.
 * @return A string containing the Log Level name is returned.
 */
static const char* levelname(ELogLevel rLevel);

/** Append the specified line to a Logger.
 * 
 * @param prLog Pointer to a GeeLog struct representing the Logger.
 * @param sLine String containing the line to append.  Linefeeds should be 
 * present.
 * @return 0 indicates success, less than 0 indicates failure.
 */
static int logappend(GeeLog* prLog, const char *sLine);

/** Close a GeeLog Logger Log File.
 * 
 * @param prLog Pointer to a GeeLog struct representing the Logger.
 * @return 0 indicates success, less than 0 indicates failure.
 */
static int logclose(GeeLog* prLog);

/** Open a GeeLog Logger Log File.
 * 
 * @param prLog Pointer to a GeeLog struct representing the Logger.
 * @return 0 indicates success, less than 0 indicates failure.
 */
static int logopen(GeeLog* prLog);

int geelog_init(GeeLog* prLog, const char* sFile) {
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    prLog->rFd = -1;
    strncpy(prLog->sLogPath, sFile, GEELOG_PATH_LEN);
    return GEELOG_SUCCESS;
}

int geelog_log(GeeLog* prLog, const ELogLevel rLevel, const char* sMsg) {
    char sLine[GEELOG_LINE_LEN + 1];
    if (prLog == NULL || sMsg == NULL) {
        return GEELOG_NULLPTR;
    }
    if (rLevel > prLog->rLevel) {
        return GEELOG_SUCCESS;
    }
    sprintf(sLine, "%s: %s\n", levelname(rLevel), sMsg);
    if (logappend(prLog, sLine) != GEELOG_SUCCESS) {
        return GEELOG_FAILURE;
    }
    return GEELOG_SUCCESS;
}

int geelog_start(GeeLog* prLog, ELogLevel rLevel) {
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    prLog->rLevel = rLevel;
    if (logopen(prLog) != GEELOG_SUCCESS) {
        return GEELOG_FAILURE;
    }
    return GEELOG_SUCCESS;
}

int geelog_stop(GeeLog* prLog) {
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    if (logclose(prLog) != GEELOG_SUCCESS) {
        return GEELOG_FAILURE;
    }
    return GEELOG_SUCCESS;
}

static const char* levelname(ELogLevel rLevel) {
    const char* sName = NULL;
    switch (rLevel) {
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
        case LOG_NONE:
            sName = "NONE";
            break;
    }
    return sName;
}

static int logappend(GeeLog* prLog, const char *sLine) {
    int llen = 0;
    int bw = 0;
    
    if (prLog == NULL || sLine == NULL) {
        return GEELOG_NULLPTR;
    }
    if (prLog->rFd < 0) {
        return GEELOG_FAILURE;
    }
    llen = strnlen(sLine, GEELOG_LINE_LEN);
    bw = sceIoWrite(prLog->rFd, sLine, llen);
    if (bw != llen) {
        return GEELOG_IOERROR;
    }
    return GEELOG_SUCCESS;
}

static int logclose(GeeLog* prLog) {
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    if (prLog->rFd < 0) {
        return GEELOG_SUCCESS;
    }
    if (sceIoClose(prLog->rFd) < 0) {
        return GEELOG_IOERROR;
    }
    return GEELOG_SUCCESS;
}

static int logopen(GeeLog* prLog) {
    if (prLog == NULL) {
        return GEELOG_NULLPTR;
    }
    if (prLog->rFd >= 0) {
        return GEELOG_FAILURE;
    }
    if (prLog->sLogPath == NULL) {
        return GEELOG_INVPATH;
    }
    prLog->rFd = sceIoOpen(prLog->sLogPath, 
            PSP_O_WRONLY|PSP_O_APPEND|PSP_O_CREAT, 0644);
    if (prLog->rFd < 0) {
        return GEELOG_IOERROR;
    }
    return GEELOG_SUCCESS;
}
