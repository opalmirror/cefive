/* 
 * File:   geelogqueue.h
 * Author: george
 *
 * Created on May 11, 2011, 3:14 PM
 */

#ifndef GEELOGQUEUE_H
#define	GEELOGQUEUE_H

#include <psptypes.h>
#include "geelogmsg.h"

/** The Length of the Queue */
#define GEELOGQUEUE_QLEN (16)
/** The name of the GeeLogQueue Mutex */
#define GEELOGQUEUE_MUTEX "GeeLogQueueMutex"

/** Indicate success. */
#define GEELOGQUEUE_SUCCESS (0)
/** Indicates failure. */
#define GEELOGQUEUE_FAILURE (-1)
/** Indicates a NULL pointer. */
#define GEELOGQUEUE_NULLPTR (-2)
/** Indicates a memory error. */
#define GEELOGQUEUE_MEMORY (-3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GeeLogQueue {
        /** Array of GeeLogMsg struct representing the Queue. */
        GeeLogMsg arMsg[GEELOGQUEUE_QLEN];
        /** The first active element in the Queue. */
        int iHead;
        /** The last active element in the Queue. */
        int iTail;
        /** The SceUID of the Sema for the Queue. */
        SceUID rLock;
    } 
    /** The GeeLogQueue struct represents a rotating queue of GeeLogMsg structs.
     */
    GeeLogQueue;

    /** Indicate whether a GeeLog Queue current has any messages available.
     * 
     * @param prQueue Pointer to a GeeLogQueue struct representing the Queue.
     * @return 1 is returned if the Queue has a message available, 0 is returned
     * otherwise.
     */
    int geelogqueue_hasmsg(GeeLogQueue* prQueue);
    
    /** Initialize a GeeLogQueue struct.
     * 
     * @param prQueue Pointer to the GeeLogQueue struct to initialize.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelogqueue_init(GeeLogQueue* prQueue);
    
    /** Create a new Message in the Queue.
     * 
     * @param prMsg Pointer to a GeeLogMsg struct to hold the new Message.
     * @param prQueue Pointer to a GeeLogQueue struct representing the Queue.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelogqueue_newmsg(GeeLogMsg** prMsg, GeeLogQueue* prQueue);
    
    /** Get a pointer to the next Message in the Queue.
     * 
     * @param prMsg Pointer to a GeeLogMsg struct to hold the next Message.
     * @param prQueue Pointer to a GeeLogQueue struct representing the Queue.
     * @return 0 indicates success, less than 0 indicates failure.
     */
    int geelogqueue_next(GeeLogMsg** prMsg, GeeLogQueue* prQueue);

#ifdef	__cplusplus
}
#endif

#endif	/* GEELOGQUEUE_H */
