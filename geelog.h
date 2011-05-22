/* 
 * File:   geelog.h
 * Author: Sir Gee of Five
 *
 * Created on May 12, 2011, 3:22 PM
 */

#ifndef GEELOG_H
#define	GEELOG_H

#include <pspkerneltypes.h>
#include <pspsysmem.h>
#include <pspthreadman.h>
#include <pspiofilemgr.h>
#include <string.h>

/** Indicates success. */
#define GEELOG_SUCCESS (0)

/** Indicates failure. */
#define GEELOG_FAILURE (-1)

/** Indicates a NULL pointer. */
#define GEELOG_NULLPTR (-2)

/** Indicates a memory error. */
#define GEELOG_MEMORY (-3)

/** Indicates an I/O error. */
#define GEELOG_IOERROR (-4)

/** Indicates an invalid Logd mbx. */
#define GEELOG_INVMBX (-5)

/** Indicate an invalid log file path. */
#define GEELOG_INVPATH (-6)

/** Maximum length of a Log Line */
#define GEELOG_LINELEN (511)

/** LOG_ERROR Log Level Name */
#define GEELOG_N_ERROR "ERROR"

/** LOG_WARN Log Level Name */
#define GEELOG_N_WARN "WARNING"

/** LOG_INFO Log Level Name */
#define GEELOG_N_INFO "INFO"

/** LOG_DEBUG Log Level Name */
#define GEELOG_N_DEBUG "DEBUG"

/** LOG_NONE Log Level Name */
#define GEELOG_N_NONE "NONE"

/** The maximum length of a filename with path. */
#define GEELOG_PATH_LEN (255)

/** The name of the GeeLog Kernel Semaphore for Mutex. */
#define GEELOG_MUTEX_NAME "GeeLogMutex"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ELogLevel {
        LOG_ERROR, /** Indicates an Error. */
        LOG_WARN, /** Indicates a Warning. */
        LOG_INFO, /** Indicates an Informational Message. */
        LOG_DEBUG, /** Indicates a Debugging Message. */
        LOG_NONE /** Indicates No Logging. */
    } 
    /** The ELogLevel enumeration is used to indicate a Log Level. */
    ELogLevel;

    typedef struct _GeeLog {
        /** The current Log Level of the Logger. */
        ELogLevel rLevel;
        /** The current Log File file descriptor. */
        SceUID rFd;
        /** The current Mutex Id. */
        SceUID rMutex;
        /** The file name including path of the current Log File. */
        char sLogFile[GEELOG_PATH_LEN + 1];
    } 
    /** The GeeLog struct represents a Logger. */
    GeeLog;
    
    int geelog_dlog(ELogLevel rLevel, const char* sMsg);

    /** Append a Log Message from a particular function.
     * 
     * @param rLevel ELogLevel value indicating the Log Level of the Message.
     * @param sFunc string containing the function name to prepend.
     * @param sMsg string containing the Log Message.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int geelog_flog(ELogLevel rLevel, const char* sFunc, const char* sMsg);
    
    /** Initialize a GeeLog Logger, specifying the Log File name.
     * 
     * @param sLogFile string containing the Log File name including path.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int geelog_init(const char* sLogFile);
    
    /** Return a string containing a human readable representation of a Log 
     * Level.
     * 
     * @param rLevel ELogLevel indicating the Log Level.
     * @return A string containing the Log Level is returned.
     */
    const char* geelog_levelname(ELogLevel rLevel);
    
    /** Append a Message to the Logger.
     * 
     * @param rLevel ELogLevel indicating the Log Level of the Message.
     * @param sMsg string containing the Message to append.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int geelog_log(ELogLevel rLevel, const char* sMsg);
    
    /** Start a GeeLog Logger at the indicated Log Level.  The Log File is 
     * opened as a result.
     * 
     * @param rLevel ELogLevel indicating the Log Level of the Logger.  Messages
     * with a Log Level greater than this value will be ignored by the Logger.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int geelog_start(ELogLevel rLevel);
    
    /** Stop a GeeLog Logger.  The Log File is closed as a result.
     * 
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int geelog_stop();
    
#ifdef	__cplusplus
}
#endif

#endif	/* GEELOG_H */
