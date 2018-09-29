//
// Created by ken on 2018/8/28.
//

#ifndef ATHENAAPP_FLV_TAG_H
#define ATHENAAPP_FLV_TAG_H

#include <iostream>

#include "flv_amf_format.h"

#define FLV_TAG_PRE_SIZE  4
#define FLV_TAG_HEAD_SIZE 11
#define FLV_TAG_DEFAULT_SIZE 1024 * 8

class FLVTag
{
public:
    FLVTag(uint32_t bufferSize);

    virtual ~FLVTag();

    void setHeader(const shared_ptr<FLVBuffer> &headerData);

    void setBody(const shared_ptr<FLVBuffer> &bodyData);

    uint8_t *getBuffer() const;

    uint32_t getUseByteSize() const;

private:

    uint8_t     *buffer;

    uint32_t    bufferSize;

    uint8_t     *header;

    uint8_t     *body;

    uint32_t    bodySize;

    uint8_t     *preTagSize;

    uint32_t    useByteSize;

};


#endif //ATHENAAPP_FLV_TAG_H
