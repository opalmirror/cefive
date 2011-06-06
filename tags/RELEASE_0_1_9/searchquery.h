/* 
 * File:   searchquery.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 3:27 PM
 */

#ifndef SEARCHQUERY_H
#define	SEARCHQUERY_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include "searchsize.h"
#include "searchmode.h"

#define SEARCHQUERY_TEXT_LEN (127)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _SearchQuery {
        /** Indicates the mode of the Search. */
        ESearchMode searchMode;
        /** Indicates the size of the Search. */
        ESearchSize searchSize;
        /** Indicates the value to Search for. */
        SceUInt32 value;
        /** Indicates the startign address of the Search. */
        SceUInt32 startAddress;
        /** Indicates the ending address of the Search. */
        SceUInt32 endAddress;
        /** Indicates a string to search for. */
        char text[SEARCHQUERY_TEXT_LEN + 1];
    }
    /** The SearchQuery struct represents a Search Query.
     * A Search Query is used to search through active memory for in indicated
     * value.
     */
    SearchQuery;


#ifdef	__cplusplus
}
#endif

#endif	/* SEARCHQUERY_H */
