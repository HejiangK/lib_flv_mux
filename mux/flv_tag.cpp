//
// Created by ken on 2018/8/28.
//

#include "flv_tag.h"

FLVTag::FLVTag(uint32_t bufferSize):bufferSize(bufferSize),useByteSize(0),bodySize(0)
{
    buffer  = (uint8_t *)malloc(bufferSize);
    header  = buffer;
    body    = buffer + FLV_TAG_HEAD_SIZE;

    memset(buffer,0,bufferSize);
}

FLVTag::~FLVTag()
{
    free(buffer);
}

void FLVTag::setHeader(const shared_ptr<FLVBuffer> &headerData)
{
    memcpy(header, headerData->buffer, headerData->size);
}

void FLVTag::setBody(const shared_ptr<FLVBuffer> &bodyData)
{
    memcpy(body,bodyData->buffer,bodyData->size);

    bodySize = bodyData->size;

    preTagSize = body + bodySize;

    ui32_to_bytes(preTagSize, FLV_TAG_HEAD_SIZE + bodySize);

    useByteSize = FLV_TAG_HEAD_SIZE + bodySize + FLV_TAG_PRE_SIZE;
}

uint8_t *FLVTag::getBuffer() const
{
    return buffer;
}

uint32_t FLVTag::getUseByteSize() const
{
    return useByteSize;
}
