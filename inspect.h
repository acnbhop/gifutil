/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      inspect.h: Inspect Header File.
 */
#ifndef INSPECT_H
#define INSPECT_H

/* core header file */
#include "core.h"

/* gifutil header file */
#include "gifutil.h"

/* inspects loaded gif context and prints details */
gif_error inspect_gif(const gif_context* context);

#endif /* INSPECT_H */