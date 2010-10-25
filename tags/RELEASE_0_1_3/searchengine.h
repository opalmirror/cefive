/* 
 * File:   searchengine.h
 *  The Search Engine for Cheat Engine of Five
 * Author: Sir Gee of Five
 *
 * Created on September 29, 2010, 10:13 AM
 */

#ifndef _SEARCHENGINE_H
#define	_SEARCHENGINE_H

#include <pspkerneltypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

    #define SEARCHENGINE_SUCCESS        (0)
    #define SEARCHENGINE_FAILURE        (-1)
    #define SEARCHENGINE_MEMORY         (-2)
    #define SEARCHENGINE_NOT_FOUND      (-3)
    #define SEARCHENGINE_BAD_ADDR       (-4)

    #define SEARCHENGINE_TEXTLEN        255
    #define SEARCHENGINE_MAXRESULTS     100
    #define SEARCHENGINE_DEFIPC         0x10000

    typedef enum _ESearchMode {
        ESM_Immediate, ESM_Text, ESM_Byte
    }ESearchMode;

    typedef enum _ESearchSize {
        ESZ_Byte, ESZ_Word, ESZ_Dword
    }ESearchSize;

    typedef enum _ESearchState {
        ESS_Fault, ESS_Stopped, ESS_Idle, ESS_Searching, ESS_Finished
    }ESearchState;

    typedef struct _SearchQuery {
        ESearchMode     searchMode;
        ESearchSize     searchSize;
        unsigned int    value;
        char            text[SEARCHENGINE_TEXTLEN + 1];
        SceUInt32       startAddr;
        SceUInt32       endAddr;
    }SearchQuery;

    typedef struct _SearchResult {
        unsigned int    address;
        unsigned int    value;
        char            text[SEARCHENGINE_TEXTLEN + 1];
    }SearchResult;

    typedef struct _SearchEngineConfig {
        SceUInt32       min_position;
        SceUInt32       max_position;
        int             iterations_per_cycle;
    }SearchEngineConfig;

    typedef struct _SearchEngine {
        SearchEngineConfig  rConfig;
        ESearchState        rState;
        SceUInt32           position;
        SearchQuery         rQuery;
        SearchResult        arResult[SEARCHENGINE_MAXRESULTS];
        int                 result_count;
    }SearchEngine;

    int searchengine_cancel(SearchEngine* prEngine);
    int searchengine_cycle_mode(SearchEngine* prEngine);
    int searchengine_cycle_size(SearchEngine* prEngine);
    SearchResult* searchengine_get_result(SearchEngine* prEngine, int index);
    int searchengine_get_result_count(SearchEngine* prEngine);
    int searchengine_reset(SearchEngine* prEngine);
    int searchengine_run(SearchEngine* prEngine);
    int searchengine_search(SearchEngine* prEngine);
    int searchengine_seek(SearchEngine* prEngine, SceUInt32 position);
    int searchengine_start(SearchEngine* prEngine);
    int searchengine_stop(SearchEngine* prEngine);
    SceUInt32 searchengine_tell(SearchEngine *prEngine);

#ifdef	__cplusplus
}
#endif

#endif	/* _SEARCHENGINE_H */

