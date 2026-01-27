#include "polygon.h"
#include "../include/gdstk/polygon.hpp"
#include "../include/gdstk/utils.hpp"
#include <cstdlib>
#include <cstring>
#include <new>

// Polygon C wrapper functions

Polygon* polygon_new(const Vec2* points, uint64_t num_points, uint32_t layer, uint32_t datatype) {
    if (!points || num_points == 0) return nullptr;
    
    gdstk::Polygon* cpp_polygon = static_cast<gdstk::Polygon*>(malloc(sizeof(gdstk::Polygon)));
    if (!cpp_polygon) return nullptr;
    
    // Initialize the C++ object in-place
    new(cpp_polygon) gdstk::Polygon();
    
    // Set tag (Tag is just uint64_t, pack layer and datatype)
    cpp_polygon->tag = ((uint64_t)layer << 16) | datatype;
    
    // Set points
    cpp_polygon->point_array.ensure_slots(num_points);
    for (uint64_t i = 0; i < num_points; i++) {
        gdstk::Vec2 point = {points[i].x, points[i].y};
        cpp_polygon->point_array.append(point);
    }
    
    return reinterpret_cast<Polygon*>(cpp_polygon);
}

void polygon_free(Polygon* polygon) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->~Polygon(); // Call destructor
        free(cpp_polygon);
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

uint32_t polygon_layer(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return (uint32_t)(cpp_polygon->tag >> 16);
    }
    return 0;
}

uint32_t polygon_datatype(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return (uint32_t)(cpp_polygon->tag & 0xFFFF);
    }
    return 0;
}

void polygon_set_layer(Polygon* polygon, uint32_t layer) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->tag = (cpp_polygon->tag & 0xFFFF) | ((uint64_t)layer << 16);
    }
}

void polygon_set_datatype(Polygon* polygon, uint32_t datatype) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_polygon->tag = (cpp_polygon->tag & 0xFFFF0000ULL) | datatype;
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

double polygon_area(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->area();
    }
    return 0.0;
}

double polygon_signed_area(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return cpp_polygon->signed_area();
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

bool polygon_contain(const Polygon* polygon, Vec2 point) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_point = {point.x, point.y};
        return cpp_polygon->contain(cpp_point);
    }
    return false;
}

bool polygon_contain_all(const Polygon* polygon, const Array* points) {
    if (polygon && points) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        const gdstk::Array<gdstk::Vec2>* cpp_points = reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        return cpp_polygon->contain_all(*cpp_points);
    }
    return false;
}

bool polygon_contain_any(const Polygon* polygon, const Array* points) {
    if (polygon && points) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        const gdstk::Array<gdstk::Vec2>* cpp_points = reinterpret_cast<const gdstk::Array<gdstk::Vec2>*>(points);
        return cpp_polygon->contain_any(*cpp_points);
    }
    return false;
}

void polygon_bounding_box(const Polygon* polygon, Vec2* min, Vec2* max) {
    if (polygon && min && max) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_min, cpp_max;
        cpp_polygon->bounding_box(cpp_min, cpp_max);
        min->x = cpp_min.x;
        min->y = cpp_min.y;
        max->x = cpp_max.x;
        max->y = cpp_max.y;
    }
}

void polygon_translate(Polygon* polygon, Vec2 v) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_vec = {v.x, v.y};
        cpp_polygon->translate(cpp_vec);
    }
}

void polygon_scale(Polygon* polygon, Vec2 scale, Vec2 center) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_scale = {scale.x, scale.y};
        gdstk::Vec2 cpp_center = {center.x, center.y};
        cpp_polygon->scale(cpp_scale, cpp_center);
    }
}

void polygon_mirror(Polygon* polygon, Vec2 p0, Vec2 p1) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_p0 = {p0.x, p0.y};
        gdstk::Vec2 cpp_p1 = {p1.x, p1.y};
        cpp_polygon->mirror(cpp_p0, cpp_p1);
    }
}

void polygon_rotate(Polygon* polygon, double angle, Vec2 center) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_center = {center.x, center.y};
        cpp_polygon->rotate(angle, cpp_center);
    }
}

void polygon_transform(Polygon* polygon, double magnification, bool x_reflection,
                      double rotation, Vec2 origin) {
    if (polygon) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::Vec2 cpp_origin = {origin.x, origin.y};
        cpp_polygon->transform(magnification, x_reflection, rotation, cpp_origin);
    }
}

void polygon_fillet(Polygon* polygon, const Array* radii, double tolerance) {
    if (polygon && radii) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        const gdstk::Array<double>* cpp_radii = reinterpret_cast<const gdstk::Array<double>*>(radii);
        cpp_polygon->fillet(*cpp_radii, tolerance);
    }
}

void polygon_fracture(const Polygon* polygon, uint64_t max_points, double precision, Array* result) {
    if (polygon && result) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::Array<gdstk::Polygon*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Polygon*>*>(result);
        cpp_polygon->fracture(max_points, precision, *cpp_result);
    }
}

