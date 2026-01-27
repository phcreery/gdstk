/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "curve.h"
#include "../include/gdstk/curve.hpp"
#include "../include/gdstk/vec.hpp"
#include "../include/gdstk/array.hpp"

extern "C" {

// Curve creation functions
Curve* curve_new(Vec2 initial_position, double tolerance) {
    gdstk::Curve* cpp_curve = new gdstk::Curve();
    gdstk::Vec2 init_pos;
    init_pos.x = initial_position.x;
    init_pos.y = initial_position.y;
    cpp_curve->init(init_pos, tolerance);
    return reinterpret_cast<Curve*>(cpp_curve);
}

void curve_free(Curve* curve) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->clear();
        delete cpp_curve;
    }
}

void curve_clear(Curve* curve) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->clear();
    }
}

void curve_print(const Curve* curve, bool all) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        cpp_curve->print(all);
    }
}

// Copy function
void curve_copy_from(Curve* curve, const Curve* source) {
    if (curve && source) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Curve* cpp_source = reinterpret_cast<const gdstk::Curve*>(source);
        cpp_curve->copy_from(*cpp_source);
    }
}

// Basic curve building
void curve_segment(Curve* curve, Vec2 end_point, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 end_pos;
        end_pos.x = end_point.x;
        end_pos.y = end_point.y;
        cpp_curve->segment(end_pos, relative);
    }
}

void curve_segment_to(Curve* curve, Vec2 end_point) {
    curve_segment(curve, end_point, false);
}

void curve_segment_by(Curve* curve, Vec2 offset) {
    curve_segment(curve, offset, true);
}

// Arc functions
void curve_arc(Curve* curve, double radius, double initial_angle, double final_angle, 
              double rotation, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->arc(radius, initial_angle, final_angle, rotation, relative);
    }
}

void curve_arc_to(Curve* curve, double radius, double initial_angle, double final_angle, 
                 double rotation) {
    curve_arc(curve, radius, initial_angle, final_angle, rotation, false);
}

void curve_arc_by(Curve* curve, double radius, double initial_angle, double final_angle, 
                 double rotation) {
    curve_arc(curve, radius, initial_angle, final_angle, rotation, true);
}

// Turn function (convenience for arcs)
void curve_turn(Curve* curve, double radius, double angle) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->turn(radius, angle);
    }
}

// Bezier curve functions
void curve_bezier(Curve* curve, const Array* control_points, bool relative) {
    if (curve && control_points) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = 
            reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(control_points);
        cpp_curve->bezier(*cpp_points, relative);
    }
}

void curve_bezier_to(Curve* curve, const Array* control_points) {
    curve_bezier(curve, control_points, false);
}

void curve_bezier_by(Curve* curve, const Array* control_points) {
    curve_bezier(curve, control_points, true);
}

// Quadratic Bezier
void curve_quadratic(Curve* curve, Vec2 control_point, Vec2 end_point, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 ctrl_pos;
        ctrl_pos.x = control_point.x;
        ctrl_pos.y = control_point.y;
        gdstk::Vec2 end_pos;
        end_pos.x = end_point.x;
        end_pos.y = end_point.y;
        cpp_curve->quadratic(ctrl_pos, end_pos, relative);
    }
}

void curve_quadratic_to(Curve* curve, Vec2 control_point, Vec2 end_point) {
    curve_quadratic(curve, control_point, end_point, false);
}

void curve_quadratic_by(Curve* curve, Vec2 control_point, Vec2 end_point) {
    curve_quadratic(curve, control_point, end_point, true);
}

// Cubic Bezier
void curve_cubic(Curve* curve, Vec2 control1, Vec2 control2, Vec2 end_point, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 ctrl1_pos;
        ctrl1_pos.x = control1.x;
        ctrl1_pos.y = control1.y;
        gdstk::Vec2 ctrl2_pos;
        ctrl2_pos.x = control2.x;
        ctrl2_pos.y = control2.y;
        gdstk::Vec2 end_pos;
        end_pos.x = end_point.x;
        end_pos.y = end_point.y;
        cpp_curve->cubic(ctrl1_pos, ctrl2_pos, end_pos, relative);
    }
}

void curve_cubic_to(Curve* curve, Vec2 control1, Vec2 control2, Vec2 end_point) {
    curve_cubic(curve, control1, control2, end_point, false);
}

void curve_cubic_by(Curve* curve, Vec2 control1, Vec2 control2, Vec2 end_point) {
    curve_cubic(curve, control1, control2, end_point, true);
}

// Curve properties
uint64_t curve_point_count(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        return cpp_curve->point_array.count;
    }
    return 0;
}

Vec2 curve_get_point(const Curve* curve, uint64_t index) {
    Vec2 result = {0.0, 0.0};
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        if (index < cpp_curve->point_array.count) {
            const gdstk::Vec2& point = cpp_curve->point_array[index];
            result.x = point.x;
            result.y = point.y;
        }
    }
    return result;
}

void curve_set_point(Curve* curve, uint64_t index, Vec2 point) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        if (index < cpp_curve->point_array.count) {
            gdstk::Vec2& cpp_point = cpp_curve->point_array[index];
            cpp_point.x = point.x;
            cpp_point.y = point.y;
        }
    }
}

