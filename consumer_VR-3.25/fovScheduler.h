/*************************************************************************
	> File Name: mpdManager.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月06日 星期一 14时18分12秒
 ************************************************************************/

#ifndef _FOVSCHEDULER_H
#define _FOVSCHEDULER_H

//#include "parse.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <fstream>

#include "mpdManager.h"

struct coordinate{
    int x[3];
};

using namespace std;
//using namespace svcdash;

namespace svcdash{
    class fovScheduler{
        public:
            //fovScheduler();
            ~fovScheduler();
            void getFov(int index, vector<coordinate>& dest, int method);
            void getTilesByAdaptation(int index, vector<coordinate> fov, mpdManager mpds[][4][4], int adp, double lastBitRate, int lastLayer, int lastLayerSwitch, double bufferState, vector<string>& segs);

        private:

    };
}

#endif
