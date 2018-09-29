//
// Created by ken on 2018/8/29.
//

#include "flv_tag_body_creator.h"

shared_ptr<FLVBuffer> FLVTagBodyCreator::createScriptBody()
{
    shared_ptr<FLVBuffer> tagBody = shared_ptr<FLVBuffer>(new FLVBuffer(4096));

    uint8_t *buf   = tagBody->buffer;

    //AMF 1
    buf = ui08_to_bytes(buf, AMF_DATA_TYPE_STRING);
    buf = amf_string_to_bytes(buf, "onMetaData");

    //AMF2
    buf = ui08_to_bytes(buf, AMF_DATA_TYPE_MIXEDARRAY);
    buf = ui32_to_bytes(buf, AMF_VIDEO_ECMA_ARRAY_LENGTH);

    //-----duration
    buf = amf_string_to_bytes(buf, "duration");
    buf = amf_double_to_bytes(buf, 0); // written at end of encoding

    //-----width
    buf = amf_string_to_bytes(buf, "width");
    buf = amf_double_to_bytes(buf, 320);

    //-----height
    buf = amf_string_to_bytes(buf, "height");
    buf = amf_double_to_bytes(buf, 240);

    //-----videodatarate
    buf = amf_string_to_bytes(buf, "videodatarate");
    buf = amf_double_to_bytes(buf, 0); // written at end of encoding

    //-----framerate
    buf = amf_string_to_bytes(buf, "framerate");
    buf = amf_double_to_bytes(buf, 24); // written at end of encoding

    //-----videocodecid
    buf = amf_string_to_bytes(buf, "videocodecid");
    buf = amf_double_to_bytes(buf, 7);

    //-----filesize
    buf = amf_string_to_bytes(buf, "filesize");
    buf = amf_double_to_bytes(buf, 0); // written at end of encoding

    //-----END_OF_OBJECT
    buf = amf_string_to_bytes(buf, "");
    buf = ui08_to_bytes(buf, AMF_END_OF_OBJECT);

    tagBody->size = (int)(buf - tagBody->buffer);

    return tagBody;
}

shared_ptr<FLVBuffer>
FLVTagBodyCreator::createVideoInfoTagBody(uint8_t *sps, uint32_t spsLen, uint8_t *pps, uint32_t ppsLen)
{
    shared_ptr<FLVBuffer> tagBody = shared_ptr<FLVBuffer>(new FLVBuffer(spsLen + ppsLen + 64));

    spsLen -= H264_STARTCODEPREFIX_LENGTH;
    sps += H264_STARTCODEPREFIX_LENGTH;
    ppsLen -= H264_STARTCODEPREFIX_LENGTH;
    pps += H264_STARTCODEPREFIX_LENGTH;

    uint8_t *buf = tagBody->buffer;
    uint8_t flag = FLV_FRAME_KEY;
    buf = ui08_to_bytes(buf, flag);

    buf = ui08_to_bytes(buf, 0); // AVCPacketType: 0x00 - AVC sequence header
    buf = ui24_to_bytes(buf, 0); // composition time

    // generate AVCC with sps and pps, AVCDecoderConfigurationRecord
    buf = ui08_to_bytes(buf, 1); // configurationVersion
    buf = ui08_to_bytes(buf, sps[1]); // AVCProfileIndication
    buf = ui08_to_bytes(buf, sps[2]); // profile_compatibility
    buf = ui08_to_bytes(buf, sps[3]); // AVCLevelIndication
    // 6 bits reserved (111111) + 2 bits nal size length - 1
    // (Reserved << 2) | Nal_Size_length = (0x3F << 2) | 0x03 = 0xFF
    buf = ui08_to_bytes(buf, 0xff);
    // 3 bits reserved (111) + 5 bits number of sps (00001)
    // (Reserved << 5) | Number_of_SPS = (0x07 << 5) | 0x01 = 0xe1
    buf = ui08_to_bytes(buf, 0xe1);

    // sps
    buf = ui16_to_bytes(buf, (uint16_t)spsLen);
    memcpy(buf, sps, spsLen);
    buf += spsLen;

    // pps
    buf = ui08_to_bytes(buf, 1); // number of pps
    buf = ui16_to_bytes(buf, (uint16_t)ppsLen);
    memcpy(buf, pps, ppsLen);
    buf += ppsLen;

    tagBody->size = (int)(buf - tagBody->buffer);

    return tagBody;
}



