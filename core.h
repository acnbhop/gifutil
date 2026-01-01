/*
 *  (C) Copyright 2025/2026 Grant Abernathy & Kendalynn Kohler. All Rights Reserved.
 *
 *      core.h: Core Header File.
 */
#ifndef CORE_H
#define CORE_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif

#include <stdint.h>
#include <stddef.h>

typedef float           f32;
typedef double          f64;

typedef int8_t          s8;
typedef int16_t         s16;
typedef int32_t         s32;
typedef int64_t         s64;

typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;

typedef size_t          usize;
typedef ptrdiff_t       isize;
typedef intmax_t        imax;
typedef uintmax_t       umax;
typedef uintptr_t       uptr;
typedef intptr_t        iptr;

#endif // CORE_H