#include "Adaptation_BR_Based.h"

using namespace svcdash;

int Adaptation_BR_Based::getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep){
	int decisionByBuffer;
	int layerNum=concep.size();
	double interval=1.0/(double)layerNum;
	for(int i=0;i<layerNum;i++){
		if(bufferState==0){decisionByBuffer=1;break;}
		if(bufferState>=i*interval&&bufferState<=(i+1)*interval){decisionByBuffer=i+1;break;}
	}

	int decisionByRate;
	vector<double> bitRates;
	if(lastBitRate==0){decisionByRate=1;}
	else{
		for(int i=0;i<concep.size();i++){
		bitRates.push_back((double)concep[i].bandwidth/(8.0*1024.0));
		}
		int index=0;
		while(index<bitRates.size()){
			if(lastBitRate>=bitRates[index]){index++;}
			else{break;}
		}
		if(index==0){decisionByRate=1;}
		else{decisionByRate=index;}
	}

	if(decisionByRate==decisionByBuffer){return decisionByBuffer;}
	else{return (int)((decisionByRate+decisionByBuffer)/2);}
	
}