shared_ptr<FLVBuffer> FLVTagBodyCreator::createVideoTagBody(uint8_t *frame, uint32_t frameLen, uint32_t pts)
{

    shared_ptr<FLVBuffer> tagBody = shared_ptr<FLVBuffer>(new FLVBuffer(frameLen + 64));

    int startCodeLength = 4;

    if(frame[2] == 0x00)
    {
        startCodeLength = 4;
    }else if(frame[2] == 0x01)
    {
        startCodeLength = 3;
    }

    uint8_t *buf = tagBody->buffer;

    int i_type = frame[startCodeLength] & 0x1f;

    if (H264_IDR == i_type || i_type == H264_SEI)
    {
        buf = ui08_to_bytes(buf, FLV_FRAME_KEY);
    }
    else
    {
        buf = ui08_to_bytes(buf, FLV_FRAME_INTER);
    }

    buf = ui08_to_bytes(buf, 1); //AVC NALU
    buf = ui24_to_bytes(buf, 40); // COMPOSE TIME
    buf = ui32_to_bytes(buf, frameLen - startCodeLength);

    memcpy(buf, frame + startCodeLength, frameLen - startCodeLength);

    tagBody->size = (int)(buf - tagBody->buffer) + frameLen - startCodeLength;

    return tagBody;
}

shared_ptr<FLVBuffer> FLVTagBodyCreator::createAudioTagBody(uint8_t audioInfo,uint8_t *frame, uint32_t frameLen, uint32_t pts)
{
    shared_ptr<FLVBuffer> tagBody = shared_ptr<FLVBuffer>(new FLVBuffer(frameLen + 8));

    uint8_t *buf = tagBody->buffer;

    buf = ui08_to_bytes(buf, 0xAF);
    buf = ui08_to_bytes(buf, 1); // aac packetType

    memcpy(buf, frame, frameLen);

    tagBody->size = (int)(buf - tagBody->buffer) + frameLen;

    return tagBody;
}


uint8_t FLVTagBodyCreator::createFLVTagAudioInfo(int audioCodec, int sampleRate, int channels, int sampleSize)
{
    uint8_t soundRate = 0;
    if (sampleRate <= 8 * 1000)
    {
        soundRate = FLV_SAMPLERATE_SPECIAL;
    }
    else if (sampleRate > 8 * 1000 && sampleRate <= 20 * 1000)
    {
        soundRate = FLV_SAMPLERATE_11025HZ;
    }
    else if (sampleRate > 20 * 1000 && sampleRate <= 30 * 1000)
    {
        soundRate = FLV_SAMPLERATE_22050HZ;
    }
    else if (sampleRate > 30 * 1000)
    {
        soundRate = FLV_SAMPLERATE_44100HZ;
    }

    uint8_t soundType = (1 == channels) ? 0 : 1;
    uint8_t soundBitsPerSample;
    switch (sampleSize)
    {
        case 8:
            soundBitsPerSample = FLV_SAMPLESSIZE_8BIT;
            break;
        case 16:
            soundBitsPerSample = FLV_SAMPLESSIZE_16BIT;
            break;
        case 24:
            soundBitsPerSample = FLV_SAMPLESSIZE_24BIT;
            break;
        default:
            soundBitsPerSample = FLV_SAMPLESSIZE_16BIT;
            break;
    }

    uint8_t audioInfo = (audioCodec << FLV_AUDIO_CODECID_OFFSET) | (soundRate << FLV_AUDIO_SAMPLERATE_OFFSET) \
			| (soundBitsPerSample << FLV_AUDIO_SAMPLESSIZE_OFFSET) | soundType;

    return audioInfo;
}
