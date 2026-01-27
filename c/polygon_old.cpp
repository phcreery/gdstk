/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "polygon.h"
#include "../include/gdstk/polygon.hpp"
#include "../include/gdstk/utils.hpp"

extern "C" {

// Forward declaration of error conversion function (defined in cell.cpp)
extern ErrorCode convert_error_code(gdstk::ErrorCode cpp_error);

// Polygon functions
Polygon* polygon_new(const Vec2* points, uint64_t num_points, uint32_t layer, uint32_t datatype) {
    if (!points || num_points == 0) return nullptr;
    
    gdstk::Polygon* cpp_polygon = new gdstk::Polygon();
    cpp_polygon->tag = gdstk::make_tag(layer, datatype);
    
    if (points && num_points > 0) {
        cpp_polygon->point_array.ensure_slots(num_points);
        for (uint64_t i = 0; i < num_points; i++) {
            cpp_polygon->point_array.append(*reinterpret_cast<const gdstk::Vec2*>(&points[i]));
        }
    }
    
    return reinterpret_cast<Polygon*>(cpp_polygon);
}

void polygon_free(Polygon* polygon) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->clear();
        delete cpp_polygon;
    }
}

void polygon_clear(Polygon* polygon) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->clear();
    }
}

void polygon_print(const Polygon* polygon, bool all) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        cpp_polygon->print(all);
    }
}

// Property access functions  
uint32_t polygon_layer(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return gdstk::get_layer(cpp_polygon->tag);
    }
    return 0;
}

uint32_t polygon_datatype(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return gdstk::get_type(cpp_polygon->tag);
    }
    return 0;
}

void polygon_set_layer(Polygon* polygon, uint32_t layer) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::set_layer(cpp_polygon->tag, layer);
    }
}

void polygon_set_datatype(Polygon* polygon, uint32_t datatype) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::set_type(cpp_polygon->tag, datatype);
    }
}

uint64_t polygon_size(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->point_array.count;
    }
    return 0;
}

Vec2* polygon_point_array(Polygon* polygon) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        return reinterpret_cast<Vec2*>(cpp_polygon->point_array.items);
    }
    return nullptr;
}

void polygon_copy_from(Polygon* polygon, const Polygon* source) {
    if (polygon && source) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        const gdstk::Polygon* cpp_source = reinterpret_cast<const gdstk::Polygon*>(source);
        cpp_polygon->copy_from(*cpp_source);
    }
}

void polygon_print(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        cpp_polygon->print();
    }
}

double polygon_area(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->area();
    }
    return 0.0;
}

double polygon_perimeter(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->perimeter();
    }
    return 0.0;
}

void polygon_bounding_box(const Polygon* polygon, Vec2* min, Vec2* max) {
    if (polygon && min && max) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_min, cpp_max;
        cpp_polygon->bounding_box(cpp_min, cpp_max);
        *reinterpret_cast<gdstk::Vec2*>(min) = cpp_min;
        *reinterpret_cast<gdstk::Vec2*>(max) = cpp_max;
    }
}

uint64_t polygon_size(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->point_array.count;
    }
    return 0;
}

Vec2* polygon_point_array(Polygon* polygon) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        return reinterpret_cast<Vec2*>(cpp_polygon->point_array.items);
    }
    return nullptr;
}

Tag polygon_tag(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->tag;
    }
    return 0;
}

void polygon_set_tag(Polygon* polygon, Tag tag) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->tag = tag;
    }
}

uint32_t polygon_layer(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return gdstk::get_layer(cpp_polygon->tag);
    }
    return 0;
}

uint32_t polygon_datatype(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return gdstk::get_type(cpp_polygon->tag);
    }
    return 0;
}

void polygon_set_layer(Polygon* polygon, uint32_t layer) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::set_layer(cpp_polygon->tag, layer);
    }
}

void polygon_set_datatype(Polygon* polygon, uint32_t datatype) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::set_type(cpp_polygon->tag, datatype);
    }
}

bool polygon_contain(const Polygon* polygon, Vec2 point) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->contain(*reinterpret_cast<const gdstk::Vec2*>(&point));
    }
    return false;
}

bool polygon_contain_all(const Polygon* polygon, const Vec2* points, uint64_t num_points) {
    if (polygon && points) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        for (uint64_t i = 0; i < num_points; i++) {
            if (!cpp_polygon->contain(*reinterpret_cast<const gdstk::Vec2*>(&points[i]))) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool polygon_contain_any(const Polygon* polygon, const Vec2* points, uint64_t num_points) {
    if (polygon && points) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        for (uint64_t i = 0; i < num_points; i++) {
            if (cpp_polygon->contain(*reinterpret_cast<const gdstk::Vec2*>(&points[i]))) {
                return true;
            }
        }
    }
    return false;
}

void polygon_translate(Polygon* polygon, Vec2 displacement) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->translate(*reinterpret_cast<const gdstk::Vec2*>(&displacement));
    }
}

void polygon_scale(Polygon* polygon, double scaling, Vec2 center) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->scale(scaling, *reinterpret_cast<const gdstk::Vec2*>(&center));
    }
}

void polygon_mirror(Polygon* polygon, Vec2 p1, Vec2 p2) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->mirror(*reinterpret_cast<const gdstk::Vec2*>(&p1), 
                           *reinterpret_cast<const gdstk::Vec2*>(&p2));
    }
}

void polygon_rotate(Polygon* polygon, double angle, Vec2 center) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->rotate(angle, *reinterpret_cast<const gdstk::Vec2*>(&center));
    }
}

void polygon_transform(Polygon* polygon, double magnification, bool x_reflection, double rotation, Vec2 origin) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->transform(magnification, x_reflection, rotation, 
                              *reinterpret_cast<const gdstk::Vec2*>(&origin));
    }
}

void polygon_apply_repetition(Array* result, const Polygon* polygon) {
    if (result && polygon) {
        gdstk::Array<gdstk::Polygon>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Polygon>*>(result);
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        cpp_polygon->apply_repetition(*cpp_result);
    }
}

ErrorCode polygon_to_gds(const Polygon* polygon, FILE* out, double scaling) {
    if (polygon && out) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::ErrorCode result = cpp_polygon->to_gds(out, scaling);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

ErrorCode polygon_to_svg(const Polygon* polygon, FILE* out, double scaling, uint32_t precision, const char* attributes) {
    if (polygon && out) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::ErrorCode result = cpp_polygon->to_svg(out, scaling, precision, attributes);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

} // extern "C"
