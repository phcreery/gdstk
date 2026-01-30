#ifndef GDSTK_C_HEADER_CURVE
#define GDSTK_C_HEADER_CURVE

#include "common.h"
#include "vec.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function pointer types for parametric curves
typedef Vec2 (*ParametricVec2)(double, void*);

// Curve structure
typedef struct Curve {
    Array* point_array;     // Array of Vec2
    double tolerance;
    Vec2 last_ctrl;         // Last Bézier control point
    void* owner;            // For Python interface
} Curve;

// Curve creation functions
Curve* curve_new(Vec2 initial_position, double tolerance);
void curve_free(Curve* curve);
void curve_clear(Curve* curve);
void curve_print(const Curve* curve, bool all);

// Copy function
void curve_copy_from(Curve* curve, const Curve* source);

// Point append functions (direct point array manipulation)
void curve_append(Curve* curve, Vec2 point);
void curve_append_unsafe(Curve* curve, Vec2 point);
void curve_remove(Curve* curve, uint64_t index);
void curve_ensure_slots(Curve* curve, uint64_t free_slots);

// Basic curve building - single coordinate versions
void curve_horizontal(Curve* curve, double coord_x, bool relative);
void curve_vertical(Curve* curve, double coord_y, bool relative);
void curve_segment(Curve* curve, Vec2 end_point, bool relative);

// Basic curve building - array coordinate versions
void curve_horizontal_array(Curve* curve, const Array* coord_x, bool relative);
void curve_vertical_array(Curve* curve, const Array* coord_y, bool relative);
void curve_segment_array(Curve* curve, const Array* points, bool relative);

// Arc functions (matches C++ API signature: radius_x, radius_y, initial_angle, final_angle, rotation)
void curve_arc(Curve* curve, double radius_x, double radius_y, double initial_angle, double final_angle, double rotation);

// Turn function (convenience for arcs)
void curve_turn(Curve* curve, double radius, double angle);

// Bezier curve functions
void curve_bezier(Curve* curve, const Array* control_points, bool relative);

// Cubic Bezier (array of points: every 3 points define a section)
void curve_cubic(Curve* curve, const Array* points, bool relative);

// Cubic smooth Bezier (array of points: every 2 points define a section)
void curve_cubic_smooth(Curve* curve, const Array* points, bool relative);

// Quadratic Bezier (array of points: every 2 points define a section)
void curve_quadratic(Curve* curve, const Array* points, bool relative);

// Quadratic smooth Bezier - single point
void curve_quadratic_smooth(Curve* curve, Vec2 end_point, bool relative);

// Quadratic smooth Bezier - array of points
void curve_quadratic_smooth_array(Curve* curve, const Array* points, bool relative);

// Interpolation function
void curve_interpolation(Curve* curve, const Array* points, double* angles, bool* angle_constraints,
                        Vec2* tension, double initial_curl, double final_curl, bool cycle, bool relative);

// Parametric function
void curve_parametric(Curve* curve, ParametricVec2 curve_function, void* data, bool relative);

// Commands function - process array of CurveInstruction
uint64_t curve_commands(Curve* curve, const CurveInstruction* items, uint64_t count);

// Curve properties
uint64_t curve_point_count(const Curve* curve);
Vec2 curve_get_point(const Curve* curve, uint64_t index);
void curve_set_point(Curve* curve, uint64_t index, Vec2 point);

Vec2 curve_first_point(const Curve* curve);
Vec2 curve_last_point(const Curve* curve);

// Point array access
Array* curve_get_point_array(const Curve* curve);

// Tolerance management
double curve_get_tolerance(const Curve* curve);
void curve_set_tolerance(Curve* curve, double tolerance);

// Curve status
bool curve_is_closed(const Curve* curve);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_CURVE
