#ifndef GDSTK_C_HEADER_CURVE
#define GDSTK_C_HEADER_CURVE

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

// Basic curve building
void curve_segment(Curve* curve, Vec2 end_point, bool relative);
void curve_segment_to(Curve* curve, Vec2 end_point);
void curve_segment_by(Curve* curve, Vec2 offset);

// Arc functions
void curve_arc(Curve* curve, double radius, double initial_angle, double final_angle, 
              double rotation, bool relative);
void curve_arc_to(Curve* curve, double radius, double initial_angle, double final_angle, 
                 double rotation);
void curve_arc_by(Curve* curve, double radius, double initial_angle, double final_angle, 
                 double rotation);

// Turn function (convenience for arcs)
void curve_turn(Curve* curve, double radius, double angle);

// Bezier curve functions
void curve_bezier(Curve* curve, const Array* control_points, bool relative);
void curve_bezier_to(Curve* curve, const Array* control_points);
void curve_bezier_by(Curve* curve, const Array* control_points);

// Quadratic Bezier
void curve_quadratic(Curve* curve, Vec2 control_point, Vec2 end_point, bool relative);
void curve_quadratic_to(Curve* curve, Vec2 control_point, Vec2 end_point);
void curve_quadratic_by(Curve* curve, Vec2 control_point, Vec2 end_point);

// Cubic Bezier
void curve_cubic(Curve* curve, Vec2 control1, Vec2 control2, Vec2 end_point, bool relative);
void curve_cubic_to(Curve* curve, Vec2 control1, Vec2 control2, Vec2 end_point);
void curve_cubic_by(Curve* curve, Vec2 control1, Vec2 control2, Vec2 end_point);

// Smooth Bezier continuations
void curve_smooth_quadratic(Curve* curve, Vec2 end_point, bool relative);
void curve_smooth_quadratic_to(Curve* curve, Vec2 end_point);
void curve_smooth_quadratic_by(Curve* curve, Vec2 end_point);

void curve_smooth_cubic(Curve* curve, Vec2 control2, Vec2 end_point, bool relative);
void curve_smooth_cubic_to(Curve* curve, Vec2 control2, Vec2 end_point);
void curve_smooth_cubic_by(Curve* curve, Vec2 control2, Vec2 end_point);

// Interpolating curves
void curve_interpolation(Curve* curve, const Array* point_array, const double* angles, 
                        bool* angle_constraints, const Array* tension_array, 
                        double initial_curl, double final_curl, bool cycle, bool relative);

// Parametric curves
typedef Vec2 (*ParametricCurveFunction)(double t, void* data);
void curve_parametric(Curve* curve, ParametricCurveFunction curve_function, void* data,
                     bool relative);

// SVG-like path commands
void curve_commands(Curve* curve, const CurveInstruction* commands, uint64_t command_count);

// Curve properties
uint64_t curve_point_count(const Curve* curve);
Vec2 curve_get_point(const Curve* curve, uint64_t index);
void curve_set_point(Curve* curve, uint64_t index, Vec2 point);

Vec2 curve_first_point(const Curve* curve);
Vec2 curve_last_point(const Curve* curve);

// Curve geometry
double curve_length(const Curve* curve);
void curve_bounding_box(const Curve* curve, Vec2* min, Vec2* max);

// Curve position and derivatives
Vec2 curve_position_at_length(const Curve* curve, double length);
Vec2 curve_gradient_at_length(const Curve* curve, double length, bool from_below);

// Curve transformation
void curve_translate(Curve* curve, Vec2 offset);
void curve_scale(Curve* curve, Vec2 scale, Vec2 center);
void curve_rotate(Curve* curve, double angle, Vec2 center);
void curve_mirror(Curve* curve, Vec2 p0, Vec2 p1);
void curve_transform(Curve* curve, double magnification, bool x_reflection,
                    double rotation, Vec2 origin);

// Curve manipulation
void curve_reverse(Curve* curve);
void curve_close(Curve* curve);
bool curve_is_closed(const Curve* curve);

// Curve simplification
void curve_simplify(Curve* curve, double tolerance);
void curve_remove_duplicate_points(Curve* curve, double tolerance);

// Curve validation
bool curve_is_valid(const Curve* curve);
bool curve_has_self_intersections(const Curve* curve);

// Curve intersection
void curve_intersections(const Curve* curve1, const Curve* curve2, Array* result);

// Point array access
Array* curve_get_point_array(const Curve* curve);
void curve_set_point_array(Curve* curve, const Array* point_array);

// Tolerance management
double curve_get_tolerance(const Curve* curve);
void curve_set_tolerance(Curve* curve, double tolerance);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_CURVE
