/**
 * 最简单的基于FFmpeg的AVDevice例子（屏幕录制）
 * Simplest FFmpeg Device (Screen Capture)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了屏幕录制功能。可以录制并播放桌面数据。是基于FFmpeg
 * 的libavdevice类库最简单的例子。通过该例子，可以学习FFmpeg中
 * libavdevice类库的使用方法。
 * 本程序在Windows下可以使用2种方式录制屏幕：
 *  1.gdigrab: Win32下的基于GDI的屏幕录制设备。
 *             抓取桌面的时候，输入URL为“desktop”。
 *  2.dshow: 使用Directshow。注意需要安装额外的软件screen-capture-recorder
 * 在Linux下则可以使用x11grab录制屏幕。
 *
 * This software capture screen of computer. It's the simplest example
 * about usage of FFmpeg's libavdevice Library. 
 * It's suiltable for the beginner of FFmpeg.
 * This software support 2 methods to capture screen in Microsoft Windows:
 *  1.gdigrab: Win32 GDI-based screen capture device.
 *             Input URL in avformat_open_input() is "desktop".
 *  2.dshow: Use Directshow. Need to install screen-capture-recorder.
 * It use x11grab to capture screen in Linux.
 */


#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "SDL.h"
};

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <SDL/SDL.h>
#ifdef __cplusplus
};
#endif
#endif

//Output YUV420P 
#define OUTPUT_YUV420P 0
//'1' Use Dshow 
//'0' Use GDIgrab
#define USE_DSHOW 0

//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

int thread_exit=0;

int sfp_refresh_thread(void *opaque)
{
	while (thread_exit==0) {
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(10);
	}
	return 0;
}

//Show Dshow Device
void show_dshow_device(){
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("dshow");
    printf("========Device Info=============\n");
    avformat_open_input(&pFormatCtx,"video=dummy",iformat,&options);
    printf("================================\n");
}

//Show AVFoundation Device
void show_avfoundation_device(){
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("avfoundation");
    printf("==AVFoundation Device Info===\n");
    avformat_open_input(&pFormatCtx,"",iformat,&options);
    printf("=============================\n");
}


