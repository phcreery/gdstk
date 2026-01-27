/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "cell.h"
#include "../include/gdstk/cell.hpp"
#include "../include/gdstk/utils.hpp"

extern "C" {

// Helper function to convert C++ ErrorCode to C ErrorCode
ErrorCode convert_error_code(gdstk::ErrorCode cpp_error) {
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

// Cell functions
Cell* cell_new(const char* name) {
    gdstk::Cell* cpp_cell = new gdstk::Cell();
    cpp_cell->init(name);
    return reinterpret_cast<Cell*>(cpp_cell);
}

void cell_free(Cell* cell) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        cpp_cell->free_all();
        delete cpp_cell;
    }
}

void cell_clear(Cell* cell) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        cpp_cell->clear();
    }
}

void cell_print(const Cell* cell, bool all) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        cpp_cell->print(all);
    }
}

void cell_bounding_box(const Cell* cell, Vec2* min, Vec2* max) {
    if (cell && min && max) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_min, cpp_max;
        cpp_cell->bounding_box(cpp_min, cpp_max);
        *reinterpret_cast<gdstk::Vec2*>(min) = cpp_min;
        *reinterpret_cast<gdstk::Vec2*>(max) = cpp_max;
    }
}

GeometryInfo* cell_bounding_box_cached(const Cell* cell, Map* cache) {
    if (cell && cache) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Map<gdstk::GeometryInfo>* cpp_cache = reinterpret_cast<gdstk::Map<gdstk::GeometryInfo>*>(cache);
        gdstk::GeometryInfo cpp_info = cpp_cell->bounding_box(*cpp_cache);
        
        GeometryInfo* c_info = new GeometryInfo();
        c_info->convex_hull = reinterpret_cast<Array*>(&cpp_info.convex_hull);
        c_info->bounding_box_min = *reinterpret_cast<Vec2*>(&cpp_info.bounding_box_min);
        c_info->bounding_box_max = *reinterpret_cast<Vec2*>(&cpp_info.bounding_box_max);
        c_info->convex_hull_valid = cpp_info.convex_hull_valid;
        c_info->bounding_box_valid = cpp_info.bounding_box_valid;
        return c_info;
    }
    return nullptr;
}

void cell_convex_hull(const Cell* cell, Array* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Array<gdstk::Vec2>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(result);
        cpp_cell->convex_hull(*cpp_result);
    }
}

GeometryInfo* cell_convex_hull_cached(const Cell* cell, Map* cache) {
    if (cell && cache) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Map<gdstk::GeometryInfo>* cpp_cache = reinterpret_cast<gdstk::Map<gdstk::GeometryInfo>*>(cache);
        gdstk::GeometryInfo cpp_info = cpp_cell->convex_hull(*cpp_cache);
        
        GeometryInfo* c_info = new GeometryInfo();
        c_info->convex_hull = reinterpret_cast<Array*>(&cpp_info.convex_hull);
        c_info->bounding_box_min = *reinterpret_cast<Vec2*>(&cpp_info.bounding_box_min);
        c_info->bounding_box_max = *reinterpret_cast<Vec2*>(&cpp_info.bounding_box_max);
        c_info->convex_hull_valid = cpp_info.convex_hull_valid;
        c_info->bounding_box_valid = cpp_info.bounding_box_valid;
        return c_info;
    }
    return nullptr;
}

void cell_copy_from(Cell* cell, const Cell* source, const char* new_name, bool deep_copy) {
    if (cell && source) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        const gdstk::Cell* cpp_source = reinterpret_cast<const gdstk::Cell*>(source);
        cpp_cell->copy_from(*cpp_source, new_name, deep_copy);
    }
}

void cell_get_polygons(const Cell* cell, bool apply_repetitions, bool include_paths, 
                      int64_t depth, bool filter, Tag tag, Array* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Array<gdstk::Polygon*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Polygon*>*>(result);
        cpp_cell->get_polygons(apply_repetitions, include_paths, depth, filter, tag, *cpp_result);
    }
}

