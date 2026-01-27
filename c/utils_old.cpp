/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "utils.h"
#include "../include/gdstk/utils.hpp"
#include "../include/gdstk/vec.hpp"
#include "../include/gdstk/array.hpp"
#include "../include/gdstk/map.hpp"
#include "../include/gdstk/set.hpp"
#include <cmath>

extern "C" {

// Vec2 utility functions
Vec2 vec2_new(double x, double y) {
    Vec2 result;
    result.x = x;
    result.y = y;
    return result;
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Vec2 vec2_mul(Vec2 v, double scalar) {
    Vec2 result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

Vec2 vec2_div(Vec2 v, double scalar) {
    Vec2 result;
    if (scalar != 0.0) {
        result.x = v.x / scalar;
        result.y = v.y / scalar;
    } else {
        result.x = 0.0;
        result.y = 0.0;
    }
    return result;
}

double vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

double vec2_cross(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

double vec2_length(Vec2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double vec2_length_squared(Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

Vec2 vec2_normalize(Vec2 v) {
    double len = vec2_length(v);
    return vec2_div(v, len);
}

Vec2 vec2_rotate(Vec2 v, double angle) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    Vec2 result;
    result.x = v.x * cos_a - v.y * sin_a;
    result.y = v.x * sin_a + v.y * cos_a;
    return result;
}

double vec2_angle(Vec2 v) {
    return atan2(v.y, v.x);
}

double vec2_distance(Vec2 a, Vec2 b) {
    return vec2_length(vec2_sub(a, b));
}

bool vec2_equal(Vec2 a, Vec2 b) {
    const double EPSILON = 1e-12;
    return (fabs(a.x - b.x) < EPSILON) && (fabs(a.y - b.y) < EPSILON);
}

// Array utility functions (for Vec2 arrays)
Array* array_vec2_new(void) {
    gdstk::Array<gdstk::Vec2>* cpp_array = new gdstk::Array<gdstk::Vec2>();
    return reinterpret_cast<Array*>(cpp_array);
}

void array_vec2_free(Array* array) {
    if (array) {
        gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(array);
        delete cpp_array;
    }
}

void array_vec2_clear(Array* array) {
    if (array) {
        gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(array);
        cpp_array->clear();
    }
}

void array_vec2_append(Array* array, Vec2 item) {
    if (array) {
        gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(array);
        gdstk::Vec2 cpp_item;
        cpp_item.x = item.x;
        cpp_item.y = item.y;
        cpp_array->append(cpp_item);
    }
}

void array_vec2_insert(Array* array, uint64_t index, Vec2 item) {
    if (array) {
        gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(array);
        gdstk::Vec2 cpp_item;
        cpp_item.x = item.x;
        cpp_item.y = item.y;
        cpp_array->insert(index, cpp_item);
    }
}

void array_vec2_remove(Array* array, uint64_t index) {
    if (array) {
        gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(array);
        cpp_array->remove(index);
    }
}

Vec2 array_vec2_get(const Array* array, uint64_t index) {
    Vec2 result = {0.0, 0.0};
    if (array) {
        const gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(array);
        if (index < cpp_array->count) {
            const gdstk::Vec2& cpp_item = (*cpp_array)[index];
            result.x = cpp_item.x;
            result.y = cpp_item.y;
        }
    }
    return result;
}

void array_vec2_set(Array* array, uint64_t index, Vec2 item) {
    if (array) {
        gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<gdstk::Array<gdstk::Vec2>*>(array);
        if (index < cpp_array->count) {
            gdstk::Vec2& cpp_item = (*cpp_array)[index];
            cpp_item.x = item.x;
            cpp_item.y = item.y;
        }
    }
}

uint64_t array_vec2_size(const Array* array) {
    if (array) {
        const gdstk::Array<gdstk::Vec2>* cpp_array = reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(array);
        return cpp_array->count;
    }
    return 0;
}

// Array utility functions (for pointer arrays)
Array* array_ptr_new(void) {
    gdstk::Array<void*>* cpp_array = new gdstk::Array<void*>();
    return reinterpret_cast<Array*>(cpp_array);
}

void array_ptr_free(Array* array) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        delete cpp_array;
    }
}

void array_ptr_clear(Array* array) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->clear();
    }
}

void array_ptr_append(Array* array, void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->append(item);
    }
}

