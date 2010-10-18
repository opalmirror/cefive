#include <stdlib.h>
#include "cefiveconfig.h"
#include "cefive.h"

int cefive_init(CEFive* prCe) {
    CEFiveConfig* prConfig = NULL;
    
    if (prCe == NULL) {
        return CEFIVE_NULLPTR;
    }
    prConfig = &prCe->rConfig;
    cefiveconfig_init(prConfig);
    
    return CEFIVE_SUCCESS;
}
