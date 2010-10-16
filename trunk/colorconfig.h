#ifndef _COLORCONFIG_H
#define _COLORCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <pspkerneltypes.h>

#define COLORCONFIG_SUCCESS     (0)
#define COLORCONFIG_FAILURE     (-1)
#define COLORCONFIG_MEMORY      (-2)
#define COLORCONFIG_NULLPTR     (-3)

#define COLORCONFIG_BACKGROUND  ((u32)0x00000000)
#define COLORCONFIG_TEXT        ((u32)0xFFFFFFFF)

typedef struct _ColorConfig {
    u32 background;
    u32 text;
} ColorConfig;

int colorconfig_init(ColorConfig* prConfig);
int colorconfig_setcolor(ColorConfig* prConfig, u32 background, u32 text);

#ifdef	__cplusplus
}
#endif

#endif /* _COLORCONFIG_H */
