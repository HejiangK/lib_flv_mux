//
// Created by ken on 2018/8/28.
//

#include "flv_muxer.h"
#include "flv_tag_creator.h"

#ifndef WIN32

#include <dirent.h>
#include <unistd.h>

#endif

#include <sys/stat.h>


FLVMuxer::FLVMuxer()
{

}

FLVMuxer::~FLVMuxer()
{

}

bool FLVMuxer::createFLV(const std::string &fileName)
{
    this->fileName = fileName;

    pHandle = fopen(fileName.data(),"wb");

    if(pHandle == nullptr)
        return false;

    audioInfo = FLVTagBodyCreator::createFLVTagAudioInfo(FLV_CODECID_AAC,
                                                         mediaInfo.sampleRate,mediaInfo.channels,mediaInfo.sampleSize);

    writeHeader();

//    wirteScriptTag();

    return true;
}

void FLVMuxer::writeHeader()
{
    FlVHeader flvHeader;

    flvHeader.streamInfo = (mediaInfo.hasAudio << 2) | mediaInfo.hasVideo;

    uint8_t *buffer = (uint8_t *)malloc(FLV_HEAD_SIZE + FLV_TAG_PRE_SIZE);

    memcpy(buffer, &flvHeader, sizeof(FlVHeader)); //video heaer

    ui32_to_bytes(buffer + sizeof(FlVHeader), 0); // pre tag size 0x00 0x00 0x00

    fwrite(buffer,FLV_HEAD_SIZE + FLV_TAG_PRE_SIZE,1,pHandle);

    free(buffer);

}

void FLVMuxer::wirteScriptTag()
{
    shared_ptr<FLVTag> tag = FLVTagCreator::createScriptTag();

    fwrite(tag->getBuffer(),tag->getUseByteSize(),1,pHandle);
}

void FLVMuxer::writeVideoInfo(uint8_t *sps, uint32_t spsLen, uint8_t *pps, uint32_t ppsLen,uint32_t pts)
{

    shared_ptr<FLVTag> tag = FLVTagCreator::createVideoInfoTag(sps,spsLen,pps,ppsLen,pts);

    fwrite(tag->getBuffer(),tag->getUseByteSize(),1,pHandle);
}

void FLVMuxer::writeVideoFrame(uint8_t *buffer, uint32_t length, uint32_t pts)
{
    shared_ptr<FLVTag> tag = FLVTagCreator::createVideoTag(buffer,length,pts);

    fwrite(tag->getBuffer(),tag->getUseByteSize(),1,pHandle);
}

void FLVMuxer::writeAudioFrame(uint8_t *buffer, uint32_t length, uint32_t pts)
{
    shared_ptr<FLVTag> tag = FLVTagCreator::createAudioTag(audioInfo,buffer,length,pts);

    fwrite(tag->getBuffer(),tag->getUseByteSize(),1,pHandle);
}

void FLVMuxer::close()
{
    fclose(pHandle);
}

void FLVMuxer::setMediaInfo(const FLVMediaInfo &mediaInfo)
{
    FLVMuxer::mediaInfo = mediaInfo;
}

