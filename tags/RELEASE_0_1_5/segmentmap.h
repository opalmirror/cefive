/* 
 * File:   segmentmap.h
 * Author: Sir Gee of Five
 *
 * Created on October 8, 2010, 4:28 PM
 */

#ifndef _SEGMENTMAP_H
#define	_SEGMENTMAP_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <pspkerneltypes.h>

    #define SEGMENTMAP_NAME_LEN     64
    #define SEGMENTMAP_MAX_SEGMENTS 256

    typedef struct _SegmentView {
        SceOff segStart;
        SceOff segEnd;
        char segName[SEGMENTMAP_NAME_LEN + 1];
    }SegmentView;

    typedef struct _SegmentList {
        SegmentView arSegment[SEGMENTMAP_MAX_SEGMENTS];
        int size;
    }SegmentList;


#ifdef	__cplusplus
}
#endif

#endif	/* _SEGMENTMAP_H */

