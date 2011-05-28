/* 
 * File:   gcheat.h
 * Author: Sir Gee of Five
 *
 * Created on October 17, 2010, 5:15 PM
 */

#ifndef GCHEAT_H
#define	GCHEAT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "blocklist.h"
#include "block.h"
    
#define GCHEAT_SUCCESS      (0)
#define GCHEAT_FAILURE      (-1)
#define GCHEAT_MEMORY       (-2)
#define GCHEAT_NULLPTR      (-3)
#define GCHEAT_INVIDX       (-4)
    
#define GCHEAT_NAME_LEN     (96)

    typedef enum _ETriggerMode {
        TM_None, TM_Triggered, TM_Constant, TM_Once, TM_Cycle
    }ETriggerMode;
    
    typedef enum _ETriggerStatus {
        TS_None, TS_Inactive, TS_Active, TS_Triggered
    }ETriggerStatus;
    
    typedef struct _GCheat {
        char sName[GCHEAT_NAME_LEN + 1];
        BlockList rBlockList;
        ETriggerMode rTriggerMode;
        ETriggerStatus rTriggerStatus;
    }GCheat;

    int gcheat_add_block(GCheat* prCheat, Block* prBlock);
    int gcheat_clear_blocklist(GCheat* prCheat);
    Block* gcheat_get_block(GCheat* prCheat, int index);
    int gcheat_get_blockcount(GCheat* prCheat);
    BlockList* gcheat_get_blocklist(GCheat* prCheat);
    char* gcheat_get_name(GCheat* prCheat);
    int gcheat_init(GCheat* prCheat);
    int gcheat_move_block_down(GCheat* prCheat, int index);
    int gcheat_move_block_up(GCheat* prCheat, int index);
    Block* gcheat_remove_block(GCheat* prCheat, int index);
    Block* gcheat_set_block(GCheat* prCheat, int index, Block* prBlock);

#ifdef	__cplusplus
}
#endif

#endif	/* GCHEAT_H */

