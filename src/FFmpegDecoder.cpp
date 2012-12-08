//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB, PolyDimensions
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
//    Original file contributed by PolyDimensions.
//
/// \file FFmpegDecoder.cpp
/// \brief Cpp file for FFmpegDecoder.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FFmpegDecoder.h>
#include <H3D/Scene.h>

#ifdef HAVE_FFMPEG

//#define FFMPEG_DEBUG

#ifndef CODEC_TYPE_VIDEO
#define CODEC_TYPE_VIDEO AVMEDIA_TYPE_VIDEO
#endif

using namespace H3D;

// Define this if you want to render only the video component with no audio
//
#define NO_AUDIO_RENDERER

bool FFmpegDecoder::theTimeIsRight(void)
{

  struct timeval time_of_day;
  gettimeofday(&time_of_day, NULL);
  current_time = (unsigned long)((time_of_day.tv_sec*1000)+(time_of_day.tv_usec*.001)); 

  if ( current_time >= (start_time+((getPosition()*1000)/(video_fps*rate))) ) 
    return 1; 

  return 0;
}


void FFmpegDecoder::cleanupFFmpeg(void)
{
#ifdef FFMPEG_DEBUG 
  Console(4) << "cleanupFFmpeg()" << endl;
#endif

  if (!just_a_test) {

#ifdef FFMPEG_DEBUG 
    Console(4) << "cleanupFFmpeg() - full" << endl;
#endif

    have_new_frame = false;
    status = STOPPED;

    av_free_packet(&packet);
    av_free(temp_buffer);
    av_free(data);

    // Free codec 
    av_free(pCodec);
#ifdef WITH_AUDIO
    av_free(aCodec);
#endif // WITH_AUDIO

    // Free the RGB image
    av_free(pFrameRGB);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codec
    avcodec_close(aCodecCtx);
    avcodec_close(pCodecCtx);

    // Close the video file
    av_close_input_file(pFormatCtx);

    duration = 0;
  }

}


FFmpegDecoder::FFmpegDecoder( )
{
  av_register_all();
}


FFmpegDecoder::~FFmpegDecoder() {
  cleanupFFmpeg();
}


// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FFmpegDecoder::database( "FFmpegDecoder",
                                           &(newInstance<FFmpegDecoder>),
                                           typeid( FFmpegDecoder ) );


H3DVideoClipDecoderNode::DecoderRegistration
FFmpegDecoder::reader_registration(
                            "FFmpegDecoder",
                            &(newImageLoaderNode< FFmpegDecoder >),
                            &FFmpegDecoder::supportsFileType
                            );


bool FFmpegDecoder::testClip( const string &url ) {

  just_a_test=1; 

  if (av_open_input_file(&pFormatCtx, url.c_str(), NULL, 0, NULL) != 0 )
  return 0;

  if(av_find_stream_info(pFormatCtx)<0)
  return 0;
 
  dump_format(pFormatCtx, 0, url.c_str(), 0);
  av_close_input_file(pFormatCtx);
  return 1;
}


bool FFmpegDecoder::loadClip( const string &url ) {
  status = STOPPED;

  if(av_open_input_file(&pFormatCtx, url.c_str(), NULL, 0, NULL)!=0)
  return -1;

  if(av_find_stream_info(pFormatCtx)<0)
  return -1;

  // Dump information about file onto standard error

#ifdef FFMPEG_DEBUG 
  dump_format(pFormatCtx, 0, url.c_str(), 0);
#endif


  // Find the first video stream
  videoStream=-1;
  audioStream=-1;
  for(int i=0; i < pFormatCtx->nb_streams; i++) {
    if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO
       &&
         videoStream < 0) {
      videoStream=i;
    }
#ifdef WITH_AUDIO
    if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO &&
       audioStream < 0) {
      audioStream=i;
    }
#endif // WITH_AUDIO
  }
  if(videoStream==-1)
    return -1; // Didn't find a video stream
#ifdef WITH_AUDIO
  if(audioStream==-1)
    return -1;  // Didn't find an audio stream
#endif // WITH_AUDIO

  // Get a pointer to the codec context for the video stream
  pCodecCtx=pFormatCtx->streams[videoStream]->codec;

  // Get a pointer to the codec context for the audio stream
#ifdef WITH_AUDIO
  aCodecCtx=pFormatCtx->streams[audioStream]->codec;
#endif // WITH_AUDIO

  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  // Open codec
  if(avcodec_open(pCodecCtx, pCodec)<0)
    return -1; // Could not open codec

#ifdef WITH_AUDIO
  aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
  if(!aCodec) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1;
  }
  avcodec_open(aCodecCtx, aCodec);
