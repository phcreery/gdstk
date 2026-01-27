#ifndef GDSTK_C_HEADER_VEC
#define GDSTK_C_HEADER_VEC

#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// Vec2 structure (already defined in utils.h, but complete definition here)
typedef struct Vec2 {
    union {
        struct {
            double x, y;
        };
        struct {
            double u, v;
        };
        struct {
            double re, im;
        };
        double e[2];
    };
} Vec2;

// Vec2 creation and basic operations
Vec2 vec2_new(double x, double y);
Vec2 vec2_zero(void);
Vec2 vec2_copy(Vec2 v);

// Comparison operations
bool vec2_equal(Vec2 a, Vec2 b);
bool vec2_not_equal(Vec2 a, Vec2 b);
bool vec2_less_than(Vec2 a, Vec2 b);
bool vec2_greater_than(Vec2 a, Vec2 b);
bool vec2_less_equal(Vec2 a, Vec2 b);
bool vec2_greater_equal(Vec2 a, Vec2 b);

// Arithmetic operations
Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_add_scalar(Vec2 v, double s);
Vec2 vec2_subtract(Vec2 a, Vec2 b);
Vec2 vec2_subtract_scalar(Vec2 v, double s);
Vec2 vec2_multiply(Vec2 a, Vec2 b);
Vec2 vec2_multiply_scalar(Vec2 v, double s);
Vec2 vec2_divide(Vec2 a, Vec2 b);
Vec2 vec2_divide_scalar(Vec2 v, double s);
Vec2 vec2_negate(Vec2 v);

// In-place arithmetic operations
void vec2_add_inplace(Vec2* v, Vec2 other);
void vec2_add_scalar_inplace(Vec2* v, double s);
void vec2_subtract_inplace(Vec2* v, Vec2 other);
void vec2_subtract_scalar_inplace(Vec2* v, double s);
void vec2_multiply_inplace(Vec2* v, Vec2 other);
void vec2_multiply_scalar_inplace(Vec2* v, double s);
void vec2_divide_inplace(Vec2* v, Vec2 other);
void vec2_divide_scalar_inplace(Vec2* v, double s);

// Geometric operations
double vec2_length_squared(Vec2 v);
double vec2_length(Vec2 v);
Vec2 vec2_normalize(Vec2 v);
double vec2_distance_squared(Vec2 a, Vec2 b);
double vec2_distance(Vec2 a, Vec2 b);
double vec2_angle(Vec2 v);
Vec2 vec2_from_angle(double angle);

// Vector products
double vec2_dot_product(Vec2 a, Vec2 b);
double vec2_cross_product(Vec2 a, Vec2 b);

// Rotation and transformation
Vec2 vec2_rotate(Vec2 v, double angle);
Vec2 vec2_rotate_around(Vec2 v, double angle, Vec2 center);
Vec2 vec2_reflect_x(Vec2 v);
Vec2 vec2_reflect_y(Vec2 v);
Vec2 vec2_reflect_origin(Vec2 v);

// Interpolation
Vec2 vec2_lerp(Vec2 a, Vec2 b, double t);
Vec2 vec2_smooth_step(Vec2 a, Vec2 b, double t);

// Utility functions
Vec2 vec2_perpendicular(Vec2 v);
Vec2 vec2_clamp(Vec2 v, Vec2 min_val, Vec2 max_val);
Vec2 vec2_min(Vec2 a, Vec2 b);
Vec2 vec2_max(Vec2 a, Vec2 b);
Vec2 vec2_abs(Vec2 v);
Vec2 vec2_floor(Vec2 v);
Vec2 vec2_ceil(Vec2 v);
Vec2 vec2_round(Vec2 v);

// Array element access
double vec2_get_x(Vec2 v);
double vec2_get_y(Vec2 v);
double vec2_get_element(Vec2 v, int index);
void vec2_set_x(Vec2* v, double x);
void vec2_set_y(Vec2* v, double y);
void vec2_set_element(Vec2* v, int index, double value);
void vec2_set(Vec2* v, double x, double y);

// String representation and debugging
void vec2_print(Vec2 v);
char* vec2_to_string(Vec2 v);  // Caller must free the returned string

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_VEC
