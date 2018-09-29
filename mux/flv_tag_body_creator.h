//
// Created by ken on 2018/8/29.
//

#ifndef ATHENAAPP_FLV_TAG_BODY_CREATOR_H
#define ATHENAAPP_FLV_TAG_BODY_CREATOR_H

#include "flv_amf_format.h"
#include "flv_export.h"


enum MUXER_API _FLV_H264_FRAME_TYPE
{
    H264_NON_IDR	= 1,
    H264_IDR		= 5,
    H264_SEI		= 6,
    H264_SPS		= 7,
    H264_PPS		= 8,
    H264_ELSE,
};

#define FLV_VIDEO_FRAMETYPE_OFFSET	4
#define H264_STARTCODEPREFIX_LENGTH 4

#define FLV_AUDIO_SAMPLESSIZE_OFFSET	1
#define FLV_AUDIO_SAMPLERATE_OFFSET		2
#define FLV_AUDIO_CODECID_OFFSET		4

enum MUXER_API _FLV_VIDEO_FRAME_TYPE
{
    FLV_FRAME_KEY = 1 << FLV_VIDEO_FRAMETYPE_OFFSET | 7,		//frametype "1 == keyframe", codecid "7 == AVC"
    FLV_FRAME_INTER = 2 << FLV_VIDEO_FRAMETYPE_OFFSET | 7,
};


enum MUXER_API _FLV_SAMPLE_SIZE
{
    FLV_SAMPLESSIZE_8BIT = 0,
    FLV_SAMPLESSIZE_16BIT = 1,
    FLV_SAMPLESSIZE_24BIT = 2,
};

enum MUXER_API _FLV_SAMPLE_RATE
{
    FLV_SAMPLERATE_SPECIAL = 0, /**< signifies 5512Hz and 8000Hz in the case of NELLYMOSER */
    FLV_SAMPLERATE_11025HZ = 1,
    FLV_SAMPLERATE_22050HZ = 2,
    FLV_SAMPLERATE_44100HZ = 3,
};

enum MUXER_API _FLV_AUDIO_CODEC
{
    FLV_CODECID_MP3 = 2,
    FLV_CODECID_AAC = 10,
    FLV_CODECID_SPEEX = 11,
} ;

class MUXER_API FLVTagBodyCreator
{
public:

    static shared_ptr<FLVBuffer> createScriptBody();

    static shared_ptr<FLVBuffer> createVideoInfoTagBody(uint8_t *sps,uint32_t spsLen,uint8_t *pps,uint32_t ppsLen);

    static shared_ptr<FLVBuffer> createVideoTagBody(uint8_t *frame,uint32_t frameLen,uint32_t pts);

    static shared_ptr<FLVBuffer> createAudioTagBody(uint8_t audioInfo,uint8_t *frame,uint32_t frameLen,uint32_t pts);

    static uint8_t createFLVTagAudioInfo(int audioCodec, int sampleRate, int channels, int sampleSize);
};


#endif //ATHENAAPP_FLV_TAG_BODY_CREATOR_H
