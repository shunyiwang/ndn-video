/*************************************************************************
	> File Name: downparsecombine.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年03月08日 星期三 10时58分57秒
 ************************************************************************/

#include "svcExamPlat.h"

///lock
mutex g_lock1;
mutex g_lock2;

///const value,restrict
const int download_Max=2;
const int buffer_Max=5;


///int global_SegIndex;
//int AdapatationMode=0;

int global_SegNumMax;
int Width;
int Height;
int FrameRate;
int SegDuration;
double Interval;
queue<Segment> download_Buffer;//download buffer
queue<decodeSegment> decode_Buffer;//decode buffer
vector<downloadRecordItem> download_Record;//download Record deque

///mpd parse content
mpdManager mpd_Manager;
string InitSegName;

///function : get buffer state
double getBufferState(){
	if(decode_Buffer.empty()){return 0;}
	return (double)((decode_Buffer.size()-1)*SegDuration+decode_Buffer.front().dcSegment.size())/(double)(buffer_Max*SegDuration);
}

///Download Thread
void Download(){
    downloadManager download;
    Combine combine;

    ///Log
    SimpleLog downloadLog;
    SimpleLog bufferLog;
    SimpleLog downRecordLog;
    
    double downloadBitRate;//download time record per total request round
    double bufferState;//buffer state 
    
    ///history
    double lastBitRate=0;
    int lastLayer=0;
    int lastLayerSwitch=0;

    int segIndex=0;
    while(segIndex<global_SegNumMax){
        if(download_Buffer.size()<download_Max){
  		vector<string> segs;
		downloadRecordItem recordItem;
		
		///record buffer state
		bufferState=getBufferState();
		
		///adaptation make decision

                cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
                cout << "dowloading segment" << segIndex << " bufferState: " << bufferState << endl;
		mpd_Manager.getSegmentByAdaptation(segIndex,lastBitRate,lastLayer,lastLayerSwitch,bufferState,segs);
		//mpd_Manager.getSegment(segIndex,segs);

		bufferLog.Log_1(segIndex,bufferState);//log in file

		downloadBitRate=download.Download(segs);
		downloadLog.Log(segIndex,downloadBitRate);//log in file
		
		///record index bitRate layer in vector
		recordItem.bitRate=downloadBitRate;
		lastBitRate=downloadBitRate;
		recordItem.segIndex=segIndex;
		recordItem.layer=segs.size();
		lastLayer=segs.size();
		if(download_Record.empty()){recordItem.layerSwitch=0;lastLayerSwitch=0;}
		else{recordItem.layerSwitch=recordItem.layer-download_Record[download_Record.size()-1].layer;lastLayerSwitch=recordItem.layer-download_Record[download_Record.size()-1].layer;}
		download_Record.push_back(recordItem);	
		downRecordLog.Log(recordItem.segIndex,recordItem.bitRate,recordItem.layer,recordItem.layerSwitch);
		//downRecordLog.Log(segIndex,lastBitRate,lastLayer,lastLayerSwitch);	

        	string cmName=combine.merge(InitSegName,segs);
		Segment seg;
		
		seg.InitSegmentFromFile(cmName);
		seg.setIndex(segIndex);
		seg.setLayer(segs.size());

		g_lock1.lock();
		download_Buffer.push(seg);
		g_lock1.unlock();

		segIndex++;
		cout<<"Thread Download : download a segment"<<endl;
        }
	else{
		usleep(10);
	}
    }
    //return;
}

///Decode Thread
void Decode(){
    LibDecode decode;
    while(true){
	if(decode_Buffer.size()<buffer_Max){
	    if(!download_Buffer.empty()){
		decodeSegment dcSeg;
		decode.Decode(download_Buffer.front(),dcSeg);

		g_lock1.lock();
		download_Buffer.pop();
		g_lock1.unlock();

		g_lock2.lock();
		decode_Buffer.push(dcSeg);
		g_lock2.unlock();

		cout<<"Thread Decode : decode a segment"<<endl;
	    }
	}
	else{
		usleep(10);
	}
    }
}

