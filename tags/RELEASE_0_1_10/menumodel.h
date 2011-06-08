/* 
 * File:   menumodel.h
 * Author: Sir Gee of Five
 *
 * Created on May 21, 2011, 11:53 AM
 */

#ifndef MENUMODEL_H
#define	MENUMODEL_H

#include <stdio.h>
#include <string.h>

/** Indicates success. */
#define MENUMODEL_SUCCESS (0)

/** Indicates failure. */
#define MENUMODEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define MENUMODEL_NULLPTR (-2)

/** Indicates a memory error. */
#define MENUMODEL_MEMORY (-3)

/** Indicates an invalid index. */
#define MENUMODEL_INVIDX (-4)

/** The maximum length of a text column. */
#define MENUMODEL_TEXT_LEN (69)

/** The maximum number of rows in the model. */
#define MENUMODEL_ROW_MAX (25)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _MenuItem {
        /** Id of the Menu Item. */
        int id;
        /** Text representation of the Menu Item. */
        char text[MENUMODEL_TEXT_LEN + 1];
    }
    /** The MenuItem struct represents a Menu Item. */
    MenuItem;

    typedef struct _MenuModel {
        /** Number of rows in the model. */
        int rowCount;
        /** Array of MenuItem structs representing the rows. */
        MenuItem items[MENUMODEL_ROW_MAX];
    }
    /** The MenuModel struct represents a Menu Model. */
    MenuModel;

    /** Add a Menu Item to a Menu Model.
     * 
     * @param prModel Pointer to a MenuModel struct representing the Menu Model.
     * @param id int containing the id of the item to add.
     * @param sText string containing the text representation of the Menu Item.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int menumodel_additem(MenuModel* prModel, const int id, const char* sText);
    
    /** Return a pointer to a MenuItem struct representing the indicated Menu
     * Item.
     * 
     * @param prModel Pointer to a MenuModel struct representing the Menu Model.
     * @param index int indicating the Menu Item to return.
     * @return A pointer to a MenuItem struct or NULL is returned.
     */
    MenuItem* menumodel_get(MenuModel* prModel, const int index);
    
    /** Initialize a Menu Model.
     * 
     * @param prModel Pointer to the MenuModel struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int menumodel_init(MenuModel* prModel);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MENUMODEL_H */

