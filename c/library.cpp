/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "library.h"
#include "../include/gdstk/library.hpp"
#include "../include/gdstk/cell.hpp"
#include "../include/gdstk/rawcell.hpp"

// OASIS configuration constants
#define GDSTK_OASIS_CONFIG_DETECT_RECTANGLES 1
#define GDSTK_OASIS_CONFIG_DETECT_TRAPEZOIDS 2
#define GDSTK_OASIS_CONFIG_CHECK_DUPLICATES 4

extern "C" {

// Forward declaration of error conversion function (defined in cell.cpp)
ErrorCode convert_error_code(gdstk::ErrorCode cpp_error);

// Implementation of error code conversion
ErrorCode convert_error_code(gdstk::ErrorCode cpp_error) {
    switch (cpp_error) {
        case gdstk::ErrorCode::NoError: return GDSTK_NO_ERROR;
        case gdstk::ErrorCode::InsufficientMemory: return GDSTK_INSUFFICIENT_MEMORY;
        case gdstk::ErrorCode::InvalidFile: return GDSTK_INVALID_FILE;
        case gdstk::ErrorCode::MissingReference: return GDSTK_MISSING_REFERENCE;
        default: return GDSTK_INVALID_FILE;
    }
}

// Library functions
Library* library_new(const char* name, double unit, double precision) {
    gdstk::Library* cpp_library = new gdstk::Library();
    cpp_library->init(name, unit, precision);
    return reinterpret_cast<Library*>(cpp_library);
}

void library_free(Library* library) {
    if (library) {
        gdstk::Library* cpp_library = reinterpret_cast<gdstk::Library*>(library);
        cpp_library->free_all();
        delete cpp_library;
    }
}

void library_clear(Library* library) {
    if (library) {
        gdstk::Library* cpp_library = reinterpret_cast<gdstk::Library*>(library);
        cpp_library->clear();
    }
}

void library_print(const Library* library, bool all) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        cpp_library->print(all);
    }
}

// Copy function
void library_copy_from(Library* library, const Library* source, bool deep_copy) {
    if (library && source) {
        gdstk::Library* cpp_library = reinterpret_cast<gdstk::Library*>(library);
        const gdstk::Library* cpp_source = reinterpret_cast<const gdstk::Library*>(source);
        cpp_library->copy_from(*cpp_source, deep_copy);
    }
}

// Tags functions
void library_get_shape_tags(const Library* library, Set* result) {
    if (library && result) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        gdstk::Set<gdstk::Tag>* cpp_result = reinterpret_cast<gdstk::Set<gdstk::Tag>*>(result);
        cpp_library->get_shape_tags(*cpp_result);
    }
}

void library_get_label_tags(const Library* library, Set* result) {
    if (library && result) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        gdstk::Set<gdstk::Tag>* cpp_result = reinterpret_cast<gdstk::Set<gdstk::Tag>*>(result);
        cpp_library->get_label_tags(*cpp_result);
    }
}

// Top level cells functions
void library_top_level(const Library* library, Array* top_cells, Array* top_rawcells) {
    if (library && top_cells && top_rawcells) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        gdstk::Array<gdstk::Cell*>* cpp_top_cells = reinterpret_cast<gdstk::Array<gdstk::Cell*>*>(top_cells);
        gdstk::Array<gdstk::RawCell*>* cpp_top_rawcells = reinterpret_cast<gdstk::Array<gdstk::RawCell*>*>(top_rawcells);
        cpp_library->top_level(*cpp_top_cells, *cpp_top_rawcells);
    }
}

// Cell lookup functions
Cell* library_get_cell(const Library* library, const char* name) {
    if (library && name) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        gdstk::Cell* cpp_cell = cpp_library->get_cell(name);
        return reinterpret_cast<Cell*>(cpp_cell);
    }
    return nullptr;
}

