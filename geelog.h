/* 
 * File:   geelog.h
 * Author: Sir Gee of Five
 *
 * Created on October 10, 2010, 2:20 PM
 */

#ifndef _GEELOG_H
#define	_GEELOG_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <pspkerneltypes.h>

    #define GEELOG_SUCCESS      (0)
    #define GEELOG_FAILURE      (-1)
    #define GEELOG_MEMORY       (-2)
    #define GEELOG_NULLPTR      (-3)
    #define GEELOG_IOERROR      (-4)

    #define GEELOG_PATH_MAX     256
    #define GEELOG_LINE_MAX     1024

    typedef enum _ELogLevel {
        LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG
    }ELogLevel;

    typedef struct _GeeLog {
        char sLogFile[GEELOG_PATH_MAX];
        ELogLevel rLevel;
        SceUID rFd;
    }GeeLog;

    int geelog_init(GeeLog* prLog, ELogLevel rLevel, const char* sFile);
    int geelog_log(GeeLog* prLog, ELogLevel rLevel, const char* sMsg);
    int geelog_start(GeeLog* prLog);
    int geelog_stop(GeeLog* prLog);

#ifdef	__cplusplus
}
#endif

#endif	/* _GEELOG_H */

