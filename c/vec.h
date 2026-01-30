#ifndef GDSTK_C_HEADER_VEC
#define GDSTK_C_HEADER_VEC

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Vec2 structure (matches GDSTK C++ Vec2)
typedef struct Vec2 {
    union {
        struct {
            double x, y;
        };
        struct {
            double u, v;
        };
        double e[2];
    };
} Vec2;

// Creation functions
Vec2 vec2_new(double x, double y);
Vec2 vec2_zero(void);

// Comparison operations (matches C++ operators)
bool vec2_equal(Vec2 a, Vec2 b);
bool vec2_not_equal(Vec2 a, Vec2 b);
bool vec2_less_than(Vec2 a, Vec2 b);

// Arithmetic operations (matches C++ operators)
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_subtract(Vec2 a, Vec2 b);
Vec2 vec2_multiply_scalar(Vec2 v, double s);
Vec2 vec2_divide_scalar(Vec2 v, double s);
Vec2 vec2_negate(Vec2 v);

// Geometric operations (matches C++ member functions)
double vec2_length_squared(Vec2 v);
double vec2_length(Vec2 v);
Vec2 vec2_normalize(Vec2 v);
double vec2_angle(Vec2 v);

// Vector products (matches C++ functions)
double vec2_dot(Vec2 a, Vec2 b);
double vec2_cross(Vec2 a, Vec2 b);

// Distance functions
double vec2_distance(Vec2 a, Vec2 b);
double vec2_distance_squared(Vec2 a, Vec2 b);

// Rotation functions
Vec2 vec2_rotate(Vec2 v, double angle);

// Utility functions
Vec2 vec2_perpendicular(Vec2 v);

// Array element access
void vec2_set(Vec2* v, double x, double y);
void vec2_print(Vec2 v);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_VEC
