/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#ifndef GDSTK_C_COMMON_H
#define GDSTK_C_COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
typedef enum {
    GDSTK_NO_ERROR = 0,
    // Warnings
    GDSTK_BOOLEAN_ERROR,
    GDSTK_EMPTY_PATH,
    GDSTK_INTERSECTION_NOT_FOUND,
    GDSTK_MISSING_REFERENCE,
    GDSTK_UNSUPPORTED_RECORD,
    GDSTK_UNOFFICIAL_SPECIFICATION,
    GDSTK_INVALID_REPETITION,
    GDSTK_OVERFLOW,
    // Errors
    GDSTK_CHECKSUM_ERROR,
    GDSTK_OUTPUT_FILE_OPEN_ERROR,
    GDSTK_INPUT_FILE_OPEN_ERROR,
    GDSTK_INPUT_FILE_ERROR,
    GDSTK_FILE_ERROR,
    GDSTK_INVALID_FILE,
    GDSTK_INSUFFICIENT_MEMORY,
    GDSTK_ZLIB_ERROR
} ErrorCode;

// Vec2 structure
typedef struct Vec2 {
    union {
        struct {
            double x, y;
        };
        struct {
            double u, v;
        };
        struct {
            double re, im;
        };
        double e[2];
    };
} Vec2;

// Generic Array structure
typedef struct Array {
    uint64_t capacity;  // allocated capacity
    uint64_t count;     // number of slots used
    void* items;        // slots (type depends on usage)
} Array;

// Generic Map structure
typedef struct Map {
    void* internal;  // Opaque pointer to implementation
} Map;

// Generic Set structure
typedef struct Set {
    void* internal;  // Opaque pointer to implementation
} Set;

// Tag type
typedef uint64_t Tag;

// Forward declarations for main types
typedef struct Cell Cell;
typedef struct Polygon Polygon;
typedef struct Label Label;
typedef struct Reference Reference;
typedef struct Library Library;
typedef struct RawCell RawCell;
typedef struct FlexPath FlexPath;
typedef struct RobustPath RobustPath;
typedef struct Curve Curve;
typedef struct Property Property;
typedef struct Repetition Repetition;
typedef struct RaithData RaithData;

// Forward declarations for complex types
typedef struct GeometryInfo GeometryInfo;
typedef struct StyleMap StyleMap;
typedef struct TagMap TagMap;

// Function pointer types
typedef bool (*PolygonComparisonFunction)(Polygon* const, Polygon* const);
typedef Vec2 (*ParametricCurveFunction)(double t, void* data);

// Curve instruction structure (used by curve commands)
typedef struct CurveInstruction {
    union {
        char command;
        double number;
    };
} CurveInstruction;

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_COMMON_H
