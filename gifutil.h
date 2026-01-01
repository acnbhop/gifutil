/*
 * (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 * gifutil.h: Core Header File For gifutil.
 */
#ifndef GIFUTIL_H
#define GIFUTIL_H

/* core header file */
#include "core.h"

/* return codes */
typedef enum gif_error 
{
    GIF_SUCCESS = 0,
    GIF_ERROR_FILE_NOT_FOUND,
    GIF_ERROR_DECODE_FAILED,
    GIF_ERROR_INVALID_FORMAT,
    GIF_ERROR_WRITE_FAILED,
    GIF_ERROR_ALLOCATION
} gif_error;

/* context holding loaded gif data */
typedef struct gif_context
{
    u8* raw_data;       /* pointer to raw image buffer (all frames) */
    s32 width;          /* width of each frame */
    s32 height;         /* height of each frame */
    s32 frames;         /* total number of frames (z-depth) */
    s32 channels;       /* components per pixel (4 for rgba usually) */
    s32* delays;        /* array of delays in ms for each frame */
} gif_context;

/* loads a gif from disk into memory. */
gif_error gif_load(const char* filepath, gif_context* out_context);

/* frees resources that were allocated. */
void gif_free(gif_context* context);

#endif /* GIFUTIL_H */