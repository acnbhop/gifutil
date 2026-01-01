/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      inspect.c: Inspect Source File.
 */

/* core header file */
#include "core.h"

/* file header */
#include "inspect.h"

/* standard headers */
#include <stdio.h>

/* inspects loaded gif context and prints details */
gif_error inspect_gif(const gif_context* context)
{
    /* validate context */
    if (!context)
        return GIF_ERROR_INVALID_FORMAT;

    /* calculate duration statistics */
    
    u32 total_duration_ms = 0;
    u32 min_delay = 0xFFFFFFFF;     /* maximum of u32 */
    u32 max_delay = 0;

    for (usize i = 0; i < (usize)context->frames; i++)
    {
        /* delay is in ms */
        u32 delay = (u32)context->delays[i];

        /* accumulate total duration */
        total_duration_ms += delay;

        /* check min */
        if (delay < min_delay)
            min_delay = delay;
        /* check max */
        if (delay > max_delay)
            max_delay = delay;
    }

    f32 duration_sec = (f32)total_duration_ms / 1000.0f;
    f32 avg_delay = (context->frames > 0) ? (f32)total_duration_ms / (f32)context->frames : 0.0f;

    /* print report */
    printf("\n=== GIF Inspection Report ===\n");
    printf("Canvas Size:    %d x %d\n", context->width, context->height);
    printf("Frame Count:    %d\n", context->frames);
    printf("Channels:       %d (RGBA)\n", context->channels);
    printf("Total Duration: %u ms (%.2f s)\n", total_duration_ms, (double)duration_sec);
    printf("-----------------------------\n");
    printf("Frame Delays:\n");
    printf("  Average:      %.2f ms\n", (double)avg_delay);
    printf("  Minimum:      %u ms\n", min_delay);
    printf("  Maximum:      %u ms\n", max_delay);
    printf("=============================\n");
    
    /* if there are few frames, list them specifically */
    if (context->frames <= 10)
    {
        printf("Frame List:\n");
        for (int i = 0; i < context->frames; i++)
        {
            printf("  Frame %02d: %d ms\n", i, context->delays[i]);
        }
        printf("=============================\n");
    }

    return GIF_SUCCESS;
}