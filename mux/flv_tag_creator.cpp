//
// Created by ken on 2018/8/28.
//

#include "flv_tag_creator.h"
#include "flv_tag.h"

struct FLVTagDataHeader {
    unsigned char tagtype;
    unsigned char size[3];
    unsigned char timestamp[3];
    unsigned char extent_timestamp;
    unsigned char stream_id[3];

    FLVTagDataHeader()
    {
        memset(this, 0, sizeof(FLVTagDataHeader));
    }
};


shared_ptr<FLVBuffer> createTagHeader(FLVTagType type, uint32_t pts, uint32_t size)
{
    shared_ptr<FLVBuffer> tagHeader = shared_ptr<FLVBuffer>(new FLVBuffer(FLV_TAG_HEAD_SIZE));

    uint8_t *buffer = tagHeader->buffer;

    FLVTagDataHeader tagDataHeader;

    if(type == FLV_TAG_SCRIPT)
        ui08_to_bytes(&tagDataHeader.tagtype, 0x12);
    else if(type == FLV_TAG_AUDIO)
        ui08_to_bytes(&tagDataHeader.tagtype, 0x08);
    else if(type == FLV_TAG_VIDEO)
        ui08_to_bytes(&tagDataHeader.tagtype, 0x09);

    ui24_to_bytes(tagDataHeader.size, size);					//size
    ui24_to_bytes(tagDataHeader.timestamp, pts);					//timestamp
    ui08_to_bytes(&tagDataHeader.extent_timestamp, pts >> 24);				//timestamp_ex

    memcpy(buffer, &tagDataHeader, sizeof(FLVTagDataHeader));

//    //第一位标识类型
//    if(type == FLV_TAG_SCRIPT)
//        buffer = ui08_to_bytes(buffer,0x12);
//    else if(type == FLV_TAG_AUDIO)
//        buffer = ui08_to_bytes(buffer,0x08);
//    else if(type == FLV_TAG_VIDEO)
//        buffer = ui08_to_bytes(buffer,0x09);
//
//    // 2 - 4 3位标识size
//    buffer = ui32_to_bytes(buffer,size);
//
//    buffer = ui24_to_bytes(buffer, pts);
//    buffer = ui08_to_bytes(buffer, pts >> 24);

    return tagHeader;
}


shared_ptr<FLVTag> FLVTagCreator::createScriptTag()
{
    shared_ptr<FLVTag>      tag;
    shared_ptr<FLVBuffer>   body;
    shared_ptr<FLVBuffer>   header;

    body    = FLVTagBodyCreator::createScriptBody();
    header  = createTagHeader(FLV_TAG_SCRIPT,0,body->size);
    tag     = shared_ptr<FLVTag>(new FLVTag(header->size + body->size + 64));


    tag->setHeader(header);
    tag->setBody(body);

    return tag;
}

shared_ptr<FLVTag> FLVTagCreator::createVideoInfoTag(uint8_t *sps, uint32_t spsLen, uint8_t *pps, uint32_t ppsLen,uint32_t pts)
{
    shared_ptr<FLVTag>      tag;
    shared_ptr<FLVBuffer>   body;
    shared_ptr<FLVBuffer>   header;

    body    = FLVTagBodyCreator::createVideoInfoTagBody(sps,spsLen,pps,ppsLen);
    header  = createTagHeader(FLV_TAG_VIDEO,pts,body->size);
    tag     = shared_ptr<FLVTag>(new FLVTag(header->size + body->size + 64));

    tag->setHeader(header);
    tag->setBody(body);

    return tag;
}


shared_ptr<FLVTag> FLVTagCreator::createVideoTag(uint8_t *frame, uint32_t frameLen, uint32_t pts)
{
    shared_ptr<FLVTag>      tag;
    shared_ptr<FLVBuffer>   body;
    shared_ptr<FLVBuffer>   header;

    body    = FLVTagBodyCreator::createVideoTagBody(frame,frameLen,pts);
    header  = createTagHeader(FLV_TAG_VIDEO,pts,body->size);
    tag     = shared_ptr<FLVTag>(new FLVTag(header->size + body->size + 64));

    tag->setHeader(header);
    tag->setBody(body);

    return tag;
}

shared_ptr<FLVTag> FLVTagCreator::createAudioTag(uint8_t audioInfo,uint8_t *frame, uint32_t frameLen, uint32_t pts)
{
    shared_ptr<FLVTag>      tag;
    shared_ptr<FLVBuffer>   body;
    shared_ptr<FLVBuffer>   header;

    body    = FLVTagBodyCreator::createAudioTagBody(audioInfo,frame,frameLen,pts);
    header  = createTagHeader(FLV_TAG_AUDIO,pts,body->size);
    tag     = shared_ptr<FLVTag>(new FLVTag(header->size + body->size + 64));

    tag->setHeader(header);
    tag->setBody(body);

    return tag;
}

