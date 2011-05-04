/* 
 * File:   geelog.h
 * Author: Sir Gee of Five
 *
 * Created on October 10, 2010, 2:20 PM
 */

#ifndef _GEELOG_H
#define	_GEELOG_H

#include <psptypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

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
        int initialized;
    }GeeLog;

    /* geelog_init
     *   Initializes the GeeLog logging facility.  Should be called before
     *   any logging functions are called.
     * Parameters:
     *   prLog   Pointer to a GeeLog struct to act as the handle.
     *   rLevel  The default log level (One of ELogLevel enum).  Only messages
     *           with a log level below or at this level will be logged.
     *   sFile   The filename to use for the log file.
     *
     */
    int geelog_init(GeeLog* prLog, ELogLevel rLevel, const char* sFile);
    
    /* geelog_log
     *  Add a log statement to the current log file.
     * Parameters:
     *  prLog   Pointer to a GeeLog struct initialized with geelog_init.
     *  rLevel  The log level (one of ELogLevel enum) to act as the log level
     *          of the message.  If rLevel is above the current log level, the
     *          message will not be logged.
     *  sMsg    A string containing the message to write to the log file.
     */
    int geelog_log(GeeLog* prLog, ELogLevel rLevel, const char* sMsg);
    
    /* CURRENTLY DISABLED */
    /*int geelog_logf(GeeLog* prLog, ELogLevel rLevel, const char* sFmt, ...);*/
    
    int geelog_start(GeeLog* prLog);
    int geelog_stop(GeeLog* prLog);

#ifdef	__cplusplus
}
#endif

#endif	/* _GEELOG_H */