void cell_get_flexpaths(const Cell* cell, bool apply_repetitions, int64_t depth, 
                       bool filter, Tag tag, Array* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Array<gdstk::FlexPath*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::FlexPath*>*>(result);
        cpp_cell->get_flexpaths(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

void cell_get_robustpaths(const Cell* cell, bool apply_repetitions, int64_t depth, 
                         bool filter, Tag tag, Array* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Array<gdstk::RobustPath*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::RobustPath*>*>(result);
        cpp_cell->get_robustpaths(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

void cell_get_labels(const Cell* cell, bool apply_repetitions, int64_t depth, 
                    bool filter, Tag tag, Array* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Array<gdstk::Label*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Label*>*>(result);
        cpp_cell->get_labels(apply_repetitions, depth, filter, tag, *cpp_result);
    }
}

void cell_get_dependencies(const Cell* cell, bool recursive, Map* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Map<gdstk::Cell*>* cpp_result = reinterpret_cast<gdstk::Map<gdstk::Cell*>*>(result);
        cpp_cell->get_dependencies(recursive, *cpp_result);
    }
}

void cell_get_raw_dependencies(const Cell* cell, bool recursive, Map* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Map<gdstk::RawCell*>* cpp_result = reinterpret_cast<gdstk::Map<gdstk::RawCell*>*>(result);
        cpp_cell->get_raw_dependencies(recursive, *cpp_result);
    }
}

void cell_get_shape_tags(const Cell* cell, Set* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Set<gdstk::Tag>* cpp_result = reinterpret_cast<gdstk::Set<gdstk::Tag>*>(result);
        cpp_cell->get_shape_tags(*cpp_result);
    }
}

void cell_get_label_tags(const Cell* cell, Set* result) {
    if (cell && result) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Set<gdstk::Tag>* cpp_result = reinterpret_cast<gdstk::Set<gdstk::Tag>*>(result);
        cpp_cell->get_label_tags(*cpp_result);
    }
}

void cell_flatten(Cell* cell, bool apply_repetitions, Array* removed_references) {
    if (cell && removed_references) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Array<gdstk::Reference*>* cpp_removed = reinterpret_cast<gdstk::Array<gdstk::Reference*>*>(removed_references);
        cpp_cell->flatten(apply_repetitions, *cpp_removed);
    }
}

void cell_remap_tags(Cell* cell, const TagMap* map) {
    if (cell && map) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        const gdstk::TagMap* cpp_map = reinterpret_cast<const gdstk::TagMap*>(map);
        cpp_cell->remap_tags(*cpp_map);
    }
}

ErrorCode cell_to_gds(const Cell* cell, FILE* out, double scaling, uint64_t max_points, 
                     double precision, const struct tm* timestamp) {
    if (cell && out) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::ErrorCode result = cpp_cell->to_gds(out, scaling, max_points, precision, timestamp);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

ErrorCode cell_to_svg(const Cell* cell, FILE* out, double scaling, uint32_t precision, 
                     const char* attributes, PolygonComparisonFunction comp) {
    if (cell && out) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        // Convert C function pointer to C++ function pointer
        gdstk::PolygonComparisonFunction cpp_comp = reinterpret_cast<gdstk::PolygonComparisonFunction>(comp);
        gdstk::ErrorCode result = cpp_cell->to_svg(out, scaling, precision, attributes, cpp_comp);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

ErrorCode cell_write_svg(const Cell* cell, const char* filename, double scaling, 
                        uint32_t precision, StyleMap* shape_style, StyleMap* label_style, 
                        const char* background, double pad, bool pad_as_percentage, 
                        PolygonComparisonFunction comp) {
    if (cell && filename) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::StyleMap* cpp_shape_style = reinterpret_cast<gdstk::StyleMap*>(shape_style);
        gdstk::StyleMap* cpp_label_style = reinterpret_cast<gdstk::StyleMap*>(label_style);
        gdstk::PolygonComparisonFunction cpp_comp = reinterpret_cast<gdstk::PolygonComparisonFunction>(comp);
        gdstk::ErrorCode result = cpp_cell->write_svg(filename, scaling, precision, cpp_shape_style, 
                                                      cpp_label_style, background, pad, pad_as_percentage, cpp_comp);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

// GeometryInfo functions
GeometryInfo* geometry_info_new(void) {
    return new GeometryInfo();
}

void geometry_info_free(GeometryInfo* info) {
    if (info) {
        delete info;
    }
}

void geometry_info_clear(GeometryInfo* info) {
    if (info) {
        // Note: In a complete implementation, you'd need to properly handle the Array cleanup
        info->convex_hull_valid = false;
        info->bounding_box_valid = false;
    }
}

// Tag utility functions
Tag make_tag(uint32_t layer, uint32_t type) {
    return gdstk::make_tag(layer, type);
}

uint32_t get_layer(Tag tag) {
    return gdstk::get_layer(tag);
}

uint32_t get_type(Tag tag) {
    return gdstk::get_type(tag);
}

void set_layer(Tag* tag, uint32_t layer) {
    if (tag) {
        gdstk::set_layer(*tag, layer);
    }
}

void set_type(Tag* tag, uint32_t type) {
    if (tag) {
        gdstk::set_type(*tag, type);
    }
}

} // extern "C"
