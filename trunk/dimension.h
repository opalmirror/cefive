/* 
 * File:   dimension.h
 * Author: Sir Gee of Five
 *
 * Created on September 20, 2010, 10:33 AM
 */

#ifndef _DIMENSION_H
#define	_DIMENSION_H

#include <pspkerneltypes.h>
    
#define DIMENSION_SUCCESS       (0)
#define DIMENSION_FAILURE       (-1)
#define DIMENSION_MEMORY        (-2)
#define DIMENSION_NULLPTR       (-3)
#define DIMENSION_IOERROR       (-4)
    
#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct _Dimension {
        int width;
        int height;
    }Dimension;

    int dimension_copy(Dimension* prDest, Dimension* prSrc);
    
    int dimension_init(Dimension* prDim);
    
    int dimension_read(Dimension* prDim, SceUID fd);
    
    int dimension_set(Dimension* prDim, int width, int height);
    
    int dimension_write(Dimension* prDim, SceUID fd);
    
#ifdef	__cplusplus
}
#endif

#endif	/* _DIMENSION_H */
