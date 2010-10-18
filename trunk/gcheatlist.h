/* 
 * File:   gcheatlist.h
 * Author: Sir Gee of Five
 *
 * Created on October 17, 2010, 8:46 PM
 */

#ifndef GCHEATLIST_H
#define	GCHEATLIST_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "gcheat.h"
    
#define GCHEATLIST_SUCCESS      (0)
#define GCHEATLIST_FAILURE      (-1)
#define GCHEATLIST_MEMORY       (-2)
#define GCHEATLIST_NULLPTR      (-3)
#define GCHEATLIST_INVIDX       (-4)

#define GCHEATLIST_MAX_SIZE     1024
    
    typedef struct _GCheatList {
        GCheat* aprCheat[GCHEATLIST_MAX_SIZE];
        int size;
    }GCheatList;

    int gcheatlist_add(GCheatList* prList, GCheat* prCheat);
    int gcheatlist_clear(GCheatList* prList);
    GCheat* gcheatlist_get(GCheatList* prList, int index);
    int gcheatlist_init(GCheatList* prList);
    int gcheatlist_move_up(GCheatList* prList, int index);
    int gcheatlist_move_down(GCheatList* prList, int index);
    GCheat* gcheatlist_remove(GCheatList* prList, int index);
    GCheat* gcheatlist_set(GCheatList* prList, int index, GCheat* prCheat);
    
#ifdef	__cplusplus
}
#endif

#endif	/* GCHEATLIST_H */

