//
// Created by ken on 2018/8/28.
//

#ifndef ATHENAAPP_FLV_TAG__CREATOR_H
#define ATHENAAPP_FLV_TAG__CREATOR_H

#include "flv_amf_format.h"
#include "flv_tag.h"
#include "flv_tag_body_creator.h"

enum FLVTagType
{
    FLV_TAG_VIDEO,

    FLV_TAG_AUDIO,

    FLV_TAG_SCRIPT
};

class FLVTagCreator
{
public:

    static shared_ptr<FLVTag> createScriptTag();

    static shared_ptr<FLVTag> createVideoInfoTag(uint8_t *sps,uint32_t spsLen,uint8_t *pps,uint32_t ppsLen,uint32_t pts);

    static shared_ptr<FLVTag> createVideoTag(uint8_t *frame,uint32_t frameLen,uint32_t pts);

    static shared_ptr<FLVTag> createAudioTag(uint8_t audioInfo,uint8_t *frame,uint32_t frameLen,uint32_t pts);

};


#endif //ATHENAAPP_FLV_TAG_CREATOR_H
