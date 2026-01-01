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

/* helper to read file into the memory buffer (stbi_load_gif_from_memory is more reliable for frames tho) */
static unsigned char* read_file_to_buffer(const char* filepath, usize* out_size)
{
    /* file pointer */
    FILE* file = fopen(filepath, "rb");
    /* if not, return null */
    if (!file) return NULL;

    /* seek to the end */
    fseek(file, 0, SEEK_END);
    /* get the size */
    usize length = (usize)ftell(file);
    /* seek back to the beginning */
    fseek(file, 0, SEEK_SET);

    /* buffer, malloc to length */
    unsigned char* buffer = (unsigned char*)malloc(length);
    /* read into buffer */
    if (buffer)
    {
        /*
            okay, so fread (f-read) returns the number of elements read, which should be equal to length
            since we're reading 1-byte elements. if it doesn't match, we have an error and
            should free the buffer and return NULL.
        */
        if (fread(buffer, 1, length, file) != length)
        {
            free(buffer);
            buffer = NULL;
        }
    }

    /* out size is length */
    *out_size = length;
    /* close */
    fclose(file);
    /* return buffer */
    return buffer;
}

/* loads a gif from disk into memory. */
gif_error gif_load(const char* filepath, gif_context* out_context)
{
    /* if file path and out context is invalid, return GIF_ERR_INVALID_FORMAT */
    if (!filepath || !out_context)
        return GIF_ERROR_INVALID_FORMAT;

    /* file size */
    usize file_size = 0;
    /* file buffer, we read file to the buffer with file path and address to the file size */
    unsigned char* file_buffer = read_file_to_buffer(filepath, &file_size);
    /* if it is null, file was not found */
    if (!file_buffer)
        return GIF_ERROR_FILE_NOT_FOUND;
    
    /* setup with, height, frames and comp */
    int w, h, frames, comp;
    int req_comp = 4; /* rgba */
    int* delays = NULL;

    /* load as gif to get access to all frames. */
    unsigned char* data = stbi_load_gif_from_memory(
        file_buffer,
        (int)file_size,
        &delays,
        &w, &h,
        &frames, &comp,
        req_comp
    );

    /* free buffer, we don't need it anymore. */
    free(file_buffer);

    /* if the data is invalid, decoding has failed. */
    if (!data)
        return GIF_ERROR_DECODE_FAILED;

    /* populate out context */
    out_context->raw_data = data;
    out_context->width = w;
    out_context->height = h;
    out_context->frames = frames;
    out_context->channels = req_comp; /* we forced this */
    out_context->delays = delays;

    /* success */
    return GIF_SUCCESS;
}

/* frees resources that were allocated. */
void gif_free(gif_context* context)
{
    /* free raw data */
    if (context->raw_data)
    {
        stbi_image_free(context->raw_data);
        context->raw_data = NULL;
    }

    /* free delays array */
    if (context->delays)
    {
        free(context->delays);
        context->delays = NULL;
    }

    /* reset other fields */

    context->width = 0;
    context->height = 0;
    context->frames = 0;
}