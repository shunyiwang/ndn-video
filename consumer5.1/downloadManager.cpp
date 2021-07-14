/*************************************************************************
	> File Name: downloadManager.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年03月08日 星期三 08时59分41秒
 ************************************************************************/

#include "downloadManager.h"

using namespace svcdash;

bool downloadManager::downloadMPD(string mpd){
	NdnDashConsumer consumer;
        consumer.getRequestNumber(mpd);
        for(size_t j=0;j<consumer.getSegmentNumber();j++){
            consumer.getRequestData(mpd,j);
        }
        consumer.Buffer(mpd);
	return true;
}

bool downloadManager::downloadInit(string init){
	return downloadMPD(init);
}

double downloadManager::Download(vector<string> segments){

    double totalSize=0;
    double downloadTime;
    int layerSize=segments.size(); 
    //clock_t start,end,totalTime=0;
    clock_t start,end;
    start=clock();
    for(int i=0;i<layerSize;i++){
	//start=clock();
        NdnDashConsumer consumer;
        consumer.getRequestNumber(segments[i]);
        for(size_t j=0;j<consumer.getSegmentNumber();j++){
            consumer.getRequestData(segments[i],j);
        }
	//end=clock();
	//totalTime+=end-start;
        consumer.Buffer(segments[i]);
        totalSize+=consumer.getSegmentSize();
    }
    end=clock();
    downloadTime=(double)(end-start)/CLOCKS_PER_SEC;
    //downloadTime=(double)(totalTime)/CLOCKS_PER_SEC;
    return (totalSize/1024.0)/downloadTime;//return value is bitpersecond
}

/*
int main(){
    downloadManager down;
    string mpd="TOS-I-360p.mpd";
    string baseFile="TOS-I-360p.seg0-L0.svc";
    string enhanFile1="TOS-I-360p.seg0-L1.svc";
    string enhanFile2="TOS-I-360p.seg0-L2.svc";
    string enhanFile3="TOS-I-360p.seg0-L3.svc";
    vector<string> segFile;
    segFile.push_back(baseFile);
    segFile.push_back(enhanFile1);
    segFile.push_back(enhanFile2);
    segFile.push_back(enhanFile3);
    down.downloadMPD(mpd);
    down.Download(segFile);

    return 0;
}*/
