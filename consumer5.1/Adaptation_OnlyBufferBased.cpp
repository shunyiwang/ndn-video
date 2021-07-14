#include "Adaptation_OnlyBufferBased.h"

using namespace svcdash;

int Adaptation_OnlyBufferBased::getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep){
	int layerNum=concep.size();
	double interval=1.0/(double)layerNum;
	for(int i=0;i<layerNum;i++){
		if(bufferState==0){return 1;}
		if(bufferState>=i*interval&&bufferState<=(i+1)*interval){return i+1;}
	}
}
