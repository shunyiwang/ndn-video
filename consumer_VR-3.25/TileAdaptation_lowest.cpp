/*************************************************************************
        > File Name: fovScheduler.cpp
        > Author:
        > Mail:
        > Created Time: 2017年03月06日 星期一 14时35分15秒
 ************************************************************************/

#include "TileAdaptation_lowest.h"
using namespace std;
using namespace svcdash;

//fovScheduler::fovScheduler(){
//}

fovScheduler::~fovScheduler(){
}

void fovScheduler::getFov(int index, vector<coordinate>& dest, int method){
    if(method == 0){
        cout << "request for all the tiles" << endl;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++)
                for(int k=0; k<4; k++){
                    t.a[0] = i+1;
                    t.a[1] = j+1;
                    t.a[2] = k+1;
                    dest.push_back(t);
                }
        }
    }
}