void polygon_apply_repetition(Polygon* polygon, Array* result) {
    if (polygon && result) {
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        gdstk::Array<gdstk::Polygon*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Polygon*>*>(result);
        cpp_polygon->apply_repetition(*cpp_result);
    }
}

ErrorCode polygon_to_gds(const Polygon* polygon, FILE* out, double scaling) {
    if (polygon && out) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::ErrorCode result = cpp_polygon->to_gds(out, scaling);
        return static_cast<ErrorCode>(result);
    }
    return GDSTK_FILE_ERROR;
}

ErrorCode polygon_to_oas(const Polygon* polygon, OasisStream* out, OasisState* state) {
    if (polygon && out && state) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::OasisStream* cpp_out = reinterpret_cast<gdstk::OasisStream*>(out);
        gdstk::OasisState* cpp_state = reinterpret_cast<gdstk::OasisState*>(state);
        gdstk::ErrorCode result = cpp_polygon->to_oas(*cpp_out, *cpp_state);
        return static_cast<ErrorCode>(result);
    }
    return GDSTK_FILE_ERROR;
}

ErrorCode polygon_to_svg(const Polygon* polygon, FILE* out, double scaling, uint32_t precision) {
    if (polygon && out) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        gdstk::ErrorCode result = cpp_polygon->to_svg(out, scaling, precision);
        return static_cast<ErrorCode>(result);
    }
    return GDSTK_FILE_ERROR;
}

Polygon* polygon_rectangle(Vec2 corner1, Vec2 corner2, Tag tag) {
    gdstk::Polygon* cpp_polygon = static_cast<gdstk::Polygon*>(malloc(sizeof(gdstk::Polygon)));
    if (!cpp_polygon) return nullptr;
    
    // Initialize the C++ object in-place
    new(cpp_polygon) gdstk::Polygon();
    
    gdstk::Vec2 cpp_corner1 = {corner1.x, corner1.y};
    gdstk::Vec2 cpp_corner2 = {corner2.x, corner2.y};
    
    // Create a rectangle
    cpp_polygon->point_array.ensure_slots(4);
    cpp_polygon->point_array.append({cpp_corner1.x, cpp_corner1.y});
    cpp_polygon->point_array.append({cpp_corner2.x, cpp_corner1.y});
    cpp_polygon->point_array.append({cpp_corner2.x, cpp_corner2.y});
    cpp_polygon->point_array.append({cpp_corner1.x, cpp_corner2.y});
    
    cpp_polygon->tag = tag;
    
    return reinterpret_cast<Polygon*>(cpp_polygon);
}

Polygon* polygon_cross(Vec2 center, double full_size, double arm_width, Tag tag) {
    gdstk::Polygon* cpp_polygon = static_cast<gdstk::Polygon*>(malloc(sizeof(gdstk::Polygon)));
    if (!cpp_polygon) return nullptr;
    
    // Initialize the C++ object in-place
    new(cpp_polygon) gdstk::Polygon();
    
    gdstk::Vec2 cpp_center = {center.x, center.y};
    
    // Create cross shape
    cpp_polygon->point_array.count = 0;
    double half_size = full_size / 2.0;
    double half_arm = arm_width / 2.0;
    
    // Build cross vertices (simplified version)
    cpp_polygon->point_array.ensure_slots(12);
    cpp_polygon->point_array.append({cpp_center.x - half_arm, cpp_center.y - half_size});
    cpp_polygon->point_array.append({cpp_center.x + half_arm, cpp_center.y - half_size});
    cpp_polygon->point_array.append({cpp_center.x + half_arm, cpp_center.y - half_arm});
    cpp_polygon->point_array.append({cpp_center.x + half_size, cpp_center.y - half_arm});
    cpp_polygon->point_array.append({cpp_center.x + half_size, cpp_center.y + half_arm});
    cpp_polygon->point_array.append({cpp_center.x + half_arm, cpp_center.y + half_arm});
    cpp_polygon->point_array.append({cpp_center.x + half_arm, cpp_center.y + half_size});
    cpp_polygon->point_array.append({cpp_center.x - half_arm, cpp_center.y + half_size});
    cpp_polygon->point_array.append({cpp_center.x - half_arm, cpp_center.y + half_arm});
    cpp_polygon->point_array.append({cpp_center.x - half_size, cpp_center.y + half_arm});
    cpp_polygon->point_array.append({cpp_center.x - half_size, cpp_center.y - half_arm});
    cpp_polygon->point_array.append({cpp_center.x - half_arm, cpp_center.y - half_arm});
    
    cpp_polygon->tag = tag;
    
    return reinterpret_cast<Polygon*>(cpp_polygon);
}

size_t polygon_point_count(const Polygon* polygon) {
    if (!polygon) return 0;
    gdstk::Polygon* cpp_polygon = (gdstk::Polygon*)polygon;
    return cpp_polygon->point_array.count;
}
