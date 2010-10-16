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
    #include "colorconfig.h"
    #include "cursorpos.h"
    #include "dimension.h"

    typedef struct _PanelConfig {
        ColorConfig rColor;
        ColorConfig rCursor;
        ColorConfig rEdit;
        CursorPos rTop;
        Dimension rSize;
    }PanelConfig;

    ColorConfig* panelconfig_get_cursorcolor(PanelConfig* prCfg);
    ColorConfig* panelconfig_get_editcolor(PanelConfig* prCfg);
    ColorConfig* panelconfig_get_panelcolor(PanelConfig* prCfg);
    CursorPos* panelconfig_get_position(PanelConfig* prCfg);
    Dimension* panelconfig_get_size(PanelConfig* prCfg);

#ifdef	__cplusplus
}
#endif

#endif	/* _PANELCONFIG_H */

