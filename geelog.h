/* 
 * File:   geelog.h
 * Author: Sir Gee of Five
 *
 * Created on May 11, 2011, 2:28 PM
 */

#ifndef GEELOG_H
#define	GEELOG_H

#include <pspkerneltypes.h>

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
/** Indicates an invalid path or filename. */
#define GEELOG_INVPATH (-5)
/** Maximum length of a Log line. */
#define GEELOG_LINE_LEN (256)
/** Maximum length of a Log File Path. */
#define GEELOG_PATH_LEN (256)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ELogLevel {
        LOG_ERROR, /** Indicates an error. */
        LOG_WARN, /** Indicates a warning. */
        LOG_INFO, /** Indicates an informational message. */
        LOG_DEBUG, /** Indicates a debugging message. */
        LOG_NONE /** Indicates no logging. */
    } 
    /** The ELogLevel enumeration is used to indicate a Logging Level. */
    ELogLevel;

    typedef struct _GeeLog {
        /** Indicates the current Log Level of a Logger. */
        ELogLevel rLevel;
        /** Log File descriptor */
        SceUID rFd;
        /** String containing the path to the Log File. */
        char sLogPath[GEELOG_PATH_LEN + 1];
    } 
    /** The GeeLog struct is used to represent a Logger. */
    GeeLog;

    /** Initialize a GeeLog struct.  The GeeLog struct is used to represent a
     * Logger.
     * 
     * @param prLog Pointer to the GeeLog struct to initialize.
     * @param sFile String containing the path to the Log File.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelog_init(GeeLog* prLog, const char* sFile);
    
    /** Conditionally append the specified message to a GeeLog Logger.
     * 
     * @param prLog Pointer to a GeeLog struct representing the Logger.
     * @param rLevel ELogLevel indicating the Log Level of the Message.
     * @param sMsg String containing the Message to log.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelog_log(GeeLog* prLog, const ELogLevel rLevel, const char* sMsg);
    
    /** Start a GeeLog Logger at the specified Log Level.  The Log File will
     * be created and the Logger will start logging messages below the specified
     * Log Level.
     * 
     * @param prLog Pointer to a GeeLog struct representing the Logger.
     * @param rLevel ELogLevel indicating the starting Log Level.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelog_start(GeeLog* prLog, ELogLevel rLevel);
    
    /** Stop a GeeLog Logger.  The Log File will be closed as a result.
     * 
     * @param prLog Pointer to a GeeLog struct representing the Logger.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelog_stop(GeeLog* prLog);
    
#ifdef	__cplusplus
}
#endif

#endif	/* GEELOG_H */

