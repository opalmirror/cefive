/* 
 * File:   blocklist.h
 * Author: Sir Gee of Five
 *
 * Created on October 17, 2010, 4:27 PM
 */

#ifndef BLOCKLIST_H
#define	BLOCKLIST_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "block.h"
    
#define BLOCKLIST_SUCCESS   (0)
#define BLOCKLIST_FAILURE   (-1)
#define BLOCKLIST_MEMORY    (-2)
#define BLOCKLIST_NULLPTR   (-3)
#define BLOCKLIST_INVIDX    (-4)
    
#define BLOCKLIST_MAX_LEN   8192
    
    typedef struct _BlockList {
        Block* aprBlock[BLOCKLIST_MAX_LEN];
        int size;
    }BlockList;

    int blocklist_add(BlockList* prList, Block* prBlock);
    int blocklist_clear(BlockList* prList);
    Block* blocklist_get(BlockList* prList, int index);
    int blocklist_init(BlockList* prList);
    int blocklist_move_down(BlockList* prList, int index);
    int blocklist_move_up(BlockList* prList, int index);
    Block* blocklist_remove(BlockList* prList, int index);
    Block* blocklist_set(BlockList* prList, int index, Block* prBlock);


#ifdef	__cplusplus
}
#endif

#endif	/* BLOCKLIST_H */

