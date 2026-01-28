#ifndef GDSTK_C_HEADER_POLYGON
#define GDSTK_C_HEADER_POLYGON

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Additional forward declarations
typedef struct OasisStream OasisStream;
typedef struct OasisState OasisState;

// Polygon structure (C wrapper for C++ Polygon)
typedef struct Polygon {
    Tag tag;
    Array* point_array;  // Array of Vec2
    Repetition* repetition;
    Property* properties;
    void* owner;  // For Python interface
} Polygon;

// Polygon functions
Polygon* polygon_new(const Vec2* points, uint64_t num_points, Tag tag);
void polygon_free(Polygon* polygon);
void polygon_clear(Polygon* polygon);
void polygon_print(const Polygon* polygon, bool all);

// Copy function
void polygon_copy_from(Polygon* polygon, const Polygon* source);

// Area and perimeter functions
double polygon_area(const Polygon* polygon);
double polygon_signed_area(const Polygon* polygon);
double polygon_perimeter(const Polygon* polygon);

// Point containment functions
bool polygon_contain(const Polygon* polygon, Vec2 point);
bool polygon_contain_all(const Polygon* polygon, const Array* points);
bool polygon_contain_any(const Polygon* polygon, const Array* points);

// Bounding box function
void polygon_bounding_box(const Polygon* polygon, Vec2* min, Vec2* max);

// Transformation functions (matches C++ API)
void polygon_translate(Polygon* polygon, Vec2 v);
void polygon_scale(Polygon* polygon, Vec2 scale, Vec2 center);
void polygon_mirror(Polygon* polygon, Vec2 p0, Vec2 p1);
void polygon_rotate(Polygon* polygon, double angle, Vec2 center);
void polygon_transform(Polygon* polygon, double magnification, bool x_reflection, 
                      double rotation, Vec2 origin);

// Modification functions
void polygon_fillet(Polygon* polygon, const Array* radii, double tolerance);
void polygon_fracture(const Polygon* polygon, uint64_t max_points, double precision, 
                     Array* result);
void polygon_apply_repetition(Polygon* polygon, Array* result);

// Output functions
ErrorCode polygon_to_gds(const Polygon* polygon, FILE* out, double scaling);
ErrorCode polygon_to_oas(const Polygon* polygon, OasisStream* out, OasisState* state);
ErrorCode polygon_to_svg(const Polygon* polygon, FILE* out, double scaling, uint32_t precision);

// Factory functions (matches C++ API)
Polygon* polygon_rectangle(Vec2 corner1, Vec2 corner2, Tag tag);
Polygon* polygon_cross(Vec2 center, double full_size, double arm_width, Tag tag);
Polygon* polygon_regular(Vec2 center, double side_length, uint64_t sides, double rotation, Tag tag);
Polygon* polygon_ellipse(Vec2 center, double radius_x, double radius_y, double inner_radius_x,
                        double inner_radius_y, double initial_angle, double final_angle, 
                        double tolerance, Tag tag);
Polygon* polygon_racetrack(Vec2 center, double straight_length, double radius, double inner_radius,
                          bool vertical, double tolerance, Tag tag);

// Text function (creates multiple polygons)
void polygon_text(const char* text, double size, Vec2 position, bool vertical, Tag tag,
                 Array* result);

// Contour function
ErrorCode polygon_contour(const double* data, uint64_t rows, uint64_t cols, double level, 
                         double scaling, Array* result);

// Inside functions (for multiple polygons)
void polygon_inside(const Array* points, const Array* polygons, bool* result);
bool polygon_all_inside(const Array* points, const Array* polygons);
bool polygon_any_inside(const Array* points, const Array* polygons);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_POLYGON
