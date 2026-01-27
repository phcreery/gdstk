/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "reference.h"
#include "../include/gdstk/reference.hpp"
#include "../include/gdstk/utils.hpp"
#include <cstdlib>
#include <cstring>
#include <new>

extern "C" {

// Helper function to convert C++ ErrorCode to C ErrorCode
ErrorCode convert_reference_error_code(gdstk::ErrorCode cpp_error) {
    switch (cpp_error) {
        case gdstk::ErrorCode::NoError: return GDSTK_NO_ERROR;
        case gdstk::ErrorCode::BooleanError: return GDSTK_BOOLEAN_ERROR;
        case gdstk::ErrorCode::EmptyPath: return GDSTK_EMPTY_PATH;
        case gdstk::ErrorCode::IntersectionNotFound: return GDSTK_INTERSECTION_NOT_FOUND;
        case gdstk::ErrorCode::MissingReference: return GDSTK_MISSING_REFERENCE;
        case gdstk::ErrorCode::UnsupportedRecord: return GDSTK_UNSUPPORTED_RECORD;
        case gdstk::ErrorCode::UnofficialSpecification: return GDSTK_UNOFFICIAL_SPECIFICATION;
        case gdstk::ErrorCode::InvalidRepetition: return GDSTK_INVALID_REPETITION;
        case gdstk::ErrorCode::Overflow: return GDSTK_OVERFLOW;
        case gdstk::ErrorCode::ChecksumError: return GDSTK_CHECKSUM_ERROR;
        case gdstk::ErrorCode::OutputFileOpenError: return GDSTK_OUTPUT_FILE_OPEN_ERROR;
        case gdstk::ErrorCode::InputFileOpenError: return GDSTK_INPUT_FILE_OPEN_ERROR;
        case gdstk::ErrorCode::InputFileError: return GDSTK_INPUT_FILE_ERROR;
        case gdstk::ErrorCode::FileError: return GDSTK_FILE_ERROR;
        case gdstk::ErrorCode::InvalidFile: return GDSTK_INVALID_FILE;
        case gdstk::ErrorCode::InsufficientMemory: return GDSTK_INSUFFICIENT_MEMORY;
        case gdstk::ErrorCode::ZlibError: return GDSTK_ZLIB_ERROR;
        default: return GDSTK_NO_ERROR;
    }
}

// Reference functions
Reference* reference_new(Cell* cell, Vec2 origin, double rotation, double magnification, 
                        bool x_reflection, Tag tag) {
    if (!cell) return nullptr;
    
    gdstk::Reference* cpp_reference = static_cast<gdstk::Reference*>(malloc(sizeof(gdstk::Reference)));
    if (!cpp_reference) return nullptr;
    
    // Initialize the C++ object in-place
    new(cpp_reference) gdstk::Reference();
    
    cpp_reference->type = gdstk::ReferenceType::Cell;
    cpp_reference->cell = reinterpret_cast<gdstk::Cell*>(cell);
    cpp_reference->origin.x = origin.x;
    cpp_reference->origin.y = origin.y;
    cpp_reference->rotation = rotation;
    cpp_reference->magnification = magnification;
    cpp_reference->x_reflection = x_reflection;
    cpp_reference->repetition.type = gdstk::RepetitionType::None;
    
    return reinterpret_cast<Reference*>(cpp_reference);
}

void reference_free(Reference* reference) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->~Reference(); // Call destructor
        free(cpp_reference);
    }
}

void reference_clear(Reference* reference) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->clear();
    }
}

void reference_copy_from(Reference* reference, const Reference* source) {
    if (reference && source) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        const gdstk::Reference* cpp_source = reinterpret_cast<const gdstk::Reference*>(source);
        cpp_reference->copy_from(*cpp_source);
    }
}

void reference_print(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        cpp_reference->print();
    }
}

Cell* reference_cell(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        return reinterpret_cast<Cell*>(cpp_reference->cell);
    }
    return nullptr;
}

void reference_set_cell(Reference* reference, Cell* cell) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->cell = reinterpret_cast<gdstk::Cell*>(cell);
    }
}

Vec2 reference_origin(const Reference* reference) {
    Vec2 origin = {0, 0};
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        origin.x = cpp_reference->origin.x;
        origin.y = cpp_reference->origin.y;
    }
    return origin;
}

void reference_set_origin(Reference* reference, Vec2 origin) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->origin.x = origin.x;
        cpp_reference->origin.y = origin.y;
    }
}

double reference_rotation(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        return cpp_reference->rotation;
    }
    return 0.0;
}

void reference_set_rotation(Reference* reference, double rotation) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->rotation = rotation;
    }
}

double reference_magnification(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        return cpp_reference->magnification;
    }
    return 1.0;
}

void reference_set_magnification(Reference* reference, double magnification) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->magnification = magnification;
    }
}

bool reference_x_reflection(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        return cpp_reference->x_reflection;
    }
    return false;
}

void reference_set_x_reflection(Reference* reference, bool x_reflection) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->x_reflection = x_reflection;
    }
}

void reference_bounding_box(const Reference* reference, Vec2* min, Vec2* max) {
    if (reference && min && max) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Vec2 cpp_min, cpp_max;
        cpp_reference->bounding_box(cpp_min, cpp_max);
        min->x = cpp_min.x;
        min->y = cpp_min.y;
        max->x = cpp_max.x;
        max->y = cpp_max.y;
    }
}

void reference_convex_hull(const Reference* reference, Array* result) {
    if (reference && result) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Array<gdstk::Vec2>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(result);
        cpp_reference->convex_hull(*cpp_result);
    }
}

void reference_transform(Reference* reference, double magnification, bool x_reflection, double rotation, Vec2 origin) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        gdstk::Vec2 cpp_origin = {origin.x, origin.y};
        cpp_reference->transform(magnification, x_reflection, rotation, cpp_origin);
    }
}

void reference_set_repetition_rectangular(Reference* reference, uint64_t columns, uint64_t rows, Vec2 spacing) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->repetition.type = gdstk::RepetitionType::Rectangular;
        cpp_reference->repetition.columns = columns;
        cpp_reference->repetition.rows = rows;
        cpp_reference->repetition.spacing.x = spacing.x;
        cpp_reference->repetition.spacing.y = spacing.y;
    }
}

void reference_clear_repetition(Reference* reference) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->repetition.clear();
    }
}

// Note: The following functions have API mismatches and are commented out
// They would need to be implemented based on the actual GDSTK API

/*
void reference_apply_repetition(Reference* reference, Array* result) {
    // Implementation needs API research
}

void reference_get_polygons(const Reference* reference, bool apply_repetitions,
                            bool include_paths, int64_t depth, bool filter, 
                            uint64_t tag, Array* result) {
    // Implementation needs API research
}
*/

} // extern "C"
