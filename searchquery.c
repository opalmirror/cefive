#include "searchquery.h"

int searchquery_init(SearchQuery* prQuery) {
    if (prQuery == NULL) {
        return SEARCHQUERY_NULLPTR;
    }
    prQuery->searchMode = SMD_None;
    prQuery->searchSize = SSZ_None;
    prQuery->startAddress = 0;
    prQuery->endAddress = 0;
    prQuery->value = 0;
    memset(prQuery->text, 0, SEARCHQUERY_TEXT_LEN + 1);
    return SEARCHQUERY_SUCCESS;
}
