/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      gifutil.c: Core Source File For gifutil.
 */

/* core header file */
#include "core.h"

/* file header */
#include "gifutil.h"

/* standard headers */
#include <stdio.h>
#include <stdlib.h>

/* define implementations for stb libraries in this file */
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

/* helper to read file into the memory buffer */
static u8* read_file_to_buffer(const char* filepath, usize* out_size)
{
    FILE* file = fopen(filepath, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    /* ftell returns long, ensure safe cast or check */
    long length_long = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (length_long < 0)
    {
        fclose(file);
        return NULL;
    }

    usize length = (usize)length_long;

    u8* buffer = (u8*)malloc(length);
    if (buffer)
    {
        if (fread(buffer, 1, length, file) != length)
        {
            free(buffer);
            buffer = NULL;
        }
    }

    *out_size = length;
    fclose(file);
    return buffer;
}

/* loads a gif from disk into memory. */
gif_error gif_load(const char* filepath, gif_context* out_context)
{
    if (!filepath || !out_context)
        return GIF_ERROR_INVALID_FORMAT;

    usize file_size = 0;
    u8* file_buffer = read_file_to_buffer(filepath, &file_size);
    
    if (!file_buffer)
        return GIF_ERROR_FILE_NOT_FOUND;
    
    /* stb uses standard int, so we use temp variables to be safe */
    int w, h, frames, comp;
    int req_comp = 4; /* rgba */
    int* delays = NULL;

    /* load as gif to get access to all frames. */
    u8* data = stbi_load_gif_from_memory(
        file_buffer,
        (int)file_size,
        &delays,
        &w, &h,
        &frames, &comp,
        req_comp
    );

    free(file_buffer);

    if (!data)
        return GIF_ERROR_DECODE_FAILED;

    out_context->raw_data = data;
    out_context->width = (s32)w;
    out_context->height = (s32)h;
    out_context->frames = (s32)frames;
    out_context->channels = (s32)req_comp;
    out_context->delays = (s32*)delays;

    return GIF_SUCCESS;
}

/* frees resources that were allocated. */
void gif_free(gif_context* context)
{
    if (context->raw_data)
    {
        stbi_image_free(context->raw_data);
        context->raw_data = NULL;
    }

    if (context->delays)
    {
        stbi_image_free(context->delays);
        context->delays = NULL;
    }

    context->width = 0;
    context->height = 0;
    context->frames = 0;
}