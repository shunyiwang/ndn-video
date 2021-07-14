/*************************************************************************
        > File Name: fovScheduler.cpp
        > Author:
        > Mail:
        > Created Time: 2017年03月06日 星期一 14时35分15秒
 ************************************************************************/

#include "fovScheduler.h"

using namespace svcdash;
using namespace std;

//fovScheduler::fovScheduler(){
//}

fovScheduler::~fovScheduler(){
}

void fovScheduler::getFov(int index, vector<coordinate>& dest, int method){
    coordinate tf;
    if(method == 0){                                                                    //请求所有的tile
        cout << "request for all the tiles for segment" << index << endl;
        for(int i=0; i<3; i++){
            for(int j=0; j<4; j++)
                for(int k=0; k<4; k++){
                    tf.x[0] = i+1;
                    tf.x[1] = j+1;
                    tf.x[2] = k+1;
                    dest.push_back(tf);
                }
        }
    }
    else if(method == 1){                                                              //请求部分tile（根据设定的FoV轨迹）
        cout << "request for a part of the tiles for segment" << index << endl;
        float x,y;
        int t = index * 2;
        t = t%15;
        if(t>=0 && t<=1){
            x = 0;  y = 0;
        }
        else if(t>1 && t<=5){
            x = 1.5*t - 1.5;    y = 0.5*t - 0.5;
        }
        else if(t>5 && t<=6){
            x = 6; y = 2;
        }
        else if(t>6 && t<=10){
            x = 15 - 1.5*t; y = 0.5*t -1;
        }
        else if(t>10 && t<=11){
            x = 0;  y = 4;
        }
        else{
            x = 0;  y = 15-t;
        }
        x = floor(x);   y = floor(y);
        for(int i=0; i<3; i++)
            for(int j=0; j<3; j++){
                 if((int)(y+j) == 0 || (int)(y+j) == 1)         tf.x[0] = 1;
                 else if((int)(y+j) == 2 || (int)(y+j) == 3)    tf.x[0] = 2;
                 else if((int)(y+j) == 4 || (int)(y+j) == 5)  tf.x[0] = 3;

                 if( (int)(y+j) % 2 == 0 && (x+i >=0 && x+i<=3) )       tf.x[1] = 1;
                 else if( (int)(y+j) % 2 == 0 && (x+i >=4 && x+i<=7) )  tf.x[1] = 2;
                 else if( (int)(y+j) % 2 == 1 && (x+i >=0 && x+i<=3) )  tf.x[1] = 3;
                 else if( (int)(y+j) % 2 == 1 && (x+i >=4 && x+i<=7) )  tf.x[1] = 4;

                 if(x+i==0 || x+i==4)       tf.x[2] = 1;
                 else if(x+i==1 || x+i==5)  tf.x[2] = 2;
                 else if(x+i==2 || x+i==6)  tf.x[2] = 3;
                 else if(x+i==3 || x+i==7)  tf.x[2] =4;

                 dest.push_back(tf);
            }
    }
}
/*
 * 无自适应逻辑，请求fov内所有tile的0层
void fovScheduler::getTilesByAdaptation(int index, vector<coordinate> fov, mpdManager mpds[][4][4], int adpMode, double lastBitRate, int lastLayer, int lastLayerSwitch, double bufferState, vector<string> &segs){
    if(adpMode == 0){
        mpdManager *mpd = **mpds;
        for(int i=0; i<fov.size(); i++){
            vector<string> segments;
            mpd->getSegment(index,segments);
            segs.push_back(segments[0]);

            mpd++;
        }

//        cout << segs.size() << endl;
//        for(int j=0; j<segs.size(); j++){
//            cout << segs[j] << endl;
        }

    }

}
*/