void array_ptr_insert(Array* array, uint64_t index, void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->insert(index, item);
    }
}

void array_ptr_remove(Array* array, uint64_t index) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->remove(index);
    }
}

void* array_ptr_get(const Array* array, uint64_t index) {
    if (array) {
        const gdstk::Array<void*>* cpp_array = reinterpret_cast<const gdstk::Array<void*>*>(array);
        if (index < cpp_array->count) {
            return (*cpp_array)[index];
        }
    }
    return nullptr;
}

void array_ptr_set(Array* array, uint64_t index, void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        if (index < cpp_array->count) {
            (*cpp_array)[index] = item;
        }
    }
}

uint64_t array_ptr_size(const Array* array) {
    if (array) {
        const gdstk::Array<void*>* cpp_array = reinterpret_cast<const gdstk::Array<void*>*>(array);
        return cpp_array->count;
    }
    return 0;
}

// Map utility functions - simplified implementation using string keys
// Note: This is a simplified implementation. A full implementation would need
// to handle the template nature of gdstk::Map properly
Map* map_new(void) {
    // Create a simple map implementation - for demonstration purposes
    // In a real implementation, you'd want to create a wrapper around gdstk::Map
    return nullptr; // Placeholder
}

void map_free(Map* map) {
    // Placeholder implementation
}

void map_clear(Map* map) {
    // Placeholder implementation
}

bool map_has_key(const Map* map, const char* key) {
    // Placeholder implementation
    return false;
}

void* map_get(const Map* map, const char* key) {
    // Placeholder implementation
    return nullptr;
}

void map_set(Map* map, const char* key, void* value) {
    // Placeholder implementation
}

void map_remove(Map* map, const char* key) {
    // Placeholder implementation
}

uint64_t map_size(const Map* map) {
    // Placeholder implementation
    return 0;
}

// Set utility functions - simplified implementation
Set* set_new(void) {
    gdstk::Set<uint64_t>* cpp_set = new gdstk::Set<uint64_t>();
    return reinterpret_cast<Set*>(cpp_set);
}

void set_free(Set* set) {
    if (set) {
        gdstk::Set<uint64_t>* cpp_set = reinterpret_cast<gdstk::Set<uint64_t>*>(set);
        delete cpp_set;
    }
}

void set_clear(Set* set) {
    if (set) {
        gdstk::Set<uint64_t>* cpp_set = reinterpret_cast<gdstk::Set<uint64_t>*>(set);
        cpp_set->clear();
    }
}

bool set_has(const Set* set, uint64_t value) {
    if (set) {
        const gdstk::Set<uint64_t>* cpp_set = reinterpret_cast<const gdstk::Set<uint64_t>*>(set);
        return cpp_set->has_value(value);
    }
    return false;
}

void set_add(Set* set, uint64_t value) {
    if (set) {
        gdstk::Set<uint64_t>* cpp_set = reinterpret_cast<gdstk::Set<uint64_t>*>(set);
        cpp_set->add(value);
    }
}

void set_remove(Set* set, uint64_t value) {
    if (set) {
        gdstk::Set<uint64_t>* cpp_set = reinterpret_cast<gdstk::Set<uint64_t>*>(set);
        cpp_set->remove(value);
    }
}

uint64_t set_size(const Set* set) {
    if (set) {
        const gdstk::Set<uint64_t>* cpp_set = reinterpret_cast<const gdstk::Set<uint64_t>*>(set);
        return cpp_set->count;
    }
    return 0;
}

// Tag utility functions (already implemented in cell.cpp)
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

// Memory management functions
void* gdstk_allocate(uint64_t size) {
    return gdstk::allocate<uint8_t>(size);
}

void* gdstk_reallocate(void* ptr, uint64_t size) {
    return gdstk::reallocate(ptr, size);
}

void gdstk_free(void* ptr) {
    if (ptr) {
        gdstk::free_allocation(ptr);
    }
}

} // extern "C"
