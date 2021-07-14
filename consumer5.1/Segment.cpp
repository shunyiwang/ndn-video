/*************************************************************************
	> File Name: Segment.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年03月09日 星期四 09时11分30秒
 ************************************************************************/

#include "Segment.h"

using namespace svcdash;

void Segment::InitSegment(unsigned char* _data,int _size){
    size=_size;
    data=new unsigned char[size];
    memcpy(data,_data,size);
    pos=0;
}

int Segment::Read(unsigned char* out,int length){
    if(pos==size-1){
        return -1;
    }
    if(pos+length>size-1){
        int reallen=size-pos;
        memcpy(out,data+pos,reallen);
        pos+=reallen;
        return reallen;
    }
    memcpy(out,data+pos,length);
    pos+=length;
    return length;
}

void Segment::InitSegmentFromFile(string fileName){
    string path="./merge/"+fileName;
    char* buffer;
    long size;
    ifstream in(path,ios::in|ios::binary|ios::ate);
    size=in.tellg();
    in.seekg(0,ios::beg);
    buffer=new char[size];
    in.read(buffer,size);
    in.close();
    
    InitSegment((unsigned char*)buffer,size);
    cout<<"InitSegmentFromFile"<<size<<endl;
}

int Segment::getSize(){
    return size;
}

int Segment::getPos(){
    return pos;
}

void Segment::setLayer(int _layer){
    layer=_layer;
}

int Segment::getLayer(){
    return layer;
}

void Segment::setIndex(int _index){
    index=_index;
}

int Segment::getIndex(){
    return index;
}

//bool Segment::isReadOver(){
//    if(pos=size){
//        return true;
//    }
//    return false;
//}
