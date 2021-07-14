#include <iostream>
#include "IAdaptation.h"

using namespace std;
using namespace svcdash;

namespace svcdash{
	class Adaptation_OnlyLowest:public IAdaptation{
		public:
			int getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep);
	};
}
