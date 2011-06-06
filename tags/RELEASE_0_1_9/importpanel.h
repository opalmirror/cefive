/* 
 * File:   importpanel.h
 * Author: Sir Gee of Five
 *
 * Created on April 24, 2011, 6:26 PM
 */

#ifndef IMPORTPANEL_H
#define	IMPORTPANEL_H

#include "panelconfig.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _ImportPanelConfig {
        PanelConfig rPanelConfig;
    }ImportPanelConfig;

    typedef struct _ImportPanel {
        ImportPanelConfig rConfig;
    }ImportPanel;


#ifdef	__cplusplus
}
#endif

#endif	/* IMPORTPANEL_H */