void fovScheduler::getTilesByAdaptation(int index, vector<coordinate> fov, mpdManager mpds[][4][4], int adpMode, double lastBitRate, int lastLayer, int lastLayerSwitch, double bufferState, vector<string> &segs){
    if(adpMode == 0){                                                                       ///Only the Lowest，请求fov内所有tile的0层
        int x,y,z;
        for(int i=0; i<fov.size(); i++){                                                    ///每个segments中存的是对应tile[2][3][3]的0,1,2层的名字
            vector<string> segments;
            x = fov[i].x[0];   y = fov[i].x[1];   z = fov[i].x[2];
            mpds[x-1][y-1][z-1].getSegment(index,segments);

//            cout << segments.size() << endl;
//            for(int k=0; k<segments.size(); k++)
//                cout << segments[k] << endl;

            segs.push_back(segments[0]);
        }

        cout << segs.size() << endl;
        fstream adpMode0;
        adpMode0.open("adpMode0.txt",ios::app);
        for(int j=0; j<segs.size(); j++){
            cout << segs[j] << endl;
            adpMode0 << segs[j] << endl;
        }
        adpMode0 << endl;
        adpMode0.close();

    }

    else if(adpMode == 1){                                                                  ///Only the Highest，请求fov内所有tile的3层
        int x,y,z;
        for(int i=0; i<fov.size(); i++){
            vector<string> segments;
            x = fov[i].x[0];   y = fov[i].x[1];   z = fov[i].x[2];
            mpds[x-1][y-1][z-1].getSegment(index,segments);
            segs.push_back(segments[0]);
            segs.push_back(segments[1]);
            segs.push_back(segments[2]);
        }

        cout << segs.size() << endl;
        fstream adpMode1;
        adpMode1.open("adpMode1.txt",ios::app);
        for(int j=0; j<segs.size(); j++){
            cout << segs[j] << endl;
            adpMode1 << segs[j] << endl;
        }
        adpMode1 << endl;
        adpMode1.close();

    }

    else if(adpMode == 2){                                                                  ///速率自适应
        int x,y,z;
        vector<double> bitRates;
        double sum = 0.0;
        for(int i=0; i<3; i++){
            for(int j=0; j<fov.size(); j++){
                x = fov[j].x[0];   y = fov[j].x[1];   z = fov[j].x[2];
                sum += mpds[x-1][y-1][z-1].getMpdConception(i).bandwidth/(8.0*1024.0);
            }
            bitRates.push_back(sum);
        }
        cout << "Bitrate for segment" << index << " are " << bitRates[0] << "," << bitRates[1] << "," << bitRates[2] << endl;

/*
        double mmm = 0.0;
        fstream out_file;
        //out_file.open("bitrate.txt",ios::out|ios::app);
        out_file.open("bitrate.txt",ios::out);
        string line;
        out_file << "11111111" << endl;
*/

/*
        for(int i=0; i<3; i++){
            for(int j=0; j<4; j++){
                for(int k=0; k<4; k++){
                    mmm = 0.0;
                    for(int l=0; l<3;l++){
                        mmm += mpds[i][j][k].getMpdConception(l).bandwidth/(8.0*1024.0);
                        cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
                        cout << "tile" << i+1 << "_" << j+1 << "_" << k+1 << "  layer" << l << " cumulated bitrate: " << mmm << endl;
                        line = "tile" + to_string(i+1) + "_" + to_string(j+1) + "_" + to_string(k+1) +
                                         "  layer" + to_string(l) + " cumulated bitrate: " + to_string(mmm);
                        cout << line << endl;
                        out_file << line << endl;
                    }
                    out_file << endl;
                }
            }
        }
        out_file.close();

        for(int i=0; i<3; i++){
            mmm += mpds[x-1][y-1][z-1].getMpdConception(i).bandwidth/(8.0*1024.0);
            cout << "%%%%%%%" << mmm << endl;
        }
*/

        int ly = 0;
        if(lastBitRate == 0 || lastBitRate <= bitRates[0]){
            for(int i=0; i<fov.size(); i++){
                vector<string> segments;
                x = fov[i].x[0];   y = fov[i].x[1];   z = fov[i].x[2];
                mpds[x-1][y-1][z-1].getSegment(index,segments);
                segs.push_back(segments[0]);
            }
            cout << "%%%%%%%%%%%^^^^^^^^^^^^^^^^^^^" << endl;
            return;
        }
else{
        while(ly<3){
            if(lastBitRate >= bitRates[ly])    ly++;
            else    break;
        }

        for(int i=0; i<ly; i++){
            for(int j=0; j<fov.size(); j++){
                vector<string> segments;
                x = fov[j].x[0];   y = fov[j].x[1];   z = fov[j].x[2];
                mpds[x-1][y-1][z-1].getSegment(index,segments);
                segs.push_back(segments[i]);
            }
        }
}
        cout << segs.size() << endl;
        fstream adpMode2;
        adpMode2.open("adpMode2.txt",ios::app);
        for(int j=0; j<segs.size(); j++){
            cout << segs[j] << endl;
            adpMode2 << segs[j] << endl;
        }
        adpMode2 << endl;
        adpMode2.close();
    }

}













