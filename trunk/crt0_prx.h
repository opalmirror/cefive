//Crt0_prx
#ifndef CRT0_H
#define CRT0_H

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspchnnlsv.h>
#include <pspctrl.h>
#include <pspctrl_kernel.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <pspkerneltypes.h>
#include <pspmoduleinfo.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include <pspwlan.h>
#include "block.h"
#include "cefive.h"
#include "cefiveconfig.h"
#include "cefiveui.h"
#include "cheat.h"
#include "cheatengine.h"
#include "float.h"
#include "geelog.h"
#include "module.h"
#include "niteprio.h"
#include "savedata.h"
#include "searchengine.h"

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
