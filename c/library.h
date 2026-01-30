#ifndef GDSTK_C_HEADER_LIBRARY
#define GDSTK_C_HEADER_LIBRARY

#include "common.h"
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations for types not in common.h
typedef struct Cell Cell;
typedef struct RawCell RawCell;
typedef struct Property Property;

// Library structure
typedef struct Library {
    char* name;
    double unit;
    double precision;
    Array* cell_array;    // Array of Cell*
    Array* rawcell_array; // Array of RawCell*
    Property* properties;
    void* owner;  // For Python interface
} Library;

// Library functions
Library* library_new(const char* name, double unit, double precision);
void library_free(Library* library);
void library_clear(Library* library);
void library_print(const Library* library, bool all);

// Copy function
void library_copy_from(Library* library, const Library* source, bool deep_copy);

// Tags functions
void library_get_shape_tags(const Library* library, Set* result);
void library_get_label_tags(const Library* library, Set* result);

// Top level cells functions
void library_top_level(const Library* library, Array* top_cells, Array* top_rawcells);

// Cell lookup functions
Cell* library_get_cell(const Library* library, const char* name);
RawCell* library_get_rawcell(const Library* library, const char* name);

// Cell management functions
void library_rename_cell(Library* library, const char* old_name, const char* new_name);
void library_rename_cell_by_ref(Library* library, Cell* cell, const char* new_name);
void library_replace_cell(Library* library, Cell* old_cell, Cell* new_cell);

// GDSII I/O functions
ErrorCode library_write_gds(const Library* library, const char* filename, 
                           uint64_t max_points, struct tm* timestamp);

// OASIS I/O functions  
ErrorCode library_write_oas(const Library* library, const char* filename, 
                           double circle_tolerance, uint8_t deflate_level,
                           uint16_t config_flags);

// SVG output functions
ErrorCode library_write_svg(const Library* library, const char* filename, 
                           double scaling, uint32_t precision, void* shape_style,
                           void* label_style, const char* background, double pad, 
                           bool pad_as_percentage);

// Standalone I/O functions (match C++ API)
Library* read_gds(const char* filename, double unit, double tolerance, 
                  const Set* shape_tags, ErrorCode* error_code);
Library* read_oas(const char* filename, double unit, double tolerance, 
                  ErrorCode* error_code);

// Utility functions for file inspection
ErrorCode gds_units(const char* filename, double* unit, double* precision);
struct tm gds_timestamp(const char* filename, const struct tm* new_timestamp, ErrorCode* error_code);
ErrorCode gds_info(const char* filename, void* info);  // LibraryInfo not exposed in C wrapper
ErrorCode oas_precision(const char* filename, double* precision);
bool oas_validate(const char* filename, uint32_t* signature, ErrorCode* error_code);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_LIBRARY
