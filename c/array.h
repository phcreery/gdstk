#ifndef GDSTK_C_HEADER_ARRAY
#define GDSTK_C_HEADER_ARRAY

#include <stdint.h>
#include <stdbool.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Note: Array structure is defined in common.h

// Generic array functions
Array* array_new(size_t item_size);
void array_free(Array* array);
void array_clear(Array* array);
void array_print(const Array* array, bool show_all);

// Generic array operations
void array_append(Array* array, const void* item);
void array_insert(Array* array, uint64_t index, const void* item);
void array_remove(Array* array, uint64_t index);
void array_remove_unordered(Array* array, uint64_t index);
void* array_get(const Array* array, uint64_t index);
void array_set(Array* array, uint64_t index, const void* item);
uint64_t array_size(const Array* array);
uint64_t array_capacity(const Array* array);

// Array iteration
void* array_first(const Array* array);
void* array_last(const Array* array);
void* array_at(const Array* array, uint64_t index);

// Array manipulation
void array_resize(Array* array, uint64_t new_size);
void array_reserve(Array* array, uint64_t capacity);
void array_shrink_to_fit(Array* array);
bool array_empty(const Array* array);

// Array copying and extending
Array* array_copy(const Array* source);
void array_copy_from(Array* dest, const Array* source);
void array_extend(Array* dest, const Array* source);

// Specialized array types for common uses

// Array of Vec2
Array* array_vec2_new(void);
void array_vec2_append(Array* array, Vec2 item);
void array_vec2_insert(Array* array, uint64_t index, Vec2 item);
Vec2 array_vec2_get(const Array* array, uint64_t index);
void array_vec2_set(Array* array, uint64_t index, Vec2 item);

// Array of pointers
Array* array_ptr_new(void);
void array_ptr_append(Array* array, void* item);
void array_ptr_insert(Array* array, uint64_t index, void* item);
void* array_ptr_get(const Array* array, uint64_t index);
void array_ptr_set(Array* array, uint64_t index, void* item);

// Array of doubles
Array* array_double_new(void);
void array_double_append(Array* array, double item);
void array_double_insert(Array* array, uint64_t index, double item);
double array_double_get(const Array* array, uint64_t index);
void array_double_set(Array* array, uint64_t index, double item);

// Array of integers
Array* array_int_new(void);
void array_int_append(Array* array, int item);
void array_int_insert(Array* array, uint64_t index, int item);
int array_int_get(const Array* array, uint64_t index);
void array_int_set(Array* array, uint64_t index, int item);

// Array of uint64_t
Array* array_uint64_new(void);
void array_uint64_append(Array* array, uint64_t item);
void array_uint64_insert(Array* array, uint64_t index, uint64_t item);
uint64_t array_uint64_get(const Array* array, uint64_t index);
void array_uint64_set(Array* array, uint64_t index, uint64_t item);

// Array searching and sorting
int64_t array_find(const Array* array, const void* item);
bool array_contains(const Array* array, const void* item);
void array_reverse(Array* array);

// Array statistics for numeric types
double array_double_sum(const Array* array);
double array_double_mean(const Array* array);
double array_double_min(const Array* array);
double array_double_max(const Array* array);

// Array transformation for Vec2 arrays
void array_vec2_translate(Array* array, Vec2 offset);
void array_vec2_scale(Array* array, Vec2 scale, Vec2 center);
void array_vec2_rotate(Array* array, double angle, Vec2 center);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_ARRAY
