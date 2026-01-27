#ifndef GDSTK_C_HEADER_MAP
#define GDSTK_C_HEADER_MAP

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Vec2 Vec2;
typedef struct Cell Cell;
typedef struct GeometryInfo GeometryInfo;

// Map item structure
typedef struct MapItem {
    char* key;
    void* value;  // Generic pointer value
    size_t value_size;  // Size of the value for type safety
} MapItem;

// Generic Map structure  
typedef struct Map {
    uint64_t capacity;  // allocated capacity
    uint64_t count;     // number of items in the map
    MapItem* items;     // array with length capacity
    size_t value_size;  // size of each value
} Map;

// Generic map functions
Map* map_new(size_t value_size);
void map_free(Map* map);
void map_clear(Map* map);
void map_print(const Map* map, bool show_all);

// Map operations
void map_set(Map* map, const char* key, const void* value);
void* map_get(const Map* map, const char* key);
bool map_has_key(const Map* map, const char* key);
bool map_remove(Map* map, const char* key);
uint64_t map_size(const Map* map);
uint64_t map_capacity(const Map* map);

// Map iteration
MapItem* map_first(const Map* map);
MapItem* map_next(const Map* map, const MapItem* current);
void map_keys(const Map* map, char*** keys, uint64_t* count);  // Caller must free

// Map copying and merging
Map* map_copy(const Map* source);
void map_copy_from(Map* dest, const Map* source);
void map_merge(Map* dest, const Map* source, bool overwrite);

// Specialized map types for common uses

// Map of strings (char* -> char*)
Map* map_string_new(void);
void map_string_set(Map* map, const char* key, const char* value);
const char* map_string_get(const Map* map, const char* key);

// Map of pointers (char* -> void*)
Map* map_ptr_new(void);
void map_ptr_set(Map* map, const char* key, void* value);
void* map_ptr_get(const Map* map, const char* key);

// Map of integers (char* -> int64_t)
Map* map_int_new(void);
void map_int_set(Map* map, const char* key, int64_t value);
int64_t map_int_get(const Map* map, const char* key, int64_t default_value);

// Map of doubles (char* -> double)  
Map* map_double_new(void);
void map_double_set(Map* map, const char* key, double value);
double map_double_get(const Map* map, const char* key, double default_value);

// Map of Vec2 (char* -> Vec2)
Map* map_vec2_new(void);
void map_vec2_set(Map* map, const char* key, Vec2 value);
Vec2 map_vec2_get(const Map* map, const char* key, Vec2 default_value);

// Specialized maps for GDSTK types

// Map of Cells (char* -> Cell*)
Map* map_cell_new(void);
void map_cell_set(Map* map, const char* key, Cell* value);
Cell* map_cell_get(const Map* map, const char* key);

// Map of GeometryInfo (used for caching)
Map* map_geometry_info_new(void);
void map_geometry_info_set(Map* map, const char* key, const GeometryInfo* value);
GeometryInfo* map_geometry_info_get(Map* map, const char* key);

// Map utility functions
bool map_empty(const Map* map);
void map_reserve(Map* map, uint64_t capacity);
void map_shrink_to_fit(Map* map);

// Map validation and statistics
bool map_is_valid(const Map* map);
double map_load_factor(const Map* map);
void map_stats(const Map* map, uint64_t* collisions, uint64_t* max_chain_length);

// Map transformation functions
void map_transform_values(Map* map, void (*transform_func)(void* value, void* data), void* data);
Map* map_filter(const Map* source, bool (*filter_func)(const char* key, const void* value, void* data), void* data);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_MAP
