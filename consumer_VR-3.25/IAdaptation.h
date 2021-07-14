/*************************************************************************
	> File Name: IAdaptation.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月13日 星期一 13时10分14秒
 ************************************************************************/

#ifndef _IADAPTATION_H
#define _IADAPTATION_H

#include <iostream>
#include <vector>
#include "mpdConception.h"

using namespace std;
using namespace svcdash;

namespace svcdash{
	class IAdaptation{
		public:
			virtual int getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep){
				return -1;
			}
	};
}



#endif
