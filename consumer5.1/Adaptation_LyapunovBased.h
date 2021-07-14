#include <iostream>
#include "IAdaptation.h"
#include <math.h>

using namespace std;
using namespace svcdash;

namespace svcdash{
	class Adaptation_LyapunovBased:public IAdaptation{
		public:
			int getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep);
		private:
			double paramQ;
			double paramX;
			double paramH=-1;
			double paramZ=-1;
			double paramD;
			double paramU;
			double f;
			double yeta;
			double theta;
			double V;
			bool initial=false;

			int layerNum;
			int segmentDuration;
			double segmentInterval;
			vector<double> bitRates;
			
			double getPositive(double num);
			double getU(int layer);

	};
}
