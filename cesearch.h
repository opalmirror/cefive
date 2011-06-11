/* 
 * File:   cesearch.h
 * Author: Sir Gee of Five
 *
 * Created on June 9, 2011, 4:20 PM
 */

#ifndef CESEARCH_H
#define	CESEARCH_H

#include <stdio.h>
#include <pspkerneltypes.h>
#include "resultmodel.h"
#include "searchquery.h"

#define CESEARCH_SUCCESS (0)
#define CESEARCH_FAILURE (-1)
#define CESEARCH_NULLPTR (-2)
#define CESEARCH_MEMORY (-3)
#define CESEARCH_INVIDX (-4)
#define CESEARCH_BADADDR (-5)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _CESearch {
        SceUID threadId;
        SearchQuery searchQuery;
        ResultModel resultModel;
        SceUInt32 position;
    }
    CESearch;

    SearchQuery* cesearch_get_query(CESearch* prSearch);
    
    ResultModel* cesearch_get_resultmodel(CESearch* prSearch);
    
    int cesearch_init(CESearch* prSearch);
    
    int cesearch_start(CESearch* prSearch);
    
    int cesearch_stop(CESearch* prSearch);

#ifdef	__cplusplus
}
#endif

#endif	/* CESEARCH_H */

