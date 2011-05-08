/* colorconfig.h
 *  ColorConfiguration for Cheat Engine of Five.
 * Author: Sir Gee of Five
 * 
 */

#ifndef _COLORCONFIG_H
#define _COLORCONFIG_H

#include <pspkerneltypes.h>

/** Indicates success. */
#define COLORCONFIG_SUCCESS     (0)

/** Indicates failure. */
#define COLORCONFIG_FAILURE     (-1)

/** Indicates memory error. */
#define COLORCONFIG_MEMORY      (-2)

/** Indicates NULL pointer. */
#define COLORCONFIG_NULLPTR     (-3)

/** Default background color */
#define COLORCONFIG_BACKGROUND  ((u32)0x00000000)

/** Default foreground color */
#define COLORCONFIG_TEXT        ((u32)0xFFFFFFFF)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _ColorConfig {
        /** Background Text Color */
        u32 background;
        /** Foreground Text Color */
        u32 text;
    }
    /** Color Configuration structure.  Used to specify a background and 
     * foreground color for display.
     */
    ColorConfig;

    /** Copy a ColorConfig struct.
     * 
     * @param prDest Pointer to the ColorConfig struct to act as destination.
     * @param prSrc Pointer to the ColorConfig struct to act as source.
     * @return COLORCONFIG_NULLPTR is returned if either parameter is NULL.
     * COLORCONFIG_SUCCESS is returned if the ColorConfig struct is copied.
     */
    int colorconfig_copy(ColorConfig* prDest, ColorConfig* prSrc);

    /** Initialize a ColorConfig struct.
     * 
     * @param prConfig pointer to the ColorConfig struct to initialize.
     * @return 0 on success, negative value on failure.
     */
    int colorconfig_init(ColorConfig* prConfig);

    /** Read a ColorConfig struct from an open file descriptor.
     *  
     * @param prConfig pointer to the ColorConfig struct to populate.
     * @param fd SceUID file descriptor.
     * @return 0 on success, negative values on failure.
     */
    int colorconfig_read(ColorConfig* prConfig, SceUID fd);

    /** Assign the values of a ColorConfig struct.
     * 
     * @param prConfig pointer to the destination ColorConfig struct.
     * @param background u32 value containing the background color.
     * @param text u32 value containing the text color.
     * @return 0 on success, negative values on failure.
     */
    int colorconfig_setcolor(ColorConfig* prConfig, u32 background, u32 text);

    /** Write a ColorConfig struct to an open file descriptor.
     * 
     * @param prConfig pointer to the ColorConfig struct to write.
     * @param fd SceUID an open file descriptor.
     * @return 0 on success, negative values on failure.
     */
    int colorconfig_write(ColorConfig* prConfig, SceUID fd);

#ifdef	__cplusplus
}
#endif

#endif /* _COLORCONFIG_H */
