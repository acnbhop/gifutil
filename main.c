/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      main.c: Entry Point.
 */

/* core header file */
#include "core.h"

/* gifutil headers */
#include "extract.h"
#include "inspect.h"

/* standard headers */
#include <stdio.h>
#include <stdlib.h>

void print_usage(const char* prog_name)
{
    printf("Usage: %s <input.gif> extract <format> [output_prefix]\n", prog_name);
    printf("Formats: png, jpg, bmp\n");
    printf("Example: %s anim.gif extract png myframe\n", prog_name);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char* input_file = argv[1];
    const char* command = argv[2];

    /* load once for either command */
    gif_context context = {0};
    /* printf("Loading GIF: %s...\n", input_file); */
    
    gif_error load_result = gif_load(input_file, &context);
    if (load_result != GIF_SUCCESS)
    {
        fprintf(stderr, "Failed to load GIF: error code %d\n", load_result);
        return EXIT_FAILURE;
    }

    /* dispatch command */
    if (strcmp(command, "inspect") == 0)
    {
        gif_error err = inspect_gif(&context);
        if (err != GIF_SUCCESS)
        {
            fprintf(stderr, "Inspection failed: code %d\n", err);
        }
    }
    else if (strcmp(command, "extract") == 0)
    {
        if (argc < 4)
        {
            fprintf(stderr, "Extract command requires a format argument.\n");
            print_usage(argv[0]);
            gif_free(&context);
            return EXIT_FAILURE;
        }

        const char* format_string = argv[3];
        const char* output_prefix = (argc >= 5) ? argv[4] : "frame";

        output_format format;
        if (strcmp(format_string, "png") == 0) format = FMT_PNG;
        else if (strcmp(format_string, "jpg") == 0) format = FMT_JPG;
        else if (strcmp(format_string, "bmp") == 0) format = FMT_BMP;
        else
        {
            fprintf(stderr, "Unknown format: %s\n", format_string);
            gif_free(&context);
            return EXIT_FAILURE;
        }

        printf("Loaded GIF: %dx%d, %d frames\n", context.width, context.height, context.frames);
        
        gif_error extract_error = extract_frames(&context, output_prefix, format);
        if (extract_error != GIF_SUCCESS)
        {
            fprintf(stderr, "Failed to extract frames: error code %d\n", extract_error);
            gif_free(&context);
            return EXIT_FAILURE;
        }
        printf("Extraction complete.\n");
    }
    else
    {
        fprintf(stderr, "Unknown command: %s\n", command);
        print_usage(argv[0]);
        gif_free(&context);
        return EXIT_FAILURE;
    }

    gif_free(&context);
    return EXIT_SUCCESS;
}