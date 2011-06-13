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
#include <pspthreadman.h>
#include "resultmodel.h"
#include "searchquery.h"

#define CESEARCH_SUCCESS (0)
#define CESEARCH_FAILURE (-1)
#define CESEARCH_NULLPTR (-2)
#define CESEARCH_MEMORY (-3)
#define CESEARCH_INVIDX (-4)
#define CESEARCH_BADADDR (-5)

#define CESEARCH_EVENT_NAME "CESearchEvent"

#define CESEARCH_E_HIT ((SceUInt32)0x00000001)
#define CESEARCH_E_FINISHED ((SceUInt32)0x00000002)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _ESearchState {
        SES_Error = -1,
        SES_Stopped = 0,
        SES_Running,
        SES_Finished
    }
    ESearchState;

    typedef struct _CESearch {
        SceUID threadId;
        SceUID eventId;
        ESearchState searchState;
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

