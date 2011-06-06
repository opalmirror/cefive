/* 
 * File:   resultmodel.h
 * Author: Sir Gee of Five
 *
 * Created on May 28, 2011, 11:42 AM
 */

#ifndef RESULTMODEL_H
#define	RESULTMODEL_H

#include <stdio.h>
#include <pspkerneltypes.h>

/** Indicates success. */
#define RESULTMODEL_SUCCESS (0)

/** Indicates failure. */
#define RESULTMODEL_FAILURE (-1)

/** Indicates a NULL pointer. */
#define RESULTMODEL_NULLPTR (-2)

/** Indicates a memory error. */
#define RESULTMODEL_MEMORY (-3)

/** Indicates an invalid index. */
#define RESULTMODEL_INVIDX (-4)

/** The number of Result elements in a Result Model. */
#define RESULTMODEL_SIZE (100)

/** The length of a Text Result. */
#define RESULTMODEL_TEXTLEN (255)

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum _EResultType {
        SRT_None = 0, /** No type specified */
        SRT_UChar8, /** 8-bit Unsigned char */
        SRT_UShort16, /** 16-bit Unsigned short */
        SRT_UInt32, /** 32-bit Unsigned int */
        SRT_String, /** char string */
        SRT_Instruction /** MIPS32 Instruction */
    }
    /** The EResultType enumeration is used to indicate the type of a Result. */
    EResultType;

    typedef struct _Result {
        /** Type of Result */
        EResultType resultType;
        /** Address of Result */
        SceUInt32 address;
        /** Matched Value */
        SceUInt32 value;
        /** Matched Text */
        char text[RESULTMODEL_TEXTLEN + 1];
    }
    /** The Result struct is used to represent a Search Result. */
    Result;

    typedef struct _ResultModel {
        /** Result struct array */
        Result result[RESULTMODEL_SIZE];
        /** Number of Result elements */
        int count;
    }
    /** The ResultModel struct is used to represent a Result Model. */
    ResultModel;

    /** Initialize a Search Result.
     * 
     * @param prResult Pointer to the Result struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int result_init(Result* prResult);

    /** Add a Search Result to a Result Model and return a pointer to the newly
     * added Result struct.
     * 
     * @param prModel Pointer to a ResultModel struct representing the Result
     * Model.
     * @return A pointer to a Result struct or NULL is returned.
     */
    Result* resultmodel_add(ResultModel* prModel);

    /** Reset all of the elements in a Result Model and return the count to 0.
     * 
     * @param prModel Pointer to the ResultModel struct to clear.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultmodel_clear(ResultModel* prModel);

    /** Return a pointer to a Result struct representing the specified Result.
     * 
     * @param prModel Pointer to a ResultModel struct representing the Result
     * Model.
     * @param index int specifying the index of Result to return.
     * @return A pointer to a Result struct or NULL is returned.
     */
    Result* resultmodel_get(ResultModel* prModel, const int index);

    /** Initialize a Result Model.
     * 
     * @param prModel Pointer to the ResultModel struct to initialize.
     * @return 0 indicates success, &lt;0 indicates failure.
     */
    int resultmodel_init(ResultModel* prModel);

#ifdef	__cplusplus
}
#endif

#endif	/* RESULTMODEL_H */
