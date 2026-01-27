#include "utils.h"
#include "../include/gdstk/utils.hpp"
#include <cmath>

// Vec2 utility functions
Vec2 vec2_new(double x, double y) {
    Vec2 result = {x, y};
    return result;
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 result = {a.x + b.x, a.y + b.y};
    return result;
}

Vec2 vec2_subtract(Vec2 a, Vec2 b) {
    Vec2 result = {a.x - b.x, a.y - b.y};
    return result;
}

Vec2 vec2_multiply(Vec2 v, double scalar) {
    Vec2 result = {v.x * scalar, v.y * scalar};
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
    if (len > 0.0) {
        Vec2 result = {v.x / len, v.y / len};
        return result;
    }
    Vec2 result = {0, 0};
    return result;
}

double vec2_distance(Vec2 a, Vec2 b) {
    return vec2_length(vec2_subtract(a, b));
}

double vec2_distance_squared(Vec2 a, Vec2 b) {
    return vec2_length_squared(vec2_subtract(a, b));
}

bool vec2_equal(Vec2 a, Vec2 b, double tolerance) {
    return vec2_distance_squared(a, b) <= tolerance * tolerance;
}

// Array utility functions
Array* array_new(uint64_t initial_capacity) {
    gdstk::Array<void*>* cpp_array = new gdstk::Array<void*>();
    if (initial_capacity > 0) {
        cpp_array->ensure_slots(initial_capacity);
    }
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
        cpp_array->count = 0;
    }
}

void array_append(Array* array, void* item) {
    if (array) {
        gdstk::Array<void*>* cpp_array = reinterpret_cast<gdstk::Array<void*>*>(array);
        cpp_array->append(item);
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

uint64_t array_size(const Array* array) {
    if (array) {
        const gdstk::Array<void*>* cpp_array = reinterpret_cast<const gdstk::Array<void*>*>(array);
        return cpp_array->count;
    }
    return 0;
}

// Utility functions
bool approx_equal(double a, double b, double tolerance) {
    return fabs(a - b) <= tolerance;
}

bool points_equal(Vec2 a, Vec2 b) {
    return vec2_equal(a, b, 1e-12);
}

// Basic memory allocation
void* gdstk_allocate(uint64_t size) {
    return malloc(size);
}

void gdstk_deallocate(void* ptr) {
    free(ptr);
}

// Map functions (simplified implementation - just placeholders)
Map* map_new() {
    return nullptr; // Simplified - not implementing full map
}

void map_free(Map* map) {
    (void)map; // Unused
}

void map_clear(Map* map) {
    (void)map; // Unused
}

bool map_has_key(const Map* map, const char* key) {
    (void)map; (void)key; // Unused
    return false;
}

void* map_get(const Map* map, const char* key) {
    (void)map; (void)key; // Unused
    return nullptr;
}

void map_set(Map* map, const char* key, void* value) {
    (void)map; (void)key; (void)value; // Unused
}

void map_remove(Map* map, const char* key) {
    (void)map; (void)key; // Unused
}

uint64_t map_size(const Map* map) {
    (void)map; // Unused
    return 0;
}

// Set functions (simplified implementation - just placeholders)
Set* set_new() {
    return nullptr; // Simplified - not implementing full set
}

void set_free(Set* set) {
    (void)set; // Unused
}

void set_clear(Set* set) {
    (void)set; // Unused
}

void set_add(Set* set, uint64_t value) {
    (void)set; (void)value; // Unused
}

bool set_has(const Set* set, uint64_t value) {
    (void)set; (void)value; // Unused
    return false;
}

void set_remove(Set* set, uint64_t value) {
    (void)set; (void)value; // Unused
}

uint64_t set_size(const Set* set) {
    (void)set; // Unused
    return 0;
}
