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

// Point append functions (direct point array manipulation)
void curve_append(Curve* curve, Vec2 point) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_point;
        cpp_point.x = point.x;
        cpp_point.y = point.y;
        cpp_curve->append(cpp_point);
    }
}

void curve_append_unsafe(Curve* curve, Vec2 point) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 cpp_point;
        cpp_point.x = point.x;
        cpp_point.y = point.y;
        cpp_curve->append_unsafe(cpp_point);
    }
}

void curve_remove(Curve* curve, uint64_t index) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->remove(index);
    }
}

void curve_ensure_slots(Curve* curve, uint64_t free_slots) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->ensure_slots(free_slots);
    }
}

// Basic curve building - single coordinate versions
void curve_horizontal(Curve* curve, double coord_x, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->horizontal(coord_x, relative);
    }
}

void curve_vertical(Curve* curve, double coord_y, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->vertical(coord_y, relative);
    }
}

void curve_segment(Curve* curve, Vec2 end_point, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 end_pos;
        end_pos.x = end_point.x;
        end_pos.y = end_point.y;
        cpp_curve->segment(end_pos, relative);
    }
}

// Basic curve building - array coordinate versions
void curve_horizontal_array(Curve* curve, const Array* coord_x, bool relative) {
    if (curve && coord_x) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<double>* cpp_coords = reinterpret_cast<const gdstk::Array<double>*>(coord_x);
        cpp_curve->horizontal(*cpp_coords, relative);
    }
}

void curve_vertical_array(Curve* curve, const Array* coord_y, bool relative) {
    if (curve && coord_y) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<double>* cpp_coords = reinterpret_cast<const gdstk::Array<double>*>(coord_y);
        cpp_curve->vertical(*cpp_coords, relative);
    }
}

void curve_segment_array(Curve* curve, const Array* points, bool relative) {
    if (curve && points) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        cpp_curve->segment(*cpp_points, relative);
    }
}

// Arc functions (matches C++ API signature: radius_x, radius_y, initial_angle, final_angle, rotation)
void curve_arc(Curve* curve, double radius_x, double radius_y, double initial_angle, double final_angle, double rotation) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        cpp_curve->arc(radius_x, radius_y, initial_angle, final_angle, rotation);
    }
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

// Cubic Bezier (array of points: every 3 points define a section)
void curve_cubic(Curve* curve, const Array* points, bool relative) {
    if (curve && points) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = 
            reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        cpp_curve->cubic(*cpp_points, relative);
    }
}

// Cubic smooth Bezier (array of points: every 2 points define a section)
void curve_cubic_smooth(Curve* curve, const Array* points, bool relative) {
    if (curve && points) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = 
            reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        cpp_curve->cubic_smooth(*cpp_points, relative);
    }
}

// Quadratic Bezier (array of points: every 2 points define a section)
void curve_quadratic(Curve* curve, const Array* points, bool relative) {
    if (curve && points) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = 
            reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        cpp_curve->quadratic(*cpp_points, relative);
    }
}

// Quadratic smooth Bezier - single point
void curve_quadratic_smooth(Curve* curve, Vec2 end_point, bool relative) {
    if (curve) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        gdstk::Vec2 end_pos;
        end_pos.x = end_point.x;
        end_pos.y = end_point.y;
        cpp_curve->quadratic_smooth(end_pos, relative);
    }
}

// Quadratic smooth Bezier - array of points
void curve_quadratic_smooth_array(Curve* curve, const Array* points, bool relative) {
    if (curve && points) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = 
            reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        cpp_curve->quadratic_smooth(*cpp_points, relative);
    }
}

// Interpolation function
void curve_interpolation(Curve* curve, const Array* points, double* angles, bool* angle_constraints,
                        Vec2* tension, double initial_curl, double final_curl, bool cycle, bool relative) {
    if (curve && points && angles && angle_constraints && tension) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::Array<gdstk::Vec2>* cpp_points = 
            reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        
        // Convert Vec2 array to gdstk::Vec2 array for tension
        gdstk::Vec2* cpp_tension = reinterpret_cast<gdstk::Vec2*>(tension);
        
        cpp_curve->interpolation(*cpp_points, angles, angle_constraints, cpp_tension,
                               initial_curl, final_curl, cycle, relative);
    }
}

// Parametric function
void curve_parametric(Curve* curve, ParametricVec2 curve_function, void* data, bool relative) {
    if (curve && curve_function) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        // The function pointer types are compatible since they have the same signature
        gdstk::ParametricVec2 cpp_function = reinterpret_cast<gdstk::ParametricVec2>(curve_function);
        cpp_curve->parametric(cpp_function, data, relative);
    }
}

// Commands function - process array of CurveInstruction
uint64_t curve_commands(Curve* curve, const CurveInstruction* items, uint64_t count) {
    if (curve && items) {
        gdstk::Curve* cpp_curve = reinterpret_cast<gdstk::Curve*>(curve);
        const gdstk::CurveInstruction* cpp_items = 
            reinterpret_cast<const gdstk::CurveInstruction*>(items);
        return cpp_curve->commands(cpp_items, count);
    }
    return 0;
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

// Point array access
Array* curve_get_point_array(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        // Note: This is somewhat dangerous as it exposes internal data
        // But it matches the API expectation. The returned array should be treated as read-only.
        return const_cast<Array*>(reinterpret_cast<const Array*>(&cpp_curve->point_array));
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

// Curve closed check (matches the actual gdstk::Curve::closed() method)
bool curve_is_closed(const Curve* curve) {
    if (curve) {
        const gdstk::Curve* cpp_curve = reinterpret_cast<const gdstk::Curve*>(curve);
        return cpp_curve->closed();
    }
    return false;
}

} // extern "C"