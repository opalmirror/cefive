#include "menumodel.h"

int menumodel_additem(MenuModel* prModel, const int id, const char* sText) {
    MenuItem* prItem = NULL;
    int rowCount = 0;
    if (prModel == NULL) {
        return MENUMODEL_NULLPTR;
    }
    rowCount = prModel->rowCount;
    if (rowCount + 1 >= MENUMODEL_ROW_MAX) {
        return MENUMODEL_MEMORY;
    }
    prModel->rowCount++;
    prItem = menumodel_get(prModel, rowCount);
    if (prItem == NULL) {
        return MENUMODEL_FAILURE;
    }
    prItem->id = id;
    strncpy(prItem->text, sText, MENUMODEL_TEXT_LEN);
    return MENUMODEL_SUCCESS;
}

MenuItem* menumodel_get(MenuModel* prModel, const int index) {
    MenuItem* prItem = NULL;
    if (prModel != NULL) {
        if ((index >= 0) && (index < prModel->rowCount)) {
            prItem = &prModel->items[index];
        }
    }
    return prItem;
}

int menumodel_init(MenuModel* prModel) {
    MenuItem* prItem = NULL;
    int i = 0;
    
    if (prModel == NULL) {
        return MENUMODEL_NULLPTR;
    }
    for (i = 0; i < MENUMODEL_ROW_MAX; i++) {
        prItem = &prModel->items[i];
        prItem->id = -1;
    }
    prModel->rowCount = 0;
    return MENUMODEL_SUCCESS;
}
