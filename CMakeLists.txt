cmake_minimum_required(VERSION 3.5)

project(lib_muxer)

add_definitions(-DEXPORT_MUXER_SHAERD)

set(
        LIB_MUXER_SRCS

        mux/flv_export.h
        mux/flv_amf_format.h
        mux/flv_muxer.h
        mux/flv_muxer.cpp
        mux/flv_tag.h
        mux/flv_tag.cpp
        mux/flv_tag_creator.h
        mux/flv_tag_creator.cpp
        mux/flv_tag_body_creator.h
        mux/flv_tag_body_creator.cpp

)

add_library(
        muxer
        SHARED
        ${LIB_MUXER_SRCS}
)
