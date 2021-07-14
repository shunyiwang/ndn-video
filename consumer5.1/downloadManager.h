/*************************************************************************
	> File Name: downloadManager.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月08日 星期三 08时53分12秒
 ************************************************************************/

#ifndef _DOWNLOADMANAGER_H
#define _DOWNLOADMANAGER_H

#include <string.h>
#include <vector>
#include <time.h>
#include "dash-consumer.h"

using namespace ndn;
using namespace ndn::ndndash;
using namespace std;

namespace svcdash{
    class downloadRecordItem{
        public:
	    int segIndex;
	    double bitRate;
            int layer;
	    int layerSwitch;
        private:

    };
    class downloadManager{
        public:
	    bool downloadMPD(string mpd);
            bool downloadInit(string init);
            double Download(vector<string> segments);
        private:

    };
}




#endif
