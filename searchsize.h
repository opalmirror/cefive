/* 
 * File:   searchsize.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 3:34 PM
 */

#ifndef SEARCHSIZE_H
#define	SEARCHSIZE_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ESearchSize {
        SSZ_None = 0, /** No size specified. */
        SSZ_Byte, /** 8-bits. */
        SSZ_Half, /** 16-bits. */
        SSZ_Word /** 32-bits. */
    }
    /** The ESearchSize enumeration is used to indicate the size of a search. */
    ESearchSize;


#ifdef	__cplusplus
}
#endif

#endif	/* SEARCHSIZE_H */