#undef main 
int main(int argc, char* argv[])
{

	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	
	//Open File
	//char filepath[]="src01_480x272_22.h265";
	//avformat_open_input(&pFormatCtx,filepath,NULL,NULL)

	//Register Device
	avdevice_register_all();
	//Windows
#ifdef _WIN32
#if USE_DSHOW
	//Use dshow
	//
	//Need to Install screen-capture-recorder
	//screen-capture-recorder
	//Website: http://sourceforge.net/projects/screencapturer/
	//
	AVInputFormat *ifmt=av_find_input_format("dshow");
	if(avformat_open_input(&pFormatCtx,"video=screen-capture-recorder",ifmt,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#else
	//Use gdigrab
	AVDictionary* options = NULL;
	//Set some options
	//grabbing frame rate
	av_dict_set(&options,"framerate","60",0);
	//The distance from the left edge of the screen or desktop
	av_dict_set(&options,"offset_x","20",0);
	//The distance from the top edge of the screen or desktop
	av_dict_set(&options,"offset_y","40",0);
	//Video frame size. The default is to capture the full screen
	//av_dict_set(&options,"video_size","640x480",0);
	AVInputFormat *ifmt=av_find_input_format("gdigrab");
	if(avformat_open_input(&pFormatCtx,"desktop",ifmt,&options)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}

#endif
#elif defined linux
	//Linux
	AVDictionary* options = NULL;
	//Set some options
	//grabbing frame rate
	//av_dict_set(&options,"framerate","5",0);
	//Make the grabbed area follow the mouse
	//av_dict_set(&options,"follow_mouse","centered",0);
	//Video frame size. The default is to capture the full screen
	//av_dict_set(&options,"video_size","640x480",0);
	AVInputFormat *ifmt=av_find_input_format("x11grab");
	//Grab at position 10,20
	if(avformat_open_input(&pFormatCtx,":0.0+10,20",ifmt,&options)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
#else
    show_avfoundation_device();
    //Mac
    AVInputFormat *ifmt=av_find_input_format("avfoundation");
    //Avfoundation
    //[video]:[audio]
    if(avformat_open_input(&pFormatCtx,"1",ifmt,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
#endif

	if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}
    av_dump_format(pFormatCtx, 0, "recorder", 0);

	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	if(videoindex==-1)
	{
		printf("Didn't find a video stream.\n");
		return -1;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.\n");
		return -1;
	}
	AVFrame	*pFrame,*pFrameYUV;
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	//uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	//avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	//SDL----------------------------
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 
	// int screen_w=640,screen_h=360;
    // int index;
    // SDL_DisplayMode mode;
	// SDL_GetDesktopDisplayMode(index, &mode);
	// //Half of the Desktop's width and height.
	// screen_w = mode.w;
	// screen_h = mode.h;
	// SDL_Surface *screen; 
	// screen = SDL_SetVideoMode(screen_w, screen_h, 0,0);

	// if(!screen) {  
	// 	printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());  
	// 	return -1;
	// }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) { 
        fprintf(stderr, "SDL video init failed: %s\n", SDL_GetError()); 
        return 1; 
    }

    // SDL_Window* window = NULL; 
    // SDL_Surface* screen = NULL;
    // window = SDL_CreateWindow("Sphere Rendering", 
    // SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
    // 640, 360, SDL_WINDOW_SHOWN); 

    // if (window == NULL) { 
    //     fprintf(stderr, "Window could not be created: %s\n", SDL_GetError()); 
    //     return 1; 
    // } 

    SDL_Window *screen;
    screen = SDL_CreateWindow(
            "Sphere Rendering",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            pCodecCtx->width/3,
            pCodecCtx->height/3,
            SDL_WINDOW_SHOWN
        );

    if (!screen) { 
        fprintf(stderr, "Screen surface could not be created: %s\n", SDL_GetError()); 
        SDL_Quit(); 
        return 1; 
    } 

    SDL_Renderer *renderer;
    SDL_Texture *texture;
    struct SwsContext *sws_ctx = NULL;
    Uint8 *yPlane, *uPlane, *vPlane;
    size_t yPlaneSz, uvPlaneSz;
    int uvPitch;
    SDL_Event event;
    AVPacket packet;
    int ret, got_picture;
    AVCodecContext* enc_ctx = NULL;

    renderer = SDL_CreateRenderer(screen, -1, 0);
    if (!renderer) {
        fprintf(stderr, "SDL: could not create renderer - exiting\n");
        exit(1);
    }

    // Allocate a place to put our YUV image on that screen
    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            pCodecCtx->width,
            pCodecCtx->height
        );
    if (!texture) {
        fprintf(stderr, "SDL: could not create texture - exiting\n");
        exit(1);
    }

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
            AV_PIX_FMT_YUV420P,
            SWS_BILINEAR,
            NULL,
            NULL,
            NULL);

    // set up YV12 pixel array (12 bits per pixel)
    yPlaneSz = pCodecCtx->width * pCodecCtx->height;
    uvPlaneSz = pCodecCtx->width * pCodecCtx->height / 4;
    yPlane = (Uint8*)malloc(yPlaneSz);
    uPlane = (Uint8*)malloc(uvPlaneSz);
    vPlane = (Uint8*)malloc(uvPlaneSz);
    if (!yPlane || !uPlane || !vPlane) {
        fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
        exit(1);
    }

    uvPitch = pCodecCtx->width / 2;
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoindex) {
            // Decode video frame
            // avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
            ret = avcodec_send_packet(pCodecCtx, &packet);
            ret = avcodec_receive_frame(pCodecCtx, pFrame);

            // Did we get a video frame?
            if (ret == 0) {
//                 if (NULL == enc_ctx) {
//                     //打开编码器，并且设置 编码信息。
//                     AVCodec* encode = avcodec_find_encoder(AV_CODEC_ID_H264);
//                     enc_ctx = avcodec_alloc_context3(encode);
//                     enc_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
//                     enc_ctx->bit_rate = 400000;
//                     enc_ctx->framerate = pCodecCtx->framerate;
//                     enc_ctx->gop_size = 10;
//                     enc_ctx->max_b_frames = 5;
//                     enc_ctx->profile = FF_PROFILE_H264_MAIN;
//                     /*
//                      * 其实下面这些信息在容器那里也有，也可以一开始直接在容器那里打开编码器
//                      * 我从 AVFrame 里拿这些编码器参数是因为，容器的信息不一样就是最终的信息。
//                      * 因为你解码出来的 AVFrame 可能会经过 filter 滤镜，经过滤镜之后信息就会变化，但是本文没有使用滤镜。
//                      */
//                      //编码器的时间基要取 AVFrame 的时间基，因为 AVFrame 是输入。
//                     enc_ctx->time_base = pFormatCtx->streams[0]->time_base;
//                     enc_ctx->width = pFormatCtx->streams[0]->codecpar->width;
//                     enc_ctx->height = pFormatCtx->streams[0]->codecpar->height;
//                     enc_ctx->sample_aspect_ratio = pFrame->sample_aspect_ratio;
//                     enc_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
//                     enc_ctx->color_range = pFrame->color_range;
//                     enc_ctx->color_primaries = pFrame->color_primaries;
//                     enc_ctx->color_trc = pFrame->color_trc;
//                     enc_ctx->colorspace = pFrame->colorspace;
//                     enc_ctx->chroma_sample_location = pFrame->chroma_location;
// 
//                     /* 注意，这个 field_order 不同的视频的值是不一样的，这里我写死了。
//                      * 因为 本文的视频就是 AV_FIELD_PROGRESSIVE
//                      * 生产环境要对不同的视频做处理的
//                      */
//                     enc_ctx->field_order = AV_FIELD_PROGRESSIVE;
// 
//                     if ((ret = avcodec_open2(enc_ctx, encode, NULL)) < 0) {
//                         printf("open codec faile %d \n", ret);
//                         continue;
//                     }
// 
//                     AVPacket* h264packet_out = av_packet_alloc();
//                     AVFrame* h264toYUVFrame = av_frame_alloc();
//                     int h264got_picture;
//                     //往编码器发送 AVFrame，然后不断读取 AVPacket
//                     ret = avcodec_send_frame(enc_ctx, pFrame);
//                     if (ret < 0) {
//                         printf("avcodec_send_frame fail %d \n", ret);
//                         return ret;
//                     }
// 
//                     ret = avcodec_receive_packet(enc_ctx, h264packet_out);
//                     if (ret == AVERROR(EAGAIN)) {
//                         break;
//                     }
// 
//                     ret = avcodec_send_packet(pCodecCtx, h264packet_out);
//                     ret = avcodec_receive_frame(pCodecCtx, h264toYUVFrame);




                    AVPicture pict;
                    pict.data[0] = yPlane;
                    pict.data[1] = uPlane;
                    pict.data[2] = vPlane;
                    pict.linesize[0] = pCodecCtx->width;
                    pict.linesize[1] = uvPitch;
                    pict.linesize[2] = uvPitch;

                    // Convert the image into YUV format that SDL uses
                    sws_scale(sws_ctx, (uint8_t const* const*)pFrame->data,
                        pFrame->linesize, 0, pCodecCtx->height, pict.data,
                        pict.linesize);

                    SDL_UpdateYUVTexture(
                        texture,
                        NULL,
                        yPlane,
                        pCodecCtx->width,
                        uPlane,
                        uvPitch,
                        vPlane,
                        uvPitch
                    );

                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderPresent(renderer);
                //}

            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            //SDL_DestroyWindow(screen);
            SDL_Quit();
            exit(0);
            break;
        default:
            break;
        }

    }

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

	SDL_Quit();

	//av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}