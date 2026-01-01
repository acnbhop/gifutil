/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      extract.h: Extraction Of Frames Header File.
 */

#ifndef EXTRACT_H
#define EXTRACT_H

/* core header file */
#include "core.h"

/* the output format */
typedef enum output_format
{
    FMT_PNG = 0,
    FMT_JPG,
    FMT_BMP,
} output_format;

/* extract all frames from the context to the target directory/prefix */
gif_error extract_frames(
    const gif_context* context,
    const char* output_prefix,
    output_format format
);

#endif /* EXTRACT_H */