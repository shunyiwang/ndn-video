/*************************************************************************
	> File Name: mpdManager.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月06日 星期一 14时18分12秒
 ************************************************************************/

#ifndef _TILEADAPTATIONLOWEST_H
#define _TILEADAPTATIONLOWEST_H

//#include "parse.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

struct coordinate{
    int x[3];
};

using namespace std;
using namespace svcdash;

namespace svcdash{   
    class fovScheduler{
        public:
            //fovScheduler();
            ~fovScheduler();
            void getFov(int index, vector<coordinate>& dest, int method);

        private:

    };
}

#endif
