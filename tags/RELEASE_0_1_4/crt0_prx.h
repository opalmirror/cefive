//Crt0_prx
#ifndef CRT0_H
#define CRT0_H

//Includes
#include <pspkernel.h>
#include "module.h"

//Structures
typedef struct _Hook {
    ModuleFunc modfunc;
    char modname[32];
    char libname[32];
    u32 nid;
    void *func;
} Hook;

//Functions

#endif
