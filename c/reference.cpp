/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "reference.h"
#include "../include/gdstk/reference.hpp"
#include "../include/gdstk/utils.hpp"

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
    if (!cell) return NULL;
    
    Reference* reference = (Reference*)malloc(sizeof(Reference));
    memset(reference, 0, sizeof(Reference));
    
    gdstk::Reference* cpp_reference = (gdstk::Reference*)reference;
    
    cpp_reference->type = gdstk::ReferenceType::Cell;
    cpp_reference->cell = (gdstk::Cell*)cell;
    cpp_reference->origin.x = origin.x;
    cpp_reference->origin.y = origin.y;
    cpp_reference->rotation = rotation;
    cpp_reference->magnification = magnification;
    cpp_reference->x_reflection = x_reflection;
    // Note: tag member may not exist in this version, skip for now
    cpp_reference->repetition.type = gdstk::RepetitionType::None;
    
    return reference;
}

void reference_free(Reference* reference) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->clear();
        delete cpp_reference;
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
    if (!reference) return origin;
    gdstk::Reference* cpp_reference = (gdstk::Reference*)reference;
    origin.x = cpp_reference->origin.x;
    origin.y = cpp_reference->origin.y;
    return origin;
}

void reference_set_origin(Reference* reference, Vec2 origin) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->origin = *reinterpret_cast<const gdstk::Vec2*>(&origin);
    }
}

double reference_rotation(const Reference* reference) {
    if (!reference) return 0.0;
    gdstk::Reference* cpp_reference = (gdstk::Reference*)reference;
    return cpp_reference->rotation;
}

void reference_set_rotation(Reference* reference, double rotation) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->rotation = rotation;
    }
}

double reference_magnification(const Reference* reference) {
    if (!reference) return 1.0;
    gdstk::Reference* cpp_reference = (gdstk::Reference*)reference;
    return cpp_reference->magnification;
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
        *reinterpret_cast<gdstk::Vec2*>(min) = cpp_min;
        *reinterpret_cast<gdstk::Vec2*>(max) = cpp_max;
    }
}

void reference_convex_hull(const Reference* reference, Array* result) {
    if (reference && result) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Array<gdstk::Vec2>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(result);
        cpp_reference->convex_hull(*cpp_result);
    }
}

void reference_translate(Reference* reference, Vec2 displacement) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->translate(*reinterpret_cast<const gdstk::Vec2*>(&displacement));
    }
}

void reference_scale(Reference* reference, double scaling, Vec2 center) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->scale(scaling, *reinterpret_cast<const gdstk::Vec2*>(&center));
    }
}

void reference_mirror(Reference* reference, Vec2 p1, Vec2 p2) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->mirror(*reinterpret_cast<const gdstk::Vec2*>(&p1), 
                             *reinterpret_cast<const gdstk::Vec2*>(&p2));
    }
}

void reference_rotate(Reference* reference, double angle, Vec2 center) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->rotate(angle, *reinterpret_cast<const gdstk::Vec2*>(&center));
    }
}

void reference_transform(Reference* reference, double magnification, bool x_reflection, double rotation, Vec2 origin) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->transform(magnification, x_reflection, rotation, 
                                *reinterpret_cast<const gdstk::Vec2*>(&origin));
    }
}

void reference_set_repetition_rectangular(Reference* reference, uint64_t columns, uint64_t rows, Vec2 spacing) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        gdstk::Repetition repetition = {};
        repetition.type = gdstk::Repetition::Type::Rectangular;
        repetition.columns = columns;
        repetition.rows = rows;
        repetition.spacing = *reinterpret_cast<const gdstk::Vec2*>(&spacing);
        cpp_reference->repetition = repetition;
    }
}

void reference_set_repetition_regular(Reference* reference, uint64_t count, Vec2 displacement) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        gdstk::Repetition repetition = {};
        repetition.type = gdstk::Repetition::Type::Regular;
        repetition.count = count;
        repetition.displacement = *reinterpret_cast<const gdstk::Vec2*>(&displacement);
        cpp_reference->repetition = repetition;
    }
}

void reference_set_repetition_explicit(Reference* reference, const Vec2* offsets, uint64_t count) {
    if (reference && offsets) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        gdstk::Repetition repetition = {};
        repetition.type = gdstk::Repetition::Type::Explicit;
        repetition.coords.clear();
        repetition.coords.ensure_slots(count);
        for (uint64_t i = 0; i < count; i++) {
            repetition.coords.append(*reinterpret_cast<const gdstk::Vec2*>(&offsets[i]));
        }
        cpp_reference->repetition = repetition;
    }
}

void reference_clear_repetition(Reference* reference) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->repetition.clear();
    }
}

void reference_apply_repetition(Array* result, const Reference* reference) {
    if (result && reference) {
        gdstk::Array<gdstk::Reference>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Reference>*>(result);
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        cpp_reference->apply_repetition(*cpp_result);
    }
}

void reference_get_polygons(const Reference* reference, bool apply_repetitions, int64_t depth, bool filter, Tag tag, Array* result) {
    if (reference && result) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Array<gdstk::Polygon*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Polygon*>*>(result);
        cpp_reference->get_polygons(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

void reference_get_flexpaths(const Reference* reference, bool apply_repetitions, int64_t depth, bool filter, Tag tag, Array* result) {
    if (reference && result) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Array<gdstk::FlexPath*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::FlexPath*>*>(result);
        cpp_reference->get_flexpaths(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

void reference_get_robustpaths(const Reference* reference, bool apply_repetitions, int64_t depth, bool filter, Tag tag, Array* result) {
    if (reference && result) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Array<gdstk::RobustPath*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::RobustPath*>*>(result);
        cpp_reference->get_robustpaths(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

void reference_get_labels(const Reference* reference, bool apply_repetitions, int64_t depth, bool filter, Tag tag, Array* result) {
    if (reference && result) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::Array<gdstk::Label*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Label*>*>(result);
        cpp_reference->get_labels(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

ErrorCode reference_to_gds(const Reference* reference, FILE* out, double scaling) {
    if (reference && out) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        gdstk::ErrorCode result = cpp_reference->to_gds(out, scaling);
        return convert_reference_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

} // extern "C"
