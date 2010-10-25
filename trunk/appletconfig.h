/* 
 * File:   appletconfig.h
 * Author: Sir Gee of Five
 *
 * Created on October 3, 2010, 1:35 PM
 */

#ifndef _APPLETCONFIG_H
#define	_APPLETCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "panelconfig.h"

#define APPLETCONFIG_SUCCESS        (0)
#define APPLETCONFIG_FAILURE        (-1)
#define APPLETCONFIG_MEMORY         (-2)
#define APPLETCONFIG_NULLPTR        (-3)
#define APPLETCONFIG_IOERROR        (-4)

    typedef struct _AppletConfig {
        PanelConfig rPanel;
        ColorConfig rStatus;
        ColorConfig rTitlebar;
    }AppletConfig;

    int appletconfig_copy(AppletConfig* prDest, AppletConfig* prSrc);
    ColorConfig* appletconfig_get_cursorcolor(AppletConfig* prConfig);
    ColorConfig* appletconfig_get_editcolor(AppletConfig* prConfig);
    ColorConfig* appletconfig_get_panelcolor(AppletConfig* prConfig);
    PanelConfig* appletconfig_get_panelconfig(AppletConfig* prConfig);
    ColorConfig* appletconfig_get_statuscolor(AppletConfig* prConfig);
    ColorConfig* appletconfig_get_titlecolor(AppletConfig* prConfig);
    int appletconfig_init(AppletConfig* prConfig);
    int appletconfig_read(AppletConfig* prConfig, SceUID fd);
    int appletconfig_write(AppletConfig* prConfig, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif	/* _APPLETCONFIG_H */

