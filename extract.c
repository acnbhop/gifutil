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

/* extract all frames from the context to the target directory/prefix */
gif_error extract_frames(
    const gif_context* context,
    const char* output_prefix,
    output_format format
)
{
    /* if no context and context is pointing to raw_data, GIF_ERR_INVALID_FORMAT */
    if (!context || !context->raw_data)
        return GIF_ERROR_INVALID_FORMAT;

    /* stride is with (*) channels, bytes per row
        frame size if stride * height
    */
    int frame_size = context->width * context->channels * context->height;
    int stride = context->width * context->channels;

    for (int i = 0; i < context->frames; i++)
    {
        char filename[512];
        const char* ext = (format == FMT_PNG) ? "png" :
                          (format == FMT_JPG) ? "jpg" : "bmp";
        
        /* create */
        snprintf(
            filename,
            sizeof(filename),
            "%s_%03d.%s",
            output_prefix,
            (int)i,
            ext
        );

        unsigned char* frame_ptr = context->raw_data + (i * frame_size);
        usize result = 0;

        printf("Extracting frame %d to %s...\n", (int)i, filename);

        switch (format)
        {
            case FMT_PNG:
                result = stbi_write_png(
                    filename,
                    context->width,
                    context->height,
                    context->channels,
                    frame_ptr,
                    (usize)stride
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
    }
    return GIF_ERROR_WRITE_FAILED;
}