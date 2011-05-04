/* 
 * File:   dasmconfig.h
 * Author: Sir Gee of Five
 *
 * Created on May 1, 2011, 4:10 PM
 */

#ifndef DASMCONFIG_H
#define	DASMCONFIG_H

#include <psptypes.h>
#include "appletconfig.h"
#include "colorconfig.h"

/** Indicates success. */
#define DASMCONFIG_SUCCESS (0)

/** Indicates failure. */
#define DASMCONFIG_FAILURE (-1)

/** Indicates a NULL pointer. */
#define DASMCONFIG_NULLPTR (-2)

/** Indicates a memory error. */
#define DASMCONFIG_MEMORY (-3)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _DasmConfig {
        /** Pointer to the Applet Configuration */
        AppletConfig* prApCfg;
        /** The Base Address of the Disassembler. */
        SceUInt32 baseAddress;
        /** Color Configuration for displaying pointer values. */
        ColorConfig rPtrColor;
    }
    /** The DasmConfig struct represenents the configuration settings
     * for the Disassembler.
     */
    DasmConfig;

    /** Return a pointer to the AppletConfig struct representing the Applet
     * Configuration to use for the Dissasembler.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to an AppletConfig struct or NULL is returned.
     */
    AppletConfig* dasmconfig_get_appletconfig(DasmConfig* prCfg);
    
    /** Return a 32-bit unsigned integer address to use as a base address
     * for the Disassembler.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return SceUInt32 containing the base address of the disassembler.
     */
    SceUInt32 dasmconfig_get_baseaddr(DasmConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct that represents the Cursor
     * Color Configuration in a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmconfig_get_cursorcolor(DasmConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct that represents the Editor
     * Color Configuration in a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmconfig_get_editcolor(DasmConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct that represents the Panel
     * Color Configuration in a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmconfig_get_panelcolor(DasmConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct that represents the Pointer
     * Color Configuration in a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmconfig_get_pointercolor(DasmConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct that represents the Status Bar
     * Color Configuration in a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmconfig_get_statuscolor(DasmConfig* prCfg);
    
    /** Return a pointer to a ColorConfig struct that represents the Title Bar
     * Color Configuration in a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @return A pointer to a ColorConfig struct or NULL is returned.
     */
    ColorConfig* dasmconfig_get_titlecolor(DasmConfig* prCfg);
    
    /** Initialize a DasmConfig struct to use to represent a Disassembler
     * configuration.
     * 
     * @param prCfg Pointer to the DasmConfig struct to initialize.
     * @return 0 is returned for success, less than 0 is returned for failure.
     */
    int dasmconfig_init(DasmConfig* prCfg);
    
    /** Assign an Applet Configuration to a Disassembler Configuration.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @param prApp Pointer to an AppletConfig struct representing the
     * Applet Configuration.
     * @return 0 is returned for success, less than 0 is returned for failure.
     */
    int dasmconfig_set_appletconfig(DasmConfig* prCfg, AppletConfig* prApp);
    
    /** Assign a 32-bit base address to a Disassembler Configuration.
     * The base address is used to calculate the address shown to the user.
     * The base address should be equivalent to the first visible byte in
     * the disassembler.
     * 
     * @param prCfg Pointer to a DasmConfig struct representing the 
     * configuration of the Disassembler.
     * @param addr SceUInt32 containing the base address to assign.
     * @return 0 is returned for success, less than 0 is returned for failure.
     */
    int dasmconfig_set_baseaddr(DasmConfig* prCfg, SceUInt32 addr);
    
#ifdef	__cplusplus
}
#endif

#endif	/* DASMCONFIG_H */