RawCell* library_get_rawcell(const Library* library, const char* name) {
    if (library && name) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        gdstk::RawCell* cpp_rawcell = cpp_library->get_rawcell(name);
        return reinterpret_cast<RawCell*>(cpp_rawcell);
    }
    return nullptr;
}

// Cell management functions
void library_rename_cell(Library* library, const char* old_name, const char* new_name) {
    if (library && old_name && new_name) {
        gdstk::Library* cpp_library = reinterpret_cast<gdstk::Library*>(library);
        cpp_library->rename_cell(old_name, new_name);
    }
}

void library_rename_cell_by_ref(Library* library, Cell* cell, const char* new_name) {
    if (library && cell && new_name) {
        gdstk::Library* cpp_library = reinterpret_cast<gdstk::Library*>(library);
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        cpp_library->rename_cell(cpp_cell, new_name);
    }
}

void library_replace_cell(Library* library, Cell* old_cell, Cell* new_cell) {
    if (library && old_cell && new_cell) {
        gdstk::Library* cpp_library = reinterpret_cast<gdstk::Library*>(library);
        gdstk::Cell* cpp_old_cell = reinterpret_cast<gdstk::Cell*>(old_cell);
        gdstk::Cell* cpp_new_cell = reinterpret_cast<gdstk::Cell*>(new_cell);
        cpp_library->replace_cell(cpp_old_cell, cpp_new_cell);
    }
}

// GDSII I/O functions
ErrorCode library_write_gds(const Library* library, const char* filename, 
                           uint64_t max_points, struct tm* timestamp) {
    if (library && filename) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        gdstk::ErrorCode result = cpp_library->write_gds(filename, max_points, timestamp);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

ErrorCode library_read_gds(const char* filename, double unit, double precision, 
                          Library* library) {
    if (filename && library) {
        gdstk::ErrorCode error_code;
        gdstk::Library cpp_library = gdstk::read_gds(filename, unit, precision, nullptr, &error_code);
        
        if (error_code == gdstk::ErrorCode::NoError) {
            // Copy the loaded library into the provided library
            gdstk::Library* cpp_library_ptr = reinterpret_cast<gdstk::Library*>(library);
            cpp_library_ptr->copy_from(cpp_library, true);
        }
        
        return convert_error_code(error_code);
    }
    return GDSTK_INVALID_FILE;
}

// OASIS I/O functions  
ErrorCode library_write_oas(const Library* library, const char* filename, 
                           double compression_level, bool detect_rectangles,
                           bool detect_trapezoids, uint64_t circle_tolerance, 
                           double standard_properties, double validation, 
                           bool check_duplicates) {
    if (library && filename) {
        gdstk::Library* cpp_library = const_cast<gdstk::Library*>(reinterpret_cast<const gdstk::Library*>(library));
        
        // Build config flags
        uint16_t config_flags = 0;
        if (detect_rectangles) config_flags |= GDSTK_OASIS_CONFIG_DETECT_RECTANGLES;
        if (detect_trapezoids) config_flags |= GDSTK_OASIS_CONFIG_DETECT_TRAPEZOIDS;
        if (check_duplicates) config_flags |= GDSTK_OASIS_CONFIG_CHECK_DUPLICATES;
        
        gdstk::ErrorCode result = cpp_library->write_oas(filename, circle_tolerance, 
                                                         (uint8_t)compression_level, config_flags);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

// SVG output functions
ErrorCode library_write_svg(const Library* library, const char* filename, 
                           double scaling, uint32_t precision, void* shape_style,
                           void* label_style, const char* background, double pad, 
                           bool pad_as_percentage) {
    if (library && filename) {
        // Note: SVG output for entire library is not directly supported in GDSTK C++
        // This would need to iterate over cells and write them individually
        // For now, return an error indicating unsupported operation
        return GDSTK_UNSUPPORTED_RECORD;
    }
    return GDSTK_INVALID_FILE;
}

} // extern "C"
