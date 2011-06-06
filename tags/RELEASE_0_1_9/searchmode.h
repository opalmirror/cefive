/* 
 * File:   searchmode.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 3:30 PM
 */

#ifndef SEARCHMODE_H
#define	SEARCHMODE_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ESearchMode {
        SMD_None = 0, /** No mode specified.*/
        SMD_Immediate, /** Immediate Values. */
        SMD_Text, /** Text and Character data. */
        SMD_Byte /** Physical Memory byte-by-byte. */
    }
    /** The ESearchMode enumeration is used to indicate a mode for searching
     * memory.
     */
    ESearchMode;


#ifdef	__cplusplus
}
#endif

#endif	/* SEARCHMODE_H */