#endif // WITH_AUDIO

  // Allocate video frame
  pFrame=avcodec_alloc_frame();

  // Allocate an AVFrame structure
  pFrameRGB=avcodec_alloc_frame();
  if(pFrameRGB==NULL)
    return -1;

  uint8_t *temp_buffer;

  // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                            pCodecCtx->height);
  temp_buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)pFrameRGB, temp_buffer, PIX_FMT_RGB24,
                pCodecCtx->width, pCodecCtx->height);

  duration = pFormatCtx->duration * 10e-7;

  video_fps = pCodecCtx->time_base.den/pCodecCtx->time_base.num;
  
  if (video_fps < 15 || video_fps > 30) {
    video_fps = 25;
  }
  Console(4) << "playing@" << video_fps << "fps" << endl;

  frame_height = pCodecCtx->height;
  frame_width  = pCodecCtx->width;
  frame_bpp    = 24;

  data_size    = frame_height * frame_width * frame_bpp;
  setRate( 1 );
  current_frame = 0;

  imgConvertCtx = sws_getContext( pCodecCtx->width, 
                                  pCodecCtx->height, 
                                  pCodecCtx->pix_fmt, 
                                  pCodecCtx->width, 
                                  pCodecCtx->height, 
                                  PIX_FMT_RGB24, 
                                  SWS_FAST_BILINEAR,
                                  NULL, NULL, NULL);

#ifdef FFMPEG_DEBUG 
  Console(4) << "duration=" << duration << "s" << endl;
#endif

  struct timeval time_of_day;
  gettimeofday(&time_of_day, NULL);
  start_time = (unsigned long)((time_of_day.tv_sec*1000)+(time_of_day.tv_usec*.001)); 

  status = PLAYING;
  return 1; 
}


void FFmpegDecoder::getNewFrame( unsigned char *buffer ) {

  if (!theTimeIsRight()) return; 

  if (av_read_frame(pFormatCtx, &packet)>=0) {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream) {
        // Decode video frame
      //#if LIBAVFORMAT_BUILD > 4628 
      //avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,
      //packet.data, packet.size);

      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
      // Did we get a video frame?
      if(frameFinished) {
        // Convert the image from its native format to RGB
        sws_scale(imgConvertCtx,
                  pFrame->data, pFrame->linesize,
                  0, pCodecCtx->height, 
                  pFrameRGB->data, pFrameRGB->linesize);
        
          
        // Render the image to the texture 
        if (pCodecCtx->height > 0 && pCodecCtx->width > 0) {
          for(int y=pCodecCtx->height-1; y>-1; y--) {
            memcpy( buffer, pFrameRGB->data[0]+(y*pFrameRGB->linesize[0]), pCodecCtx->width*3 );
            buffer+=pCodecCtx->width*3;
          }
          current_frame++;
        }
        
        //have_new_frame = false;
        
#ifdef FFMPEG_DEBUG 
        //Console(4) << "Pos=" << getPosition() << endl;
#endif
        
      }
    }
  }  
  else if (looping) {
    stopPlaying();
    startPlaying();
  }
  else {
    stopPlaying();
  }
}


void FFmpegDecoder::startPlaying() {
#ifdef FFMPEG_DEBUG 
  Console(4) << "FFmpegDecoder::startPlaying()" << endl;
#endif

  have_new_frame = true;

    if (status != PLAYING) {
    struct timeval time_of_day;
    gettimeofday(&time_of_day, NULL);
    start_time = (unsigned long)((time_of_day.tv_sec*1000)+(time_of_day.tv_usec*.001)); 

    status = PLAYING;
  }

}


void FFmpegDecoder::stopPlaying() {
#ifdef FFMPEG_DEBUG 
  Console(4) << "FFmpegDecoder::stopPlaying()" << endl;
#endif

  if (status != STOPPED) 
    status = STOPPED;

  av_seek_frame(pFormatCtx, videoStream, 0, 0);
#ifdef WITH_AUDIO
  av_seek_frame(pFormatCtx, audioStream, 0, 0);
#endif // WITH_AUDIO
  current_frame = 0;
  have_new_frame = false;
}


void FFmpegDecoder::pausePlaying() {
#ifdef FFMPEG_DEBUG 
  Console(4) << "FFmpegDecoder::pausePlaying()" << endl;
#endif

  have_new_frame = false;

  if (status == PLAYING) 
    status = PAUSED;
}


bool FFmpegDecoder::supportsFileType( const string &url ) {
#ifdef FFMPEG_DEBUG 
  Console(4) << "FFmpegDecoder::supportsFileType( " << url << " )" << endl;
#endif

  AutoRef< FFmpegDecoder > dec( new FFmpegDecoder() );
  return dec->testClip( url );
}


/// Get the current position in the clip (in seconds from start position)
H3DTime FFmpegDecoder::getPosition() {
#ifdef FFMPEG_DEBUG 
  Console(4) << "FFmpegDecoder::getPosition()" << endl;
#endif

  return current_frame;
}


/// Set the current position in the clip(in seconds from start position)
void FFmpegDecoder::setPosition( H3DTime pos ) {
#ifdef FFMPEG_DEBUG 
  Console(4) << "FFmpegDecoder::setPosition( " << pos << " )" << endl;
#endif
  av_seek_frame(pFormatCtx, videoStream, pos, 0);
#ifdef WITH_AUDIO
  av_seek_frame(pFormatCtx, audioStream, pos, 0);
#endif // WITH_AUDIO
  current_frame=0;
}

#endif // HAVE_FFMPEG
