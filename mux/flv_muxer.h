//
// Created by ken on 2018/8/28.
//

#ifndef ATHENAAPP_FLV_MUXER_H
#define ATHENAAPP_FLV_MUXER_H

#include <iostream>
#include <stdio.h>

#include "flv_export.h"


struct MUXER_API FLVMediaInfo
{
    int width;

    int height;

    int fps;

    int sampleRate;

    int channels;

    int sampleSize;

    bool hasVideo;

    bool hasAudio;
};

class MUXER_API FLVMuxer
{
public:
    FLVMuxer();

    virtual ~FLVMuxer();

    void setMediaInfo(const FLVMediaInfo &mediaInfo);

    bool createFLV(const std::string &fileName);

    void writeVideoInfo(uint8_t *sps,uint32_t spsLen,uint8_t *pps,uint32_t ppsLen,uint32_t pts);

    void writeVideoFrame(uint8_t *buffer,uint32_t length,uint32_t pts);
    
    void writeAudioFrame(uint8_t *buffer,uint32_t length,uint32_t pts);

    void close();

protected:

    void writeHeader();

    void wirteScriptTag();

private:

    std::string         fileName;

    FILE                *pHandle;

    uint8_t             audioInfo;

    FLVMediaInfo        mediaInfo;

};


#endif //ATHENAAPP_FLV_MUXER_H
