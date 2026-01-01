/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      extract.c: Extraction Of Frames Source File.
 */

/* core header file */
#include "core.h"

/* file header */
#include "extract.h"

/* gifutil header file */
#include "gifutil.h"

/* standard headers */
#include <stdio.h>
#include <stdlib.h>

/* include stb write header 
    to get prototypes */
#include "stb_image_write.h"

gif_error extract_frames(
    const gif_context* context,
    const char* output_prefix,
    output_format format
)
{
    if (!context || !context->raw_data)
        return GIF_ERROR_INVALID_FORMAT;

    /* stride is width * channels (bytes per row) */
    s32 stride = context->width * context->channels;
    usize frame_size = (usize)(stride * context->height);

    for (s32 i = 0; i < context->frames; i++)
    {
        char filename[512];
        const char* ext = (format == FMT_PNG) ? "png" :
                          (format == FMT_JPG) ? "jpg" : "bmp";
        
        snprintf(
            filename,
            sizeof(filename),
            "%s_%03d.%s",
            output_prefix,
            i,
            ext
        );

        u8* frame_ptr = context->raw_data + (i * frame_size);
        int result = 0;

        printf("Extracting frame %d to %s...\n", i, filename);

        switch (format)
        {
            case FMT_PNG:
                result = stbi_write_png(
                    filename,
                    context->width,
                    context->height,
                    context->channels,
                    frame_ptr,
                    stride
                );
                break;
            case FMT_JPG:
                result = stbi_write_jpg(
                    filename,
                    context->width,
                    context->height,
                    context->channels,
                    frame_ptr,
                    90 /* quality */
                );
                break;
            case FMT_BMP:
                result = stbi_write_bmp(
                    filename,
                    context->width,
                    context->height,
                    context->channels,
                    frame_ptr
                );
                break;
            default:
                return GIF_ERROR_INVALID_FORMAT;
        }

        /* 0 indicates failure in stbi_write */
        if (result == 0)
        {
            return GIF_ERROR_WRITE_FAILED;
        }
    }

    return GIF_SUCCESS;
}