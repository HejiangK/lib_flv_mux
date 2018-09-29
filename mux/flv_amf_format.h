//
// Created by ken on 2018/8/28.
//

#ifndef ATHENAAPP_FLV_AMF_FORMAT_H
#define ATHENAAPP_FLV_AMF_FORMAT_H

#include <iostream>
#include <memory>

using namespace std;

#define FLV_HEAD_SIZE 9

typedef enum
{
    AMF_DATA_TYPE_NUMBER = 0x00,
    AMF_DATA_TYPE_BOOL = 0x01,
    AMF_DATA_TYPE_STRING = 0x02,
    AMF_DATA_TYPE_OBJECT = 0x03,
    AMF_DATA_TYPE_NULL = 0x05,
    AMF_DATA_TYPE_UNDEFINED = 0x06,
    AMF_DATA_TYPE_REFERENCE = 0x07,
    AMF_DATA_TYPE_MIXEDARRAY = 0x08,
    AMF_DATA_TYPE_OBJECT_END = 0x09,
    AMF_DATA_TYPE_ARRAY = 0x0a,
    AMF_DATA_TYPE_DATE = 0x0b,
    AMF_DATA_TYPE_LONG_STRING = 0x0c,
    AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
} _AMFDataType;

#define AMF_AUDIO_ECMA_ARRAY_LENGTH		8
#define AMF_VIDEO_ECMA_ARRAY_LENGTH		7
#define AMF_END_OF_OBJECT				0x09

struct FlVHeader {
    unsigned char tag_f;
    unsigned char tag_l;
    unsigned char tag_v;
    unsigned char version;
    unsigned char streamInfo;
    unsigned char DataOffset[4];

    FlVHeader()
    {
        tag_f       = 'F';
        tag_l       = 'L';
        tag_v       = 'V';
        version     = 0x01;
        streamInfo  = 0x05;

        memset(DataOffset, 0, 4);
        DataOffset[3] = 0x09;
    }
};


struct FLVBuffer
{
    uint8_t *buffer;

    uint32_t size;

    FLVBuffer()
    {
        this->size = 0;

        buffer = nullptr;
    }

    FLVBuffer(uint32_t size)
    {
        this->size = size;

        buffer = (uint8_t*)malloc(this->size);

        memset(buffer,0,size);
    }

    ~FLVBuffer()
    {
        free(buffer);
    }
};


static void printBUF(uint8_t *buf,uint32_t size)
{
    for(int i = 0; i < size; i ++)
    {
        printf("%2x ",buf[i]);
    }

    printf("\n");
}

static uint8_t *ui08_to_bytes(uint8_t *buf, uint8_t val) {
    buf[0] = (val)& 0xff;

    return buf + 1;
}

static uint8_t *ui16_to_bytes(uint8_t *buf, uint16_t val) {
    buf[0] = (val >> 8) & 0xff;
    buf[1] = (val) & 0xff;

    return buf + 2;
}

static uint8_t *ui24_to_bytes(uint8_t *buf, uint32_t val) {
    buf[0] = (val >> 16) & 0xff;
    buf[1] = (val >> 8) & 0xff;
    buf[2] = (val) & 0xff;

    return buf + 3;
}

static uint8_t *ui32_to_bytes(uint8_t *buf, uint32_t val) {
    buf[0] = (val >> 24) & 0xff;
    buf[1] = (val >> 16) & 0xff;
    buf[2] = (val >> 8) & 0xff;
    buf[3] = (val) & 0xff;

    return buf + 4;
}

static uint8_t *ui64_to_bytes(uint8_t *buf, uint64_t val) {
    buf[0] = (val >> 56) & 0xff;
    buf[1] = (val >> 48) & 0xff;
    buf[2] = (val >> 40) & 0xff;
    buf[3] = (val >> 32) & 0xff;
    buf[4] = (val >> 24) & 0xff;
    buf[5] = (val >> 16) & 0xff;
    buf[6] = (val >> 8) & 0xff;
    buf[7] = (val) & 0xff;

    return buf + 8;
}

static uint8_t *double_to_bytes(uint8_t *buf, double val) {
    union {
        uint8_t dc[8];
        double dd;
    } d;

    uint8_t b[8];

    d.dd = val;

    b[0] = d.dc[7];
    b[1] = d.dc[6];
    b[2] = d.dc[5];
    b[3] = d.dc[4];
    b[4] = d.dc[3];
    b[5] = d.dc[2];
    b[6] = d.dc[1];
    b[7] = d.dc[0];

    memcpy(buf, b, 8);

    return buf + 8;
}

static uint8_t bytes_to_ui32(const uint8_t *buf) {
    return (((buf[0]) << 24) & 0xff000000)
           | (((buf[1]) << 16) & 0xff0000)
           | (((buf[2]) << 8) & 0xff00)
           | (((buf[3])) & 0xff);
}


static uint8_t *amf_string_to_bytes(uint8_t *buf, const char *str) {
    uint8_t *pbuf = buf;
    size_t len = strlen(str);
    pbuf = ui16_to_bytes(pbuf, len);
    memcpy(pbuf, str, len);
    pbuf += len;

    return pbuf;
}

static uint8_t *amf_double_to_bytes(uint8_t *buf, double d) {
    uint8_t *pbuf = buf;
    pbuf = ui08_to_bytes(pbuf, AMF_DATA_TYPE_NUMBER);
    pbuf = double_to_bytes(pbuf, d);

    return pbuf;
}

static uint8_t *amf_bool_to_bytes(uint8_t *buf, int b) {
    uint8_t *pbuf = buf;
    pbuf = ui08_to_bytes(pbuf, AMF_DATA_TYPE_BOOL);
    pbuf = ui08_to_bytes(pbuf, !!b);

    return pbuf;
}

static uint64_t dbl2int(double value)
{
    union
    {
        double f;
        uint64_t i;
    }_val;

    _val.f = value;

    return _val.i;
}


#endif //ATHENAAPP_FLV_AMF_FORMAT_H
