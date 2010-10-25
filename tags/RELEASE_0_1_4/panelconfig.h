/* 
 * File:   panelconfig.h
 * Author: Sir Gee of Five
 *
 * Created on October 3, 2010, 1:31 PM
 */

#ifndef _PANELCONFIG_H
#define	_PANELCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <pspkerneltypes.h>
#include "colorconfig.h"
#include "cursorpos.h"
#include "dimension.h"

#define PANELCONFIG_SUCCESS     (0)
#define PANELCONFIG_FAILURE     (-1)
#define PANELCONFIG_MEMORY      (-2)
#define PANELCONFIG_NULLPTR     (-3)
#define PANELCONFIG_IOERROR     (-4)
    
    typedef struct _PanelConfig {
        ColorConfig rColor;
        ColorConfig rCursor;
        ColorConfig rEdit;
        CursorPos rTop;
        Dimension rSize;
    }PanelConfig;

    int panelconfig_copy(PanelConfig* prDest, PanelConfig* prSrc);
    ColorConfig* panelconfig_get_cursorcolor(PanelConfig* prCfg);
    ColorConfig* panelconfig_get_editcolor(PanelConfig* prCfg);
    ColorConfig* panelconfig_get_panelcolor(PanelConfig* prCfg);
    CursorPos* panelconfig_get_position(PanelConfig* prCfg);
    Dimension* panelconfig_get_size(PanelConfig* prCfg);
    int panelconfig_init(PanelConfig* prCfg);
    int panelconfig_read(PanelConfig* prCfg, SceUID fd);
    int panelconfig_write(PanelConfig* prCfg, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif	/* _PANELCONFIG_H */

