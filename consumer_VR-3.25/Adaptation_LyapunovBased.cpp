#include "Adaptation_LyapunovBased.h"

using namespace svcdash;

double Adaptation_LyapunovBased::getPositive(double num){
	if(num<0){return 0;}
	return num;
}

double Adaptation_LyapunovBased::getU(int layer){
	double gap=1.0/(double)(bitRates.size());
	return gap*(double)(layer+1);
}

int Adaptation_LyapunovBased::getNextSegment(double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<mpdConception>& concep){
        if(!initial){                                           //最开始请求segment0，做初始化工作
		layerNum=concep.size();
		f=atoi((concep[0].frameRate).c_str());
		segmentDuration=concep[0].segmentDuration;
		segmentInterval=(double)segmentDuration/(double)f;
		for(int i=0;i<concep.size();i++){
			bitRates.push_back((double)concep[i].bandwidth/(8.0*1024.0));
		}
		
		///parameter
		theta=2;
		yeta=0.5;
		V=40;	
	}
	
	
	paramQ=bufferState*5*segmentDuration;//5 is solid 
	paramX=abs(lastLayerSwitch);

	if(paramH==-1){paramH=theta;}
	else{paramH=getPositive(paramH-paramQ)+theta;}
	if(paramZ==-1){paramZ=paramX;}
	else{paramZ=getPositive(paramZ-yeta)+paramX;}

	double min=0;
	double sum=0;
	int layer;
	for(int i=0;i<layerNum;i++){
		sum+=bitRates[i];
		paramD=(double)segmentDuration*lastBitRate*segmentInterval/sum;
		paramU=getU(i);
		double temp=(paramQ-paramH)*(paramD-segmentInterval*f)+paramZ*(paramX-yeta)-V*paramU;
		if(min==0){min=temp;layer=1;}
		else if(temp<min){
			min=temp;
			layer=i+1;
		}
	}

	return layer;	
}
