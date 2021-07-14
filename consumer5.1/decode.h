/*************************************************************************
	> File Name: play.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月08日 星期三 16时48分38秒
 ************************************************************************/

#ifndef _PLAY_H
#define _PLAY_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <time.h>
#include "Segment.h"  
#include "stdint.h"
  
#define __STDC_CONSTANT_MACROS  
  
#ifdef _WIN32  
//Windows  
extern "C"  
{  
#include "libavcodec/avcodec.h"  
#include "libavformat/avformat.h"  
#include "libswscale/swscale.h"  
#include "libavutil/imgutils.h"  
#include "SDL2/SDL.h"  
};  
#else  
//Linux...  
#ifdef __cplusplus  
extern "C"  
{  
#endif  
#include <libavcodec/avcodec.h>  
#include <libavformat/avformat.h>  
#include <libswscale/swscale.h>  
#include <SDL2/SDL.h>  
#include <libavutil/imgutils.h>  
#ifdef __cplusplus  
};  
#endif  
#endif  
  
namespace svcdash{
	class decodeFrame{
		public:
			AVFrame* pFrameYUV;
	};

	class decodeSegment{
		public:
			queue<decodeFrame> dcSegment;
			int index;
			int layer;
	};

	class LibDecode{
		public:
			int Decode(Segment &seg,decodeSegment &dcSeg);
		//private:
		//	int ioRead(void * opaque,uint8_t *buf, int bufsize);
	};
} 
 

#endif
