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

// Error code conversion from C++ to C
static ErrorCode convert_error_code(gdstk::ErrorCode cpp_error) {
    switch (cpp_error) {
        case gdstk::ErrorCode::NoError: return GDSTK_NO_ERROR;
        // Warnings
        case gdstk::ErrorCode::BooleanError: return GDSTK_BOOLEAN_ERROR;
        case gdstk::ErrorCode::EmptyPath: return GDSTK_EMPTY_PATH;
        case gdstk::ErrorCode::IntersectionNotFound: return GDSTK_INTERSECTION_NOT_FOUND;
        case gdstk::ErrorCode::MissingReference: return GDSTK_MISSING_REFERENCE;
        case gdstk::ErrorCode::UnsupportedRecord: return GDSTK_UNSUPPORTED_RECORD;
        case gdstk::ErrorCode::UnofficialSpecification: return GDSTK_UNOFFICIAL_SPECIFICATION;
        case gdstk::ErrorCode::InvalidRepetition: return GDSTK_INVALID_REPETITION;
        case gdstk::ErrorCode::Overflow: return GDSTK_OVERFLOW;
        // Errors
        case gdstk::ErrorCode::ChecksumError: return GDSTK_CHECKSUM_ERROR;
        case gdstk::ErrorCode::OutputFileOpenError: return GDSTK_OUTPUT_FILE_OPEN_ERROR;
        case gdstk::ErrorCode::InputFileOpenError: return GDSTK_INPUT_FILE_OPEN_ERROR;
        case gdstk::ErrorCode::InputFileError: return GDSTK_INPUT_FILE_ERROR;
        case gdstk::ErrorCode::FileError: return GDSTK_FILE_ERROR;
        case gdstk::ErrorCode::InvalidFile: return GDSTK_INVALID_FILE;
        case gdstk::ErrorCode::InsufficientMemory: return GDSTK_INSUFFICIENT_MEMORY;
        case gdstk::ErrorCode::ZlibError: return GDSTK_ZLIB_ERROR;
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

// Library property getters - safe access to internal data
const char* library_name(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        return cpp_library->name;
    }
    return nullptr;
}

double library_unit(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        return cpp_library->unit;
    }
    return 0.0;
}

double library_precision(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        return cpp_library->precision;
    }
    return 0.0;
}

Array* library_cell_array(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        // Return pointer to the embedded C++ Array object
        return reinterpret_cast<Array*>(const_cast<gdstk::Array<gdstk::Cell*>*>(&cpp_library->cell_array));
    }
    return nullptr;
}

Array* library_rawcell_array(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        // Return pointer to the embedded C++ Array object
        return reinterpret_cast<Array*>(const_cast<gdstk::Array<gdstk::RawCell*>*>(&cpp_library->rawcell_array));
    }
    return nullptr;
}

uint64_t library_cell_count(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        return cpp_library->cell_array.count;
    }
    return 0;
}

