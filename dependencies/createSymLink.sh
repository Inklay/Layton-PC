#!/bin/bash

ln -srf FFmpeg_lgpl/lib/$1/libavcodec.so.62.32.100 FFmpeg_lgpl/lib/$1/libavcodec.so.62
ln -srf FFmpeg_lgpl/lib/$1/libavcodec.so.62 FFmpeg_lgpl/lib/$1/libavcodec.so

ln -srf FFmpeg_lgpl/lib/$1/libavformat.so.62.15.100 FFmpeg_lgpl/lib/$1/libavformat.so.62
ln -srf FFmpeg_lgpl/lib/$1/libavformat.so.62 FFmpeg_lgpl/lib/$1/libavformat.so

ln -srf FFmpeg_lgpl/lib/$1/libavutil.so.60.31.100 FFmpeg_lgpl/lib/$1/libavutil.so.60
ln -srf FFmpeg_lgpl/lib/$1/libavutil.so.60 FFmpeg_lgpl/lib/$1/libavutil.so

ln -srf FFmpeg_lgpl/lib/$1/libswresample.so.6.4.100 FFmpeg_lgpl/lib/$1/libswresample.so.6
ln -srf FFmpeg_lgpl/lib/$1/libswresample.so.6 FFmpeg_lgpl/lib/$1/libswresample.so

ln -srf FFmpeg_lgpl/lib/$1/libswscale.so.9.7.100 FFmpeg_lgpl/lib/$1/libswscale.so.9
ln -srf FFmpeg_lgpl/lib/$1/libswscale.so.9 FFmpeg_lgpl/lib/$1/libswscale.so

ln -srf SDL3/lib/$1/libSDL3.so.0.4.8 SDL3/lib/$1/libSDL3.so.0
ln -srf SDL3/lib/$1/libSDL3.so.0 SDL3/lib/$1/libSDL3.so

ln -srf SDL3_image/lib/$1/libSDL3_image.so.0.4.4 SDL3_image/lib/$1/libSDL3_image.so.0
ln -srf SDL3_image/lib/$1/libSDL3_image.so.0 SDL3_image/lib/$1/libSDL3_image.so
