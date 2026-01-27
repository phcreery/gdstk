#ifndef GDSTK_C_HEADER_UTILS
#define GDSTK_C_HEADER_UTILS

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Vec2 utility functions
Vec2 vec2_new(double x, double y);
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_subtract(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 v, double scalar);
Vec2 vec2_div(Vec2 v, double scalar);
double vec2_dot(Vec2 a, Vec2 b);
double vec2_cross(Vec2 a, Vec2 b);
double vec2_length(Vec2 v);
double vec2_length_squared(Vec2 v);
Vec2 vec2_normalize(Vec2 v);
Vec2 vec2_rotate(Vec2 v, double angle);
double vec2_angle(Vec2 v);
double vec2_distance(Vec2 a, Vec2 b);
bool vec2_equal(Vec2 a, Vec2 b);

// Array utility functions (for Vec2 arrays)
Array* array_vec2_new(void);
void array_vec2_free(Array* array);
void array_vec2_clear(Array* array);
void array_vec2_append(Array* array, Vec2 item);
void array_vec2_insert(Array* array, uint64_t index, Vec2 item);
void array_vec2_remove(Array* array, uint64_t index);
Vec2 array_vec2_get(const Array* array, uint64_t index);
void array_vec2_set(Array* array, uint64_t index, Vec2 item);
uint64_t array_vec2_size(const Array* array);

// Array utility functions (for pointer arrays)
Array* array_ptr_new(void);
void array_ptr_free(Array* array);
void array_ptr_clear(Array* array);
void array_ptr_append(Array* array, void* item);
void array_ptr_insert(Array* array, uint64_t index, void* item);
void array_ptr_remove(Array* array, uint64_t index);
void* array_ptr_get(const Array* array, uint64_t index);
void array_ptr_set(Array* array, uint64_t index, void* item);
uint64_t array_ptr_size(const Array* array);

// Map utility functions
Map* map_new(void);
void map_free(Map* map);
void map_clear(Map* map);
bool map_has_key(const Map* map, const char* key);
void* map_get(const Map* map, const char* key);
void map_set(Map* map, const char* key, void* value);
void map_remove(Map* map, const char* key);
uint64_t map_size(const Map* map);

// Set utility functions
Set* set_new(void);
void set_free(Set* set);
void set_clear(Set* set);
bool set_has(const Set* set, uint64_t value);
void set_add(Set* set, uint64_t value);
void set_remove(Set* set, uint64_t value);
uint64_t set_size(const Set* set);

// Tag utility functions (already in cell.h but included for completeness)
typedef uint64_t Tag;
Tag make_tag(uint32_t layer, uint32_t type);
uint32_t get_layer(Tag tag);
uint32_t get_type(Tag tag);
void set_layer(Tag* tag, uint32_t layer);
void set_type(Tag* tag, uint32_t type);

// Memory management functions
void* gdstk_allocate(uint64_t size);
void* gdstk_reallocate(void* ptr, uint64_t size);
void gdstk_free(void* ptr);

// Test utility functions
bool approx_equal(double a, double b, double tolerance);
bool points_equal(Vec2 a, Vec2 b);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_UTILS
