/* 
 * File:   geelogmsg.h
 * Author: george
 *
 * Created on May 11, 2011, 3:17 PM
 */

#ifndef GEELOGMSG_H
#define	GEELOGMSG_H

#include <pspthreadman.h>

#define GEELOGMSG_MSGLEN (256)
#define GEELOGMSG_THIDLEN (64)

#define GEELOGMSG_SUCCESS (0)
#define GEELOGMSG_FAILURE (-1)
#define GEELOGMSG_NULLPTR (-2)
#define GEELOGMSG_MEMORY (-3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ELogLevel {
        LOG_ERROR, /** Error message. */
        LOG_WARN, /** Warnings message. */
        LOG_INFO, /** Informational message. */
        LOG_DEBUG, /** Debug message. */
        LOG_NONE /** Non reported message. */
    } 
    /** Enumeration indicating a Log Level. */
    ELogLevel;

    typedef struct _GeeLogMsg {
        /** SceKernelMsgPacket containing the Message Header. */
        SceKernelMsgPacket rHeader;
        /** The type of the Message. */
        int iMsgType;
        /** The Log Level of the Message. */
        ELogLevel rLevel;
        /** The text of the Message. */
        char sMessage[GEELOGMSG_MSGLEN + 1];
        /** The name of the Thread issuing the Message. */
        char sThread[GEELOGMSG_THIDLEN + 1];
    }
    /** The GeeLogMsg struct represents a GeeLog Log Message. */
    GeeLogMsg;
    
    int geelogmsg_init(GeeLogMsg* prMsg);

#ifdef	__cplusplus
}
#endif

#endif	/* GEELOGMSG_H */

