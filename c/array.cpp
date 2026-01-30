#include "array.h"
#include "../include/gdstk/utils.hpp"
#include <cstdlib>

// Generic array utility functions
Array* array_new(size_t item_size) {
    // For a generic pointer array, item_size is ignored
    // since we store void* pointers
    gdstk::Array<void*>* cpp_array = new gdstk::Array<void*>();
    return reinterpret_cast<Array*>(cpp_array);
}

void array_free(Array* array) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        delete cpp_array;
    }
}

void array_clear(Array* array) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->clear();
    }
}

void array_append(Array* array, const void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->append(const_cast<void*>(item));
    }
}

void array_insert(Array* array, uint64_t index, const void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        if (index <= cpp_array->count) {
            cpp_array->ensure_slots(1);
            // Shift elements to make room
            for (uint64_t i = cpp_array->count; i > index; --i) {
                cpp_array->items[i] = cpp_array->items[i-1];
            }
            cpp_array->items[index] = const_cast<void*>(item);
            cpp_array->count++;
        }
    }
}

void array_remove(Array* array, uint64_t index) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        if (index < cpp_array->count) {
            // Shift elements to close the gap
            for (uint64_t i = index; i < cpp_array->count - 1; ++i) {
                cpp_array->items[i] = cpp_array->items[i+1];
            }
            cpp_array->count--;
        }
    }
}

void* array_get(const Array* array, uint64_t index) {
    if (array) {
        const gdstk::Array<void*>* cpp_array = reinterpret_cast<const gdstk::Array<void*>*>(array);
        if (index < cpp_array->count) {
            return cpp_array->items[index];
        }
    }
    return nullptr;
}

void array_set(Array* array, uint64_t index, const void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        if (index < cpp_array->count) {
            cpp_array->items[index] = const_cast<void*>(item);
        }
    }
}

uint64_t array_size(const Array* array) {
    if (array) {
        const gdstk::Array<void*>* cpp_array = reinterpret_cast<const gdstk::Array<void*>*>(array);
        return cpp_array->count;
    }
    return 0;
}
