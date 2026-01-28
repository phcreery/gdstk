#ifndef GDSTK_C_HEADER_FLEXPATH
#define GDSTK_C_HEADER_FLEXPATH

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Curve Curve;
typedef struct Repetition Repetition;
typedef struct RaithData RaithData;

// Join types for path elements (matches C++ JoinType)
typedef enum {
    GDSTK_JOIN_NATURAL = 0,  // Only bevel acute joins
    GDSTK_JOIN_MITER,
    GDSTK_JOIN_BEVEL,
    GDSTK_JOIN_ROUND,
    GDSTK_JOIN_SMOOTH,       // Becomes Round if simple_path
    GDSTK_JOIN_FUNCTION      // Use join_function
} JoinType;

// End types for path elements (matches C++ EndType)
typedef enum {
    GDSTK_END_FLUSH = 0,
    GDSTK_END_ROUND,
    GDSTK_END_HALFWIDTH,
    GDSTK_END_EXTENDED,      // Use end_extensions
    GDSTK_END_SMOOTH,        // Becomes Round if simple_path
    GDSTK_END_FUNCTION       // Use end_function
} EndType;

// Bend types for path elements (matches C++ BendType)
typedef enum {
    GDSTK_BEND_NONE = 0,
    GDSTK_BEND_CIRCULAR,     // Use bend_radius
    GDSTK_BEND_FUNCTION      // Use bend_function
} BendType;

// Function pointer types (matches C++ API signatures)
typedef Vec2 (*ParametricVec2)(double, void*);
typedef Array* (*JoinFunction)(Vec2 first_point, Vec2 first_direction, 
                               Vec2 second_point, Vec2 second_direction, 
                               Vec2 center, double width, void* data);
typedef Array* (*EndFunction)(Vec2 first_point, Vec2 first_direction,
                              Vec2 second_point, Vec2 second_direction, void* data);
typedef Array* (*BendFunction)(double radius, double initial_angle,
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

// FlexPath structure (C wrapper for C++ FlexPath)
typedef struct FlexPath {
    Curve* spine;                // The spine curve
    FlexPathElement* elements;   // Array with count num_elements
    uint64_t num_elements;
    bool simple_path;            // If true, elements treated as constant width
    bool scale_width;            // Whether width scales with path transformations
    Repetition* repetition;      // Repetition data
    Property* properties;        // Properties list
    RaithData* raith_data;       // Raith-specific data
    void* owner;                 // For Python interface
} FlexPath;

// FlexPath creation functions (matches C++ init overloads)
FlexPath* flexpath_new(Vec2 initial_point, double width, double offset, double tolerance, Tag tag);
FlexPath* flexpath_new_multi(Vec2 initial_point, const double* widths, 
                            const double* offsets, double tolerance, const Tag* tags, uint64_t num_paths);
FlexPath* flexpath_new_separated(Vec2 initial_point, uint64_t num_elements, double width, 
                                double separation, double tolerance, Tag tag);
FlexPath* flexpath_new_multi_separated(Vec2 initial_point, uint64_t num_elements, const double* widths,
                                      const double* offsets, double tolerance, const Tag* tags);
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
void flexpath_horizontal(FlexPath* flexpath, double coord_x, const double* widths, 
                        const double* offsets, bool relative);
void flexpath_horizontal_array(FlexPath* flexpath, const Array* coord_x, const double* widths, 
                              const double* offsets, bool relative);
void flexpath_vertical(FlexPath* flexpath, double coord_y, const double* widths, 
                      const double* offsets, bool relative);
void flexpath_vertical_array(FlexPath* flexpath, const Array* coord_y, const double* widths, 
                            const double* offsets, bool relative);
void flexpath_segment(FlexPath* flexpath, Vec2 end_point, const double* widths, 
                     const double* offsets, bool relative);
void flexpath_segment_array(FlexPath* flexpath, const Array* point_array, const double* widths, 
                           const double* offsets, bool relative);
void flexpath_arc(FlexPath* flexpath, double radius_x, double radius_y, double initial_angle, 
                 double final_angle, double rotation, const double* widths, 
                 const double* offsets);
void flexpath_turn(FlexPath* flexpath, double radius, double angle, 
                  const double* widths, const double* offsets);

// Bezier curve functions
void flexpath_bezier(FlexPath* flexpath, const Array* control_points,
                    const double* widths, const double* offsets, bool relative);
void flexpath_cubic(FlexPath* flexpath, const Array* point_array,
                   const double* widths, const double* offsets, bool relative);
void flexpath_cubic_smooth(FlexPath* flexpath, const Array* point_array,
                          const double* widths, const double* offsets, bool relative);
void flexpath_quadratic(FlexPath* flexpath, const Array* point_array,
                       const double* widths, const double* offsets, bool relative);
void flexpath_quadratic_smooth(FlexPath* flexpath, Vec2 end_point,
                              const double* widths, const double* offsets, bool relative);
void flexpath_quadratic_smooth_array(FlexPath* flexpath, const Array* point_array,
                                    const double* widths, const double* offsets, bool relative);

// Advanced curve functions
void flexpath_interpolation(FlexPath* flexpath, const Array* point_array, 
                           double* angles, bool* angle_constraints,
                           Vec2* tension, double initial_curl, 
                           double final_curl, bool cycle, const double* widths, 
                           const double* offsets, bool relative);
void flexpath_parametric(FlexPath* flexpath, ParametricVec2 curve_function, void* data,
                        const double* widths, const double* offsets, bool relative);
uint64_t flexpath_commands(FlexPath* flexpath, const CurveInstruction* items, uint64_t count);

// Transformation functions (matches C++ API)
void flexpath_translate(FlexPath* flexpath, Vec2 v);
void flexpath_scale(FlexPath* flexpath, double scale, Vec2 center);
void flexpath_mirror(FlexPath* flexpath, Vec2 p0, Vec2 p1);
void flexpath_rotate(FlexPath* flexpath, double angle, Vec2 center);
void flexpath_transform(FlexPath* flexpath, double magnification, bool x_reflection,
                       double rotation, Vec2 origin);

// Conversion functions
ErrorCode flexpath_to_polygons(const FlexPath* flexpath, bool filter, Tag tag, 
                              Array* result);
ErrorCode flexpath_element_center(const FlexPath* flexpath, const FlexPathElement* element,
                                 Array* result);
void flexpath_apply_repetition(const FlexPath* flexpath, Array* result);

// Output functions
ErrorCode flexpath_to_gds(const FlexPath* flexpath, FILE* out, double scaling);
ErrorCode flexpath_to_svg(const FlexPath* flexpath, FILE* out, double scaling, 
                         uint32_t precision);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_FLEXPATH
