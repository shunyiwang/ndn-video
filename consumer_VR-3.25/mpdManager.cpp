/*************************************************************************
	> File Name: mpdManager.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年03月06日 星期一 14时35分15秒
 ************************************************************************/

#include "mpdManager.h"
//using namespace std;
using namespace svcdash;

//mpdManager::mpdManager(){
//}

mpdManager::~mpdManager(){
}

int mpdManager::getLocationSize(){
    return locationSize;
}

void mpdManager::setLocationSize(int size){
    locationSize=size;
}

int mpdManager::getPeriodSize(){
    return periodSize;
}

void mpdManager::setPeriodSize(int size){
    periodSize=size;
}

int mpdManager::getAdaptationsetSize(){
    return adaptationsetSize;
}

void mpdManager::setAdaptationsetSize(int size){
    adaptationsetSize=size;
}

int mpdManager::getAdaptationMode(){
    return adaptationMode;
}

void mpdManager::setAdaptationMode(int _adaptationMode){
    adaptationMode=_adaptationMode;
    switch(adaptationMode){
	case 0:
		adaptation=new Adaptation_OnlyLowest();
		break;
	case 1:
		adaptation=new Adaptation_OnlyHighest();
		break;
	case 2:
		adaptation=new Adaptation_OnlyRateBased();
		break;
	case 3:
		adaptation=new Adaptation_OnlyBufferBased();
		break;
	case 4:
		adaptation=new Adaptation_BR_Based();
		break;
	case 5:
		adaptation=new Adaptation_LyapunovBased();
		break;
	default:
		adaptation=new Adaptation_OnlyLowest();
		break;
	
    }
}

string mpdManager::getBaseUrl(){
    return baseUrl;
}

void mpdManager::setBaseUrl(string url){
    baseUrl=url;
}

string mpdManager::getInitSegment(){
    return initSegment;
}

void mpdManager::setInitSegment(string init){
    initSegment=init;
}

void mpdManager::getSegment(int index,vector<string>& dest){
    split(segments[index],",",dest);
}

void mpdManager::setSegments(vector<string>& segmentList){
    for(int i=0;i<segmentList.size();i++){
        segments.push_back(segmentList[i]);
    }
}

mpdConception mpdManager::getMpdConception(int index){
    return conceptions[index]; 
}

void mpdManager::setMpdConceptions(vector<mpdConception>& concep){
    for(int i=0;i<concep.size();i++){
        conceptions.push_back(concep[i]);
    }
}

int mpdManager:: getWidth(){
    return conceptions[0].width;
}

int mpdManager:: getHeight(){
    return conceptions[0].height;
}

int mpdManager:: getFrameRate(){
    return atoi((conceptions[0].frameRate).c_str());
}

int mpdManager:: getSegDuration(){
    return conceptions[0].segmentDuration;
}

double mpdManager:: getInterval(){
    return (double)conceptions[0].segmentDuration/(double)getFrameRate();
}

void mpdManager::split(const string& src,const string& seperator,vector<string>& dest){
    string str=src;
    string substring;
    string::size_type start=0,index;
    do{
        index=str.find_first_of(seperator,start);
        if(index!=string::npos){
            substring=str.substr(start,index-start);
            dest.push_back(substring);
            start=str.find_first_not_of(seperator,index);
            if(start==string::npos){
                return;
            }
        }
    }while(index!=string::npos);
    substring=str.substr(start);
    dest.push_back(substring);
}

void mpdManager::getSegmentByAdaptation(int segIndex,double lastBitRate,int lastLayer,int lastLayerSwitch,double bufferState,vector<string>& segs){
	int decision=adaptation->getNextSegment(lastBitRate,lastLayer,lastLayerSwitch,bufferState,conceptions);
        vector<string> segments;     //这里每个segmnets单元对应各层；而private成员segments每个单元对应一个segment的多个层;decision对应的就是该segment[index]应该请求几层
	getSegment(segIndex,segments);
        for(int i=0;i<decision;i++){
		segs.push_back(segments[i]);
        }
}

/*
int main(int argc,char* argv[]){
    mpdManager test;
    vector<string> dest;
    test.split(argv[1],argv[2],dest);
    for(int i=0;i<dest.size();i++){
        cout<<dest[i]<<endl;
    }
    return 0;

}
*/
