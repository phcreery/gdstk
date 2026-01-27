#ifndef GDSTK_C_HEADER_FLEXPATH
#define GDSTK_C_HEADER_FLEXPATH

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Vec2 Vec2;
typedef struct Array Array;
typedef struct Curve Curve;
typedef struct Polygon Polygon;
typedef struct Repetition Repetition;
typedef struct Property Property;
typedef struct RaithData RaithData;
typedef uint64_t Tag;
typedef enum ErrorCode ErrorCode;

// Join types for path elements
typedef enum {
    GDSTK_JOIN_MITER = 0,
    GDSTK_JOIN_BEVEL,
    GDSTK_JOIN_ROUND,
    GDSTK_JOIN_FUNCTION
} JoinType;

// End types for path elements
typedef enum {
    GDSTK_END_FLUSH = 0,
    GDSTK_END_EXTENDED,
    GDSTK_END_ROUND,
    GDSTK_END_FUNCTION
} EndType;

// Bend types for path elements
typedef enum {
    GDSTK_BEND_NONE = 0,
    GDSTK_BEND_CIRCULAR,
    GDSTK_BEND_FUNCTION
} BendType;

// Function pointer types
typedef Array* (*JoinFunction)(Vec2 first_point, Vec2 first_direction, 
                               Vec2 second_point, Vec2 second_direction, 
                               Vec2 center, double width, void* data);
typedef Array* (*EndFunction)(Vec2 first_point, Vec2 first_direction,
                              Vec2 second_point, Vec2 second_direction, void* data);
typedef Array* (*BendFunction)(Vec2 radius_vector, double initial_angle,
                               double final_angle, Vec2 center, void* data);

// FlexPath element structure
typedef struct FlexPathElement {
    Tag tag;
    Array* half_width_and_offset;  // Array of Vec2
    JoinType join_type;
    JoinFunction join_function;
    void* join_function_data;
    EndType end_type;
    Vec2 end_extensions;
    EndFunction end_function;
    void* end_function_data;
    BendType bend_type;
    double bend_radius;
    BendFunction bend_function;
    void* bend_function_data;
} FlexPathElement;

// FlexPath structure
typedef struct FlexPath {
    Curve* spine;
    FlexPathElement* elements;  // Array with count num_elements
    uint64_t num_elements;
    bool simple_path;
    bool scale_width;
    Repetition* repetition;
    Property* properties;
    RaithData* raith_data;
    void* owner;  // For Python interface
} FlexPath;

// FlexPath creation functions
FlexPath* flexpath_new(Vec2 initial_point, double width, Tag tag);
FlexPath* flexpath_new_multi(Vec2 initial_point, const double* widths, 
                            const double* offsets, const Tag* tags, uint64_t num_paths);
void flexpath_free(FlexPath* flexpath);
void flexpath_clear(FlexPath* flexpath);
void flexpath_print(const FlexPath* flexpath, bool all);

// Copy function
void flexpath_copy_from(FlexPath* flexpath, const FlexPath* source);

// Element management
void flexpath_add_element(FlexPath* flexpath, double width, double offset, Tag tag);
void flexpath_remove_element(FlexPath* flexpath, uint64_t index);
uint64_t flexpath_num_elements(const FlexPath* flexpath);

// Element property setters
void flexpath_set_element_width(FlexPath* flexpath, uint64_t element_index, double width);
void flexpath_set_element_offset(FlexPath* flexpath, uint64_t element_index, double offset);
void flexpath_set_element_tag(FlexPath* flexpath, uint64_t element_index, Tag tag);

// Joint and end properties
void flexpath_set_join_type(FlexPath* flexpath, uint64_t element_index, JoinType join_type);
void flexpath_set_end_type(FlexPath* flexpath, uint64_t element_index, EndType end_type);
void flexpath_set_bend_type(FlexPath* flexpath, uint64_t element_index, BendType bend_type);
void flexpath_set_bend_radius(FlexPath* flexpath, uint64_t element_index, double radius);

// Path building functions
void flexpath_segment(FlexPath* flexpath, Vec2 end_point, const double* widths, 
                     const double* offsets, bool relative);
void flexpath_arc(FlexPath* flexpath, double radius, double initial_angle, 
                 double final_angle, double rotation, const double* widths, 
                 const double* offsets);
void flexpath_turn(FlexPath* flexpath, double radius, double angle, 
                  const double* widths, const double* offsets);

// Bezier curve functions
void flexpath_bezier(FlexPath* flexpath, const Array* control_points,
                    const double* widths, const double* offsets, bool relative);
void flexpath_quadratic(FlexPath* flexpath, Vec2 control_point, Vec2 end_point,
                       const double* widths, const double* offsets, bool relative);
void flexpath_cubic(FlexPath* flexpath, Vec2 control1, Vec2 control2, Vec2 end_point,
                   const double* widths, const double* offsets, bool relative);

// Interpolating curves
void flexpath_interpolation(FlexPath* flexpath, const Array* point_array, 
                           const double* angles, bool* angle_constraints,
                           const Array* tension_array, double initial_curl, 
                           double final_curl, bool cycle, const double* widths, 
                           const double* offsets, bool relative);

// Smooth curves
void flexpath_smooth(FlexPath* flexpath, const Array* point_array, 
                    const Array* angle_array, Array* tension_array,
                    double initial_curl, double final_curl, bool cycle,
                    const double* widths, const double* offsets, bool relative);

// Geometry properties  
double flexpath_area(const FlexPath* flexpath);
double flexpath_length(const FlexPath* flexpath);
void flexpath_bounding_box(const FlexPath* flexpath, Vec2* min, Vec2* max);

// Path properties
Vec2 flexpath_position(const FlexPath* flexpath, double length);
Vec2 flexpath_gradient(const FlexPath* flexpath, double length, bool from_below);

// Transformation functions
void flexpath_translate(FlexPath* flexpath, Vec2 v);
void flexpath_scale(FlexPath* flexpath, double scaling, Vec2 center);
void flexpath_mirror(FlexPath* flexpath, Vec2 p0, Vec2 p1);
void flexpath_rotate(FlexPath* flexpath, double angle, Vec2 center);
void flexpath_transform(FlexPath* flexpath, double magnification, bool x_reflection,
                       double rotation, Vec2 origin);

// Conversion functions
ErrorCode flexpath_to_polygons(const FlexPath* flexpath, bool filter, Tag tag, 
                              Array* result);
void flexpath_apply_repetition(FlexPath* flexpath, Array* result);

// Output functions
ErrorCode flexpath_to_gds(const FlexPath* flexpath, FILE* out, double scaling);
ErrorCode flexpath_to_svg(const FlexPath* flexpath, FILE* out, double scaling, 
                         uint32_t precision);

// FlexPathElement utility functions
FlexPathElement* flexpath_element_new(Tag tag, double width, double offset);
void flexpath_element_free(FlexPathElement* element);
void flexpath_element_set_width_at_point(FlexPathElement* element, uint64_t point_index, 
                                         double width);
void flexpath_element_set_offset_at_point(FlexPathElement* element, uint64_t point_index, 
                                          double offset);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_FLEXPATH
