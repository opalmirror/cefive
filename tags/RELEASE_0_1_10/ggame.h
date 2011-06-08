/* 
 * File:   ggame.h
 * Author: Sir Gee of Five
 *
 * Created on May 14, 2011, 2:19 PM
 */

#ifndef GGAME_H
#define	GGAME_H

#include <stdio.h>
#include <string.h>
#include <pspkerneltypes.h>
#include <pspiofilemgr.h>
#include <psploadcore.h>
#include "glabel.h"
#include "glabelmap.h"
#include "gsegmap.h"
#include "gsegment.h"
#include "gstubmap.h"
#include "modstub.h"

/** Indicates success. */
#define GGAME_SUCCESS (0)

/** Indicates failure. */
#define GGAME_FAILURE (-1)

/** Indicates a NULL pointer. */
#define GGAME_NULLPTR (-2)

/** Indicates a memory error. */
#define GGAME_MEMORY (-3)

/** Indicates an I/O error. */
#define GGAME_IOERROR (-4)

/** Indicates an invalid path or file name. */
#define GGAME_INVPATH (-5)

/** Indicates an invalid module. */
#define GGAME_INVMODULE (-6)

/** The length of a Game Id. */
#define GGAME_GAMEID_LEN (10)

/** The maximum length of a path or file name. */
#define GGAME_PATH_LEN (255)

/** The default data path for a Game. */
#define GGAME_DEF_DATAPATH "disc0:/UMD_DATA.BIN"

/** The default module address for a Game. */
#define GGAME_MODULE_ADDR ((unsigned int)0x08804000)

/** The maximum number of SceLibraryStubTable pointers in the Stub Table. */
#define GGAME_STUBTABLE_SIZE (128)

/** The SCE nId for the module_start function. */
#define GGAME_NID_MODSTART ((SceUInt32)0xD632ACDB)

/** The SCE nId for the module_stop function. */
#define GGAME_NID_MODSTOP ((SceUInt32)0xCEE8593C)

/** The SCE nId for the module_info struct. */
#define GGAME_NID_MODINFO ((SceUInt32)0xF01D73A7)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _GGame {
        /** The SCE Game Id for the Game. */
        char sGameId[GGAME_GAMEID_LEN + 1];
        /** The path to locate the Game Id. */
        char sDataPath[GGAME_PATH_LEN + 1];
        /** Pointer to the Game Module. */
        SceModule* gameModule;
        /** Array of SceLibraryStubTable pointers. */
        SceLibraryStubTable* stubtable[GGAME_STUBTABLE_SIZE];
        /** Segment Map struct. */
        GsegMap segmentMap;
        /** Stub Map struct. */
        GstubMap stubMap;
        /** Label Map struct. */
        GlabelMap labelMap;
    } 
    /** The GGame struct is used to represent a Game. */
    GGame;

    /** Add a Label to a Game.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @param address SceUInt32 indicating the address of the Label to add.
     * @param name string containing the text of the Label to add.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int ggame_add_label(GGame* prGame, const SceUInt32 address, 
            const char* name);

    /** Return a pointer to a Glabel struct representing the Label for the
     * indicated address, if one exists.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @param address SceUInt32 indicating the address to return a Label for.
     * @return A pointer to a Glabel struct or NULL is returned.
     */
    Glabel* ggame_find_label(GGame* prGame, const SceUInt32 address);
    
    /** Return a pointer to a Gsegment struct representing the segment that the
     * specified address falls within.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @param address SceUInt32 indicating the address to return the segment of.
     * @return A pointer to a Gsegment struct or NULL is returned.
     */
    Gsegment* ggame_find_segment(GGame* prGame, const SceUInt32 address);
    
    /** Return the number of kernel modules exported by a Game.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return An int indicating the number of exported modules is returned.
     */
    int ggame_get_exportcount(GGame* prGame);
    
    /** Return a pointer to a SceLibraryEntryTable struct representing the head
     * element of the Game Kernel Module export table.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return A pointer to a SceLibraryEntryTable struct or NULL is returned.
     */
    SceLibraryEntryTable* ggame_get_exporttable(GGame* prGame);
    
    /** Return a pointer to a SceLibraryStubTable struct representing the head
     * element of the Game Kernel Module import table.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return A pointer to a SceLibraryStubTable struct or NULL is returned.
     */
    SceLibraryStubTable* ggame_get_importtable(GGame* prGame);
    
    /** Return a pointer to a GlabelMap struct representing the Label Map.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return A pointer to a GlabelMap struct or NULL is returned.
     */
    GlabelMap* ggame_get_labelmap(GGame* prGame);
    
    /** Return a pointer to a SceModule struct representing the Game kernel
     * module.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return A pointer to a SceModule struct or NULL is returned.
     */
    SceModule* ggame_get_module(GGame* prGame);
    
    /** Return the address of the module_start function for a Game.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return A SceUInt32 containing the address of module_start is returned.
     */
    SceUInt32 ggame_get_module_start(GGame* prGame);
    
    /** Return a pointer to a GsegMap struct representing the Segment Map.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @return A pointer to a GsegMap struct or NULL is returned.
     */
    GsegMap* ggame_get_segmap(GGame* prGame);
    
    /** Return a pointer to a GstubMap struct representing the Stub Map.
     * 
     * @param prGame Pointer to a GGame struct representing a Game.
     * @return A pointer to a GstubMap struct or NULL is returned.
     */
    GstubMap* ggame_get_stubmap(GGame* prGame);
    
    /** Initialize a Game.
     * 
     * @param prGame Pointer to the GGame struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int ggame_init(GGame* prGame);
    
    /** Load the kernel module exports from the Game, mapping the Stubs within
     * the Game Stub Map and creating Labels within the Label Map.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int ggame_load_exports(GGame* prGame);
    
    /** Load the SCE Game Id from the data path.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int ggame_load_gameid(GGame* prGame);

    /** Load the kernel module imports from the Game, mapping the Stubs within
     * the Game Stub Map, adding Segments to the Game Segment Map and creating
     * Labels within the Label Map.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int ggame_load_imports(GGame* prGame);

    /** Load the Game Module and map the appropriate data.
     * 
     * @param prGame Pointer to a GGame struct representing the Game.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int ggame_load_module(GGame* prGame);
    
#ifdef	__cplusplus
}
#endif

#endif	/* GGAME_H */