Vec2 curve_first_point(const Curve* curve) {
    return curve_get_point(curve, 0);
}

Vec2 curve_last_point(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        if (cpp_curve->point_array.count > 0) {
            return curve_get_point(curve, cpp_curve->point_array.count - 1);
        }
    }
    Vec2 result = {0.0, 0.0};
    return result;
}

// Curve geometry
double curve_length(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        return cpp_curve->length();
    }
    return 0.0;
}

void curve_bounding_box(const Curve* curve, Vec2* min, Vec2* max) {
    if (curve && min && max) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_min, cpp_max;
        cpp_curve->bounding_box(cpp_min, cpp_max);
        min->x = cpp_min.x;
        min->y = cpp_min.y;
        max->x = cpp_max.x;
        max->y = cpp_max.y;
    }
}

// Point array access
Array* curve_get_point_array(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        return reinterpret_cast<Array*>(&cpp_curve->point_array);
    }
    return nullptr;
}

// Tolerance management
double curve_get_tolerance(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        return cpp_curve->tolerance;
    }
    return 0.0;
}

void curve_set_tolerance(Curve* curve, double tolerance) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->tolerance = tolerance;
    }
}

// Curve transformation
void curve_translate(Curve* curve, Vec2 offset) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_offset;
        cpp_offset.x = offset.x;
        cpp_offset.y = offset.y;
        cpp_curve->translate(cpp_offset);
    }
}

void curve_scale(Curve* curve, Vec2 scale, Vec2 center) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_scale;
        cpp_scale.x = scale.x;
        cpp_scale.y = scale.y;
        gdstk::Vec2 cpp_center;
        cpp_center.x = center.x;
        cpp_center.y = center.y;
        cpp_curve->scale(cpp_scale, cpp_center);
    }
}

void curve_rotate(Curve* curve, double angle, Vec2 center) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_center;
        cpp_center.x = center.x;
        cpp_center.y = center.y;
        cpp_curve->rotate(angle, cpp_center);
    }
}

void curve_transform(Curve* curve, double magnification, bool x_reflection,
                    double rotation, Vec2 origin) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_origin;
        cpp_origin.x = origin.x;
        cpp_origin.y = origin.y;
        cpp_curve->transform(magnification, x_reflection, rotation, cpp_origin);
    }
}

// Basic curve manipulation - simplified implementations
void curve_reverse(Curve* curve) {
    // Placeholder - full implementation would reverse point order
}

void curve_close(Curve* curve) {
    // Placeholder - full implementation would add closing segment
}

bool curve_is_closed(const Curve* curve) {
    // Placeholder - full implementation would check if first == last point
    return false;
}

// Simplified/placeholder implementations for complex functions
void curve_smooth_quadratic(Curve* curve, Vec2 end_point, bool relative) {
    // Placeholder
}

void curve_smooth_quadratic_to(Curve* curve, Vec2 end_point) {
    curve_smooth_quadratic(curve, end_point, false);
}

void curve_smooth_quadratic_by(Curve* curve, Vec2 end_point) {
    curve_smooth_quadratic(curve, end_point, true);
}

void curve_smooth_cubic(Curve* curve, Vec2 control2, Vec2 end_point, bool relative) {
    // Placeholder
}

void curve_smooth_cubic_to(Curve* curve, Vec2 control2, Vec2 end_point) {
    curve_smooth_cubic(curve, control2, end_point, false);
}

void curve_smooth_cubic_by(Curve* curve, Vec2 control2, Vec2 end_point) {
    curve_smooth_cubic(curve, control2, end_point, true);
}

void curve_interpolation(Curve* curve, const Array* point_array, const double* angles, 
                        bool* angle_constraints, const Array* tension_array, 
                        double initial_curl, double final_curl, bool cycle, bool relative) {
    // Placeholder for complex interpolation
}

void curve_parametric(Curve* curve, ParametricCurveFunction curve_function, void* data,
                     bool relative) {
    // Placeholder for parametric curves
}

void curve_commands(Curve* curve, const CurveInstruction* commands, uint64_t command_count) {
    // Placeholder for SVG-like path commands
}

Vec2 curve_position_at_length(const Curve* curve, double length) {
    Vec2 result = {0.0, 0.0};
    // Placeholder for position calculation
    return result;
}

Vec2 curve_gradient_at_length(const Curve* curve, double length, bool from_below) {
    Vec2 result = {0.0, 0.0};
    // Placeholder for gradient calculation
    return result;
}

void curve_mirror(Curve* curve, Vec2 p0, Vec2 p1) {
    // Placeholder for mirror operation
}

void curve_simplify(Curve* curve, double tolerance) {
    // Placeholder for simplification
}

void curve_remove_duplicate_points(Curve* curve, double tolerance) {
    // Placeholder for duplicate removal
}

bool curve_is_valid(const Curve* curve) {
    return curve != nullptr;
}

bool curve_has_self_intersections(const Curve* curve) {
    // Placeholder for self-intersection check
    return false;
}

void curve_intersections(const Curve* curve1, const Curve* curve2, Array* result) {
    // Placeholder for intersection calculation
}

void curve_set_point_array(Curve* curve, const Array* point_array) {
    // Placeholder - would copy point array
}

} // extern "C"
