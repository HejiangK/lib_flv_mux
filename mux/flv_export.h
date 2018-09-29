
// Created by Administrator on 2018/9/13.


#ifndef ATHENAAPP_FLV_EXPORT_H
#define ATHENAAPP_FLV_EXPORT_H

#ifdef WIN32
#   ifdef EXPORT_MUXER_SHAERD
#   define MUXER_API __declspec(dllexport)
#   else
#   define MUXER_API  __declspec(dllimport)
#   endif
#else
#   ifdef EXPORT_MEDIA_SHAERD
#   define MUXER_API
#   else
#   define MUXER_API
#   endif
#endif

#endif //ATHENAAPP_FLV_EXPORT_H
