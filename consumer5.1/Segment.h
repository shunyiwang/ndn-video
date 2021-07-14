/*************************************************************************
	> File Name: Segment.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月09日 星期四 09时11分17秒
 ************************************************************************/

#ifndef _SEGMENT_H
#define _SEGMENT_H

#include<iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

using namespace std;


namespace svcdash{
    class Segment{
        public:
            void InitSegment(unsigned char* _data,int _size);
	    void InitSegmentFromFile(string fileName);

            int Read(unsigned char* out,int length);

            int getSize();
            int getPos();
            
            void setLayer(int _layer);
	    int getLayer();
	
	    void setIndex(int _index);
	    int getIndex();
            
            
        private:
            unsigned char* data;
	    int layer;
	    int index;
            int size;
            int pos;

    };
}

#endif

