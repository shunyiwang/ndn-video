/*************************************************************************
	> File Name: play.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年03月08日 星期三 16时48分50秒
 ************************************************************************/

#include "decode.h"
using namespace std;
using namespace svcdash;


int ioRead(void * opaque,uint8_t *buf, int bufsize){
    Segment* seg=(Segment*) opaque;
    //cout<<"ioRead:"<<seg->getSize()<<endl;
    return seg->Read(buf,bufsize);
} 

int LibDecode::Decode(Segment &seg,decodeSegment &dcSeg){
    clock_t start=clock();
    AVFormatContext *pFormatCtx;  
    AVIOContext *avio;
    unsigned char *iobuffer=new unsigned char[32768];
    AVCodecContext  *pCodecCtx;  
    AVCodec         *pCodec;
    AVPacket *packet;
    AVFrame *pFrame;
    unsigned char *out_buffer;   
    struct SwsContext *img_convert_ctx; 
    
    int             i, videoindex;  
    int ret;
    int got_picture; 

    dcSeg.index=seg.getIndex();
    dcSeg.layer=seg.getLayer();

    av_register_all();  
    avformat_network_init();  
    pFormatCtx = avformat_alloc_context();  

    avio=avio_alloc_context(iobuffer,32768,0,&seg,ioRead,NULL,NULL);
    pFormatCtx->pb=avio;
    
    if(avformat_open_input(&pFormatCtx,"",NULL,NULL)!=0){  
        printf("Couldn't open input stream.\n");  
        return -1;  
    }  
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){  
        printf("Couldn't find stream information.\n");  
        return -1;  
    }  
    videoindex=-1;  
    for(i=0; i<pFormatCtx->nb_streams; i++)   
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){  
            videoindex=i;  
            break;  
        }  
    if(videoindex==-1){  
        printf("Didn't find a video stream.\n");  
        return -1;  
    }  
  
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;  
    
    cout<<"pix_fmt:"<<(pCodecCtx->pix_fmt==AV_PIX_FMT_YUV420P)<<endl;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);  
    if(pCodec==NULL){  
        printf("Codec not found.\n");  
        return -1;  
    }  
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){  
        printf("Could not open codec.\n");  
        return -1;  
    }
	

    pFrame=av_frame_alloc();
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    while(av_read_frame(pFormatCtx, packet)>=0){  
        if(packet->stream_index==videoindex){ 
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);  
            if(ret < 0){  
                printf("Decode Error.\n");  
                return -1;  
            }
	    if(got_picture){
		out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));

		decodeFrame df;
		df.pFrameYUV=av_frame_alloc();

		av_image_fill_arrays(df.pFrameYUV->data, df.pFrameYUV->linesize,out_buffer,AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);
		sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, df.pFrameYUV->data, df.pFrameYUV->linesize);
		
                dcSeg.dcSegment.push(df);
	    }	
            
        }
        av_free_packet(packet);  
    }

    while (1) {  
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);  
        if (ret < 0)  
            break;  
        if (!got_picture)  
            break;  
        out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));

	decodeFrame df;
	df.pFrameYUV=av_frame_alloc();

	av_image_fill_arrays(df.pFrameYUV->data, df.pFrameYUV->linesize,out_buffer,AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);
	sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, df.pFrameYUV->data, df.pFrameYUV->linesize);
		
        dcSeg.dcSegment.push(df);
	av_free_packet(packet); 
    }
        
    avcodec_close(pCodecCtx);  
    avformat_close_input(&pFormatCtx);
    sws_freeContext(img_convert_ctx);
    clock_t end=clock();

    //cout<<"@@@@@@@@@Decode:"<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
}



