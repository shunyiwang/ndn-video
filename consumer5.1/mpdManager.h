/*************************************************************************
	> File Name: mpdManager.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月06日 星期一 14时18分12秒
 ************************************************************************/

#ifndef _MPDMANAGER_H
#define _MPDMANAGER_H

//#include "parse.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "IAdaptation.h"
#include "Adaptation_OnlyLowest.h"
#include "Adaptation_OnlyHighest.h"
#include "Adaptation_OnlyRateBased.h"
#include "Adaptation_OnlyBufferBased.h"
#include "Adaptation_BR_Based.h"
#include "Adaptation_LyapunovBased.h"

using namespace std;
using namespace svcdash;

namespace svcdash{   
    class mpdManager{
        public:
            //mpdManager();
            ~mpdManager();
            int getLocationSize();
            void setLocationSize(int size);
            int getPeriodSize();
            void setPeriodSize(int size);
            int getAdaptationsetSize();
            void setAdaptationsetSize(int size);
	    int getAdaptationMode();
            void setAdaptationMode(int _adaptationMode);
            string getBaseUrl();
            void setBaseUrl(string url);
            string getInitSegment();
            void setInitSegment(string init);
            void getSegment(int index,vector<string>& dest);
            void setSegments(vector<string>& segmentList);
            mpdConception getMpdConception(int index);
            void setMpdConceptions(vector<mpdConception>& concep);
	    
            int getWidth();
	    int getHeight();
	    int getFrameRate();
	    int getSegDuration();
	    double getInterval();
	
            void split(const string& src,const string& seperator,vector<string>& dest);

	    void getSegmentByAdaptation(int segIndex,double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<string>& segs);
            
        private:
            int locationSize;
            int periodSize;
            int adaptationsetSize;
	    int adaptationMode;
            svcdash::IAdaptation* adaptation;
            
            string baseUrl;
            string initSegment;
            vector<string> segments;
            vector<mpdConception> conceptions;
    };
}

#endif