uint64_t library_rawcell_count(const Library* library) {
    if (library) {
        const gdstk::Library* cpp_library = reinterpret_cast<const gdstk::Library*>(library);
        return cpp_library->rawcell_array.count;
    }
    return 0;
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

// OASIS I/O functions  
ErrorCode library_write_oas(const Library* library, const char* filename, 
                           double circle_tolerance, uint8_t deflate_level,
                           uint16_t config_flags) {
    if (library && filename) {
        gdstk::Library* cpp_library = const_cast<gdstk::Library*>(reinterpret_cast<const gdstk::Library*>(library));
        gdstk::ErrorCode result = cpp_library->write_oas(filename, circle_tolerance, 
                                                         deflate_level, config_flags);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

// SVG output functions - Note: Not directly supported in GDSTK C++ for entire library
ErrorCode library_write_svg(const Library* library, const char* filename, 
                           double scaling, uint32_t precision, void* shape_style,
                           void* label_style, const char* background, double pad, 
                           bool pad_as_percentage) {
    // SVG output for entire library is not directly supported in GDSTK C++
    // This would need to iterate over cells and write them individually
    // For now, return an error indicating unsupported operation
    return GDSTK_UNSUPPORTED_RECORD;
}

// Standalone I/O functions (match C++ API)
Library* read_gds(const char* filename, double unit, double tolerance, 
                  const Set* shape_tags, ErrorCode* error_code) {
    if (!filename) {
        if (error_code) *error_code = GDSTK_INVALID_FILE;
        return nullptr;
    }
    
    gdstk::ErrorCode cpp_error_code;
    const gdstk::Set<gdstk::Tag>* cpp_shape_tags = nullptr;
    
    if (shape_tags) {
        cpp_shape_tags = reinterpret_cast<const gdstk::Set<gdstk::Tag>*>(shape_tags);
    }
    
    gdstk::Library cpp_library = gdstk::read_gds(filename, unit, tolerance, cpp_shape_tags, &cpp_error_code);
    
    if (error_code) {
        *error_code = convert_error_code(cpp_error_code);
    }
    
    if (cpp_error_code != gdstk::ErrorCode::NoError) {
        return nullptr;
    }
    
    // Allocate and copy the library
    gdstk::Library* result = new gdstk::Library();
    result->copy_from(cpp_library, true);
    return reinterpret_cast<Library*>(result);
}

Library* read_oas(const char* filename, double unit, double tolerance, 
                  ErrorCode* error_code) {
    if (!filename) {
        if (error_code) *error_code = GDSTK_INVALID_FILE;
        return nullptr;
    }
    
    gdstk::ErrorCode cpp_error_code;
    gdstk::Library cpp_library = gdstk::read_oas(filename, unit, tolerance, &cpp_error_code);
    
    if (error_code) {
        *error_code = convert_error_code(cpp_error_code);
    }
    
    if (cpp_error_code != gdstk::ErrorCode::NoError) {
        return nullptr;
    }
    
    // Allocate and copy the library
    gdstk::Library* result = new gdstk::Library();
    result->copy_from(cpp_library, true);
    return reinterpret_cast<Library*>(result);
}

// Utility functions for file inspection
ErrorCode gds_units(const char* filename, double* unit, double* precision) {
    if (!filename || !unit || !precision) {
        return GDSTK_INVALID_FILE;
    }
    
    gdstk::ErrorCode result = gdstk::gds_units(filename, *unit, *precision);
    return convert_error_code(result);
}

struct tm gds_timestamp(const char* filename, const struct tm* new_timestamp, ErrorCode* error_code) {
    gdstk::ErrorCode cpp_error_code;
    struct tm result = gdstk::gds_timestamp(filename, new_timestamp, &cpp_error_code);
    
    if (error_code) {
        *error_code = convert_error_code(cpp_error_code);
    }
    
    return result;
}

ErrorCode gds_info(const char* filename, void* info) {
    // LibraryInfo is not exposed in C wrapper, so this is a placeholder
    // In a full implementation, you'd need to create a C-compatible LibraryInfo struct
    return GDSTK_UNSUPPORTED_RECORD;
}

ErrorCode oas_precision(const char* filename, double* precision) {
    if (!filename || !precision) {
        return GDSTK_INVALID_FILE;
    }
    
    gdstk::ErrorCode result = gdstk::oas_precision(filename, *precision);
    return convert_error_code(result);
}

bool oas_validate(const char* filename, uint32_t* signature, ErrorCode* error_code) {
    if (!filename) {
        if (error_code) *error_code = GDSTK_INVALID_FILE;
        return false;
    }
    
    gdstk::ErrorCode cpp_error_code;
    bool result = gdstk::oas_validate(filename, signature, &cpp_error_code);
    
    if (error_code) {
        *error_code = convert_error_code(cpp_error_code);
    }
    
    return result;
}

} // extern "C"
