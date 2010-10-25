#ifndef _CHEAT_H
#define _CHEAT_H

//Cheat flags
//If selected, will be disabled/enabled by music button
#define CHEAT_FLAG_SELECTED (1<<0)
//If 1, cheat is constantly on regardless of music button
#define CHEAT_FLAG_CONSTANT	(1<<1)
//Cheat was just recently enabled/disabled
#define CHEAT_FLAG_FRESH    (1<<2)

#define CHEAT_SUCCESS       (0)
#define CHEAT_FAILURE       (-1)
#define CHEAT_MEMORY        (-2)

typedef struct _Cheat{
    unsigned short block;
    unsigned short len;
    unsigned char flags;
    unsigned char name[32];
}Cheat;

int cheat_clear_flags(Cheat* prCheat);
int cheat_clearflag_constant(Cheat* prCheat);
int cheat_clearflag_fresh(Cheat* prCheat);
int cheat_clearflag_selected(Cheat* prCheat);
unsigned int cheat_get_block(Cheat* prCheat);
unsigned int cheat_get_length(Cheat *prCheat);
int cheat_is_constant(Cheat* prCheat);
int cheat_is_fresh(Cheat* prCheat);
int cheat_is_inactive(Cheat* prCheat);
int cheat_is_selected(Cheat* prCheat);
int cheat_setflag_constant(Cheat* prCheat);
int cheat_setflag_fresh(Cheat* prCheat);
int cheat_setflag_selected(Cheat* prCheat);
int cheat_set_block(Cheat* prCheat, unsigned int block);
int cheat_set_constant(Cheat* prCheat);
int cheat_set_inactive(Cheat* prCheat);
int cheat_set_length(Cheat* prCheat, unsigned int length);
int cheat_set_selected(Cheat* prCheat);

#endif
// vi:cin:et:ts=4 sw=4
