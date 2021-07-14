#include "Adaptation_OnlyRateBased.h"

using namespace svcdash;

int Adaptation_OnlyRateBased::getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep){
	vector<double> bitRates;
	if(lastBitRate==0){return 1;}
	for(int i=0;i<concep.size();i++){
		bitRates.push_back((double)concep[i].bandwidth/(8.0*1024.0));
	}
	int index=0;
	while(index<bitRates.size()){
                if(lastBitRate>=bitRates[index])    index++;
                else    break;
	}
	if(index==0){return 1;}
	else{return index;}
}
