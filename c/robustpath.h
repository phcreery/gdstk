#ifndef GDSTK_C_HEADER_ROBUSTPATH
#define GDSTK_C_HEADER_ROBUSTPATH

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Vec2 Vec2;
typedef struct Array Array;
typedef struct Polygon Polygon;
typedef struct Repetition Repetition;
typedef struct Property Property;
typedef uint64_t Tag;
typedef enum ErrorCode ErrorCode;

// Interpolation types
typedef enum {
    GDSTK_INTERPOLATION_CONSTANT = 0,  // Step-change in join region
    GDSTK_INTERPOLATION_LINEAR,        // LERP from past value to new
    GDSTK_INTERPOLATION_SMOOTH,        // SERP from past value to new
    GDSTK_INTERPOLATION_PARAMETRIC     // Uses function
} InterpolationType;

// Parametric function types
typedef double (*ParametricDouble)(double t, void* data);
typedef Vec2 (*ParametricVec2)(double t, void* data);

// Interpolation structure
typedef struct Interpolation {
    InterpolationType type;
    union {
        double value;  // Constant
        struct {       // Linear or smooth interpolation
            double initial_value;
            double final_value;
        };
        struct {
            ParametricDouble function;  // Parametric
            void* data;                // User data
        };
    };
} Interpolation;

// SubPath types
typedef enum {
    GDSTK_SUBPATH_SEGMENT,    // straight line segment
    GDSTK_SUBPATH_ARC,        // elliptical arc
    GDSTK_SUBPATH_BEZIER,     // general Bézier
    GDSTK_SUBPATH_BEZIER2,    // quadratic Bézier
    GDSTK_SUBPATH_BEZIER3,    // cubic Bézier
    GDSTK_SUBPATH_PARAMETRIC  // general parametric function
} SubPathType;

// SubPath structure
typedef struct SubPath {
    SubPathType type;
    void* data;  // Opaque pointer to implementation-specific data
} SubPath;

// RobustPath element structure
typedef struct RobustPathElement {
    Tag tag;
    Interpolation* width_interpolation;
    Interpolation* offset_interpolation;
    void* end_function;    // EndFunction pointer
    void* end_data;        // User data for end function
    void* join_function;   // JoinFunction pointer  
    void* join_data;       // User data for join function
    double bend_radius;
    double bend_tolerance;
} RobustPathElement;

// RobustPath structure
typedef struct RobustPath {
    Array* subpath_array;    // Array of SubPath*
    Array* element_array;    // Array of RobustPathElement*
    double tolerance;
    double max_evals;
    double width_scale;
    double offset_scale;
    bool trafo_applied;
    Repetition* repetition;
    Property* properties;
    void* owner;  // For Python interface
} RobustPath;

// RobustPath creation functions
RobustPath* robustpath_new(Vec2 initial_point, double width, Tag tag);
void robustpath_free(RobustPath* robustpath);
void robustpath_clear(RobustPath* robustpath);
void robustpath_print(const RobustPath* robustpath, bool all);

// Copy function
void robustpath_copy_from(RobustPath* robustpath, const RobustPath* source);

// Element management
void robustpath_add_element(RobustPath* robustpath, double width, Tag tag);
void robustpath_remove_element(RobustPath* robustpath, uint64_t index);
uint64_t robustpath_num_elements(const RobustPath* robustpath);

// Path building functions
void robustpath_segment(RobustPath* robustpath, Vec2 end_point, 
                       const double* widths, const double* offsets, bool relative);
void robustpath_arc(RobustPath* robustpath, double radius, double initial_angle, 
                   double final_angle, double rotation, const double* widths, 
                   const double* offsets);
void robustpath_turn(RobustPath* robustpath, double radius, double angle, 
                    const double* widths, const double* offsets);
void robustpath_parametric(RobustPath* robustpath, ParametricVec2 curve_function, 
                          void* curve_data, const double* widths, const double* offsets,
                          bool relative);

// Bezier curve functions
void robustpath_bezier(RobustPath* robustpath, const Array* control_points,
                      const double* widths, const double* offsets, bool relative);
void robustpath_quadratic(RobustPath* robustpath, Vec2 control_point, Vec2 end_point,
                         const double* widths, const double* offsets, bool relative);
void robustpath_cubic(RobustPath* robustpath, Vec2 control1, Vec2 control2, Vec2 end_point,
                     const double* widths, const double* offsets, bool relative);

// Smooth curves
void robustpath_smooth(RobustPath* robustpath, const Array* point_array, 
                      const Array* angle_array, Array* tension_array,
                      double initial_curl, double final_curl, bool cycle,
                      const double* widths, const double* offsets, bool relative);

// Interpolation functions
void robustpath_set_width(RobustPath* robustpath, uint64_t element_index, double width);
void robustpath_set_offset(RobustPath* robustpath, uint64_t element_index, double offset);
void robustpath_set_width_interpolation(RobustPath* robustpath, uint64_t element_index, 
                                       const Interpolation* interpolation);
void robustpath_set_offset_interpolation(RobustPath* robustpath, uint64_t element_index, 
                                        const Interpolation* interpolation);

// Geometry properties
double robustpath_area(const RobustPath* robustpath);
double robustpath_length(const RobustPath* robustpath);
void robustpath_bounding_box(const RobustPath* robustpath, Vec2* min, Vec2* max);

// Spine functions
ErrorCode robustpath_spine(const RobustPath* robustpath, Array* result);
Vec2 robustpath_position(const RobustPath* robustpath, double length);
Vec2 robustpath_gradient(const RobustPath* robustpath, double length, bool from_below);

// Transformation functions
void robustpath_translate(RobustPath* robustpath, Vec2 v);
void robustpath_scale(RobustPath* robustpath, double scaling, Vec2 center);
void robustpath_mirror(RobustPath* robustpath, Vec2 p0, Vec2 p1);
void robustpath_rotate(RobustPath* robustpath, double angle, Vec2 center);
void robustpath_transform(RobustPath* robustpath, double magnification, bool x_reflection,
                         double rotation, Vec2 origin);

// Conversion functions
ErrorCode robustpath_to_polygons(const RobustPath* robustpath, bool filter, Tag tag, 
                                Array* result);
void robustpath_apply_repetition(RobustPath* robustpath, Array* result);

// Output functions
ErrorCode robustpath_to_gds(const RobustPath* robustpath, FILE* out, double scaling);
ErrorCode robustpath_to_svg(const RobustPath* robustpath, FILE* out, double scaling, 
                           uint32_t precision);

// Interpolation utility functions
Interpolation* interpolation_constant(double value);
Interpolation* interpolation_linear(double initial_value, double final_value);
Interpolation* interpolation_smooth(double initial_value, double final_value);
Interpolation* interpolation_parametric(ParametricDouble function, void* data);
void interpolation_free(Interpolation* interp);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_ROBUSTPATH