int Play(){

    //log
    SimpleLog playLog;

    int screen_w=Width,screen_h=Height; 
    double frameIterval=1000/FrameRate;
 
    SDL_Window *screen;   
    SDL_Renderer* sdlRenderer;  
    SDL_Texture* sdlTexture;  
    SDL_Rect sdlRect;        

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {    
        printf( "Could not initialize SDL - %s\n", SDL_GetError());   
        return -1;  
    }   
  
    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,  
        screen_w, screen_h,  
        SDL_WINDOW_OPENGL);  
  
    if(!screen) {    
        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());    
        return -1;  
    }  
  
    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);    
    //IYUV: Y + U + V  (3 planes)  
    //YV12: Y + V + U  (3 planes)  
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,screen_w,screen_h);    
  
    sdlRect.x=0;  
    sdlRect.y=0;  
    sdlRect.w=screen_w;  
    sdlRect.h=screen_h;  
  
    //SDL End----------------------  
    while(true){
	if(!decode_Buffer.empty()){
		playLog.Log(decode_Buffer.front().index,decode_Buffer.front().layer);//log in file

		while(!decode_Buffer.front().dcSegment.empty()){
			SDL_UpdateYUVTexture(sdlTexture, &sdlRect,  
			decode_Buffer.front().dcSegment.front().pFrameYUV->data[0], decode_Buffer.front().dcSegment.front().pFrameYUV->linesize[0],  
			decode_Buffer.front().dcSegment.front().pFrameYUV->data[1], decode_Buffer.front().dcSegment.front().pFrameYUV->linesize[1],  
			decode_Buffer.front().dcSegment.front().pFrameYUV->data[2], decode_Buffer.front().dcSegment.front().pFrameYUV->linesize[2]); 

			//SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] ); 
		   
				  
			SDL_RenderClear( sdlRenderer );    
			SDL_RenderCopy( sdlRenderer, sdlTexture,  NULL, &sdlRect);    
			SDL_RenderPresent( sdlRenderer );    
			//SDL End-----------------------  
			//Delay 40ms  
			SDL_Delay(frameIterval);
			decode_Buffer.front().dcSegment.pop();			
		} 

		g_lock2.lock();
		decode_Buffer.pop();
		g_lock2.unlock();
	}else{
	//interrupt happens
		SDL_Delay(frameIterval);
	}
	
    }
     
  
    SDL_Quit();  
 
}

int main(){
    string mpdFileName; 
    char next;
    int adaptationMode;

    downloadManager dlManager;
    Parse parse;

    //mpd download and parse
    cout<<"///please input the video MPD URL:"<<endl;
    cin>>mpdFileName;
    cout<<"Check if "<<mpdFileName<<" is right?(Y/N)"<<endl;
    cin>>next;
    if(next=='N'||next=='n'){
        cout<<"////END////"<<endl;
        return 0;
    }
    ///mpd download
    if(dlManager.downloadMPD(mpdFileName)){
        cout<<"///"<<mpdFileName<<" is downloaded"<<endl;
    }
    ///mpd parse
    parse.ParseMPD(mpdFileName,mpd_Manager);
    cout<<"///"<<mpdFileName<<" is parsed"<<endl;
    //cout<<"//////"<<mpd_Manager.getBaseUrl()<<endl;
    Width=mpd_Manager.getWidth();
    Height=mpd_Manager.getHeight();
    FrameRate=mpd_Manager.getFrameRate();
    Interval=mpd_Manager.getInterval();
    SegDuration=mpd_Manager.getSegDuration();
    cout<<"WxH:"<<Width<<"x"<<Height<<" FrameRate:"<<FrameRate<<" Interval:"<<Interval<<"SegDuration:"<<SegDuration<<endl;

    ///Set Adaptation Mode
    cout<<"///please select a kind of Adaptation Mode (Just Input Number):"<<endl;
    cout<<"///[0] Only Lowest"<<endl;
    cout<<"///[1] Only Highest"<<endl;
    cout<<"///[2] Only Rate Based"<<endl;
    cout<<"///[3] Only Buffer Based"<<endl;
    cout<<"///[4] Buffer&Rate Based"<<endl;
    cout<<"///[5] Lyapunov Based"<<endl;
    cin>>adaptationMode;
    mpd_Manager.setAdaptationMode(adaptationMode);

    //segment download and combine
    ///download initSegment
    InitSegName=mpd_Manager.getInitSegment();  //string类
    if(dlManager.downloadInit(InitSegName)){
        cout<<"///"<<InitSegName<<" is downloaded"<<endl;
    }

    global_SegNumMax=mpd_Manager.getMpdConception(0).totalSegments;
    cout<<"///segmentMax:"<<global_SegNumMax<<endl;

    cout<<"start download and play?(Y/N)"<<endl;
    cin>>next;
    if(next=='N'||next=='n'){
        cout<<"////END////"<<endl;
        return 0;
    }

    //thread:Download
    thread *t_download=new thread(Download);
    t_download->detach();

    //thread:Decode
    thread *t_decode=new thread(Decode);
    t_decode->detach();

    //thread:Play
    thread *t_play=new thread(Play);
    t_play->detach();
 
    while(1){}

    
   
}



