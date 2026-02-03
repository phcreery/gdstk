#ifndef GDSTK_C_HEADER_ARRAY
#define GDSTK_C_HEADER_ARRAY

#include <stdint.h>
#include <stdbool.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Note: Array structure is defined in common.h

// Generic array functions (implemented)
Array* array_new(size_t item_size);
void array_free(Array* array);
void array_clear(Array* array);

// Basic array operations (implemented)
void array_append(Array* array, const void* item);
void array_insert(Array* array, uint64_t index, const void* item);
void array_remove(Array* array, uint64_t index);
void* array_get(const Array* array, uint64_t index);
void array_set(Array* array, uint64_t index, const void* item);
uint64_t array_size(const Array* array);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_ARRAY
