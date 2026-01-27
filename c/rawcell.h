#ifndef GDSTK_C_HEADER_RAWCELL
#define GDSTK_C_HEADER_RAWCELL

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Array Array;
typedef struct Map Map;
typedef enum ErrorCode ErrorCode;

// Raw source structure for file management
typedef struct RawSource {
    FILE* file;
    uint32_t uses;  // Reference count
} RawSource;

// RawCell structure - holds raw GDSII data
typedef struct RawCell {
    char* name;
    RawSource* source;
    union {
        uint8_t* data;      // In-memory data (when loaded)
        uint64_t offset;    // File offset (when on disk)
    };
    uint64_t size;
    Array* dependencies;    // Array of RawCell*
    void* owner;           // For Python interface
} RawCell;

// RawSource functions
RawSource* rawsource_new(FILE* file);
void rawsource_free(RawSource* source);
void rawsource_add_ref(RawSource* source);
void rawsource_remove_ref(RawSource* source);
int64_t rawsource_offset_read(const RawSource* source, void* buffer, 
                             uint64_t num_bytes, uint64_t offset);

// RawCell creation and management
RawCell* rawcell_new(const char* name);
void rawcell_free(RawCell* rawcell);
void rawcell_clear(RawCell* rawcell);
void rawcell_print(const RawCell* rawcell, bool all);

// RawCell data access
bool rawcell_is_loaded(const RawCell* rawcell);
void rawcell_load(RawCell* rawcell);
void rawcell_unload(RawCell* rawcell);
const uint8_t* rawcell_get_data(const RawCell* rawcell);
uint64_t rawcell_get_size(const RawCell* rawcell);

// RawCell dependencies
void rawcell_add_dependency(RawCell* rawcell, RawCell* dependency);
void rawcell_remove_dependency(RawCell* rawcell, RawCell* dependency);
Array* rawcell_get_dependencies(const RawCell* rawcell);
bool rawcell_has_dependency(const RawCell* rawcell, const RawCell* dependency);

// RawCell file operations
ErrorCode rawcell_to_gds(RawCell* rawcell, FILE* out);

// RawCell reading from files
Map* rawcell_read_gds(const char* filename, ErrorCode* error_code);
ErrorCode rawcell_read_gds_to_map(const char* filename, Map* rawcells);

// RawCell validation
bool rawcell_is_valid(const RawCell* rawcell);
ErrorCode rawcell_validate(const RawCell* rawcell);

// RawCell information
const char* rawcell_get_name(const RawCell* rawcell);
void rawcell_set_name(RawCell* rawcell, const char* name);

// RawCell utility functions
RawCell* rawcell_copy(const RawCell* rawcell);
bool rawcell_equal(const RawCell* rawcell1, const RawCell* rawcell2);

// Memory management for raw data
void rawcell_set_data(RawCell* rawcell, const uint8_t* data, uint64_t size);
uint8_t* rawcell_copy_data(const RawCell* rawcell);  // Caller must free

// RawCell statistics
void rawcell_get_stats(const RawCell* rawcell, uint64_t* num_dependencies, 
                      uint64_t* total_size, bool* is_loaded);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_RAWCELL
