#include "vec.h"
#include "../include/gdstk/vec.hpp"
#include <cstdio>

// Thin wrapper around GDSTK C++ Vec2 functions

// Creation functions
Vec2 vec2_new(double x, double y) {
    Vec2 result = {x, y};
    return result;
}

Vec2 vec2_zero(void) {
    Vec2 result = {0.0, 0.0};
    return result;
}

// Comparison operations (matches C++ operators)
bool vec2_equal(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    return ga == gb;
}

bool vec2_not_equal(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    return ga != gb;
}

bool vec2_less_than(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    return ga < gb;
}

// Arithmetic operations (matches C++ operators)
Vec2 vec2_add(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    gdstk::Vec2 result = ga + gb;
    return {result.x, result.y};
}

Vec2 vec2_subtract(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    gdstk::Vec2 result = ga - gb;
    return {result.x, result.y};
}

Vec2 vec2_multiply_scalar(Vec2 v, double s) {
    gdstk::Vec2 gv = {v.x, v.y};
    gdstk::Vec2 result = gv * s;
    return {result.x, result.y};
}

Vec2 vec2_divide_scalar(Vec2 v, double s) {
    gdstk::Vec2 gv = {v.x, v.y};
    gdstk::Vec2 result = gv / s;
    return {result.x, result.y};
}

Vec2 vec2_negate(Vec2 v) {
    gdstk::Vec2 gv = {v.x, v.y};
    gdstk::Vec2 result = -gv;
    return {result.x, result.y};
}

// Geometric operations (matches C++ member functions)
double vec2_length_squared(Vec2 v) {
    gdstk::Vec2 gv = {v.x, v.y};
    return gv.length_sq();
}

double vec2_length(Vec2 v) {
    gdstk::Vec2 gv = {v.x, v.y};
    return gv.length();
}

Vec2 vec2_normalize(Vec2 v) {
    gdstk::Vec2 gv = {v.x, v.y};
    gdstk::Vec2 result = gv.normalize();
    return {result.x, result.y};
}

double vec2_angle(Vec2 v) {
    gdstk::Vec2 gv = {v.x, v.y};
    return gv.angle();
}

// Vector products (matches C++ functions)
double vec2_dot(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    return ga.dot(gb);
}

double vec2_cross(Vec2 a, Vec2 b) {
    gdstk::Vec2 ga = {a.x, a.y};
    gdstk::Vec2 gb = {b.x, b.y};
    return ga.cross(gb);
}

// Distance functions
double vec2_distance(Vec2 a, Vec2 b) {
    return vec2_length(vec2_subtract(a, b));
}

double vec2_distance_squared(Vec2 a, Vec2 b) {
    return vec2_length_squared(vec2_subtract(a, b));
}

// Rotation functions
Vec2 vec2_rotate(Vec2 v, double angle) {
    gdstk::Vec2 gv = {v.x, v.y};
    gdstk::Vec2 result = gv.rotate(angle);
    return {result.x, result.y};
}

// Utility functions
Vec2 vec2_perpendicular(Vec2 v) {
    gdstk::Vec2 gv = {v.x, v.y};
    gdstk::Vec2 result = gv.ortho();
    return {result.x, result.y};
}

// Array element access
void vec2_set(Vec2* v, double x, double y) {
    if (v) {
        v->x = x;
        v->y = y;
    }
}

void vec2_print(Vec2 v) {
    printf("(%g, %g)", v.x, v.y);
}
