/* 
 * File:   searchquery.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 3:27 PM
 */

#ifndef SEARCHQUERY_H
#define	SEARCHQUERY_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include "searchsize.h"
#include "searchmode.h"

#define SEARCHQUERY_SUCCESS (0)
#define SEARCHQUERY_FAILURE (-1)
#define SEARCHQUERY_NULLPTR (-2)
#define SEARCHQUERY_MEMORY (-3)

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
        /** Indicates the starting address of the Search. */
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
    
    int searchquery_init(SearchQuery* prQuery);


#ifdef	__cplusplus
}
#endif

#endif	/* SEARCHQUERY_H */
