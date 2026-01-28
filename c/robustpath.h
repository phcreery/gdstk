#ifndef GDSTK_C_HEADER_ROBUSTPATH
#define GDSTK_C_HEADER_ROBUSTPATH

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

// Interpolation types (matches C++ InterpolationType enum)
typedef enum {
    GDSTK_INTERPOLATION_CONSTANT = 0,  // Step-change in join region
    GDSTK_INTERPOLATION_LINEAR,        // LERP from past value to new
    GDSTK_INTERPOLATION_SMOOTH,        // SERP from past value to new
    GDSTK_INTERPOLATION_PARAMETRIC     // Uses function
} InterpolationType;

// Parametric function types
typedef double (*ParametricDouble)(double t, void* data);
typedef Vec2 (*ParametricVec2)(double t, void* data);

// Interpolation structure (matches C++ Interpolation struct)
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

// SubPath types (matches C++ SubPathType enum)
typedef enum {
    GDSTK_SUBPATH_SEGMENT = 0,    // straight line segment
    GDSTK_SUBPATH_ARC,            // elliptical arc
    GDSTK_SUBPATH_BEZIER,         // general Bézier
    GDSTK_SUBPATH_BEZIER2,        // quadratic Bézier
    GDSTK_SUBPATH_BEZIER3,        // cubic Bézier
    GDSTK_SUBPATH_PARAMETRIC      // general parametric function
} SubPathType;

// SubPath structure (matches C++ SubPath struct)
typedef struct SubPath {
    SubPathType type;
    void* data;  // Opaque pointer to implementation-specific data
} SubPath;

// RobustPath element structure (matches C++ RobustPathElement struct)
typedef struct RobustPathElement {
    Tag tag;
    Array* width_array;    // Array of Interpolation (should match subpath_array count)
    Array* offset_array;   // Array of Interpolation (should match subpath_array count)
    double end_width;      // Last width value used in construction
    double end_offset;     // Last offset value used in construction
    EndType end_type;
    Vec2 end_extensions;
    EndFunction end_function;
    void* end_function_data;  // User data for end_function
} RobustPathElement;

// RobustPath structure (C wrapper for C++ RobustPath)
typedef struct RobustPath {
    Vec2 end_point;                // Last point on the path
    Array* subpath_array;          // Array of SubPath (path spine)
    RobustPathElement* elements;   // Array with count num_elements
    uint64_t num_elements;
    double tolerance;              // Numeric tolerance for intersections
    uint64_t max_evals;           // Maximal number of evaluations per function
    double width_scale;           // Width scale from transforms
    double offset_scale;          // Offset scale from transforms
    double trafo[6];              // Transformation matrix
    bool simple_path;             // If true, treat as constant width paths
    bool scale_width;             // Whether width scales with transformations
    Repetition* repetition;       // Repetition data
    Property* properties;
    void* owner;                  // For Python interface
} RobustPath;

// RobustPath creation functions (matches C++ init overloads)
RobustPath* robustpath_new(Vec2 initial_position, double width, double offset, 
                          double tolerance, uint64_t max_evals, Tag tag);
RobustPath* robustpath_new_multi(Vec2 initial_position, const double* widths, 
                                const double* offsets, double tolerance, uint64_t max_evals, 
                                const Tag* tags, uint64_t num_elements);
RobustPath* robustpath_new_separated(Vec2 initial_position, uint64_t num_elements, 
                                   double width, double separation, double tolerance, 
                                   uint64_t max_evals, Tag tag);
RobustPath* robustpath_new_multi_separated(Vec2 initial_position, uint64_t num_elements, 
                                          const double* widths, const double* offsets, 
                                          double tolerance, uint64_t max_evals, const Tag* tags);
void robustpath_free(RobustPath* robustpath);
void robustpath_clear(RobustPath* robustpath);
void robustpath_print(const RobustPath* robustpath, bool all);

// Copy function
void robustpath_copy_from(RobustPath* robustpath, const RobustPath* source);

// Path building functions (matches C++ API)
void robustpath_horizontal(RobustPath* robustpath, double coord_x, const Interpolation* widths, 
                          const Interpolation* offsets, bool relative);
void robustpath_vertical(RobustPath* robustpath, double coord_y, const Interpolation* widths, 
                        const Interpolation* offsets, bool relative);
void robustpath_segment(RobustPath* robustpath, Vec2 end_point, const Interpolation* widths, 
                       const Interpolation* offsets, bool relative);
void robustpath_cubic(RobustPath* robustpath, Vec2 point1, Vec2 point2, Vec2 point3,
                     const Interpolation* widths, const Interpolation* offsets, bool relative);
void robustpath_cubic_smooth(RobustPath* robustpath, Vec2 point2, Vec2 point3,
                            const Interpolation* widths, const Interpolation* offsets, bool relative);
void robustpath_quadratic(RobustPath* robustpath, Vec2 point1, Vec2 point2,
                         const Interpolation* widths, const Interpolation* offsets, bool relative);
void robustpath_quadratic_smooth(RobustPath* robustpath, Vec2 point2,
                                const Interpolation* widths, const Interpolation* offsets, bool relative);
void robustpath_bezier(RobustPath* robustpath, const Array* point_array,
                      const Interpolation* widths, const Interpolation* offsets, bool relative);
void robustpath_interpolation(RobustPath* robustpath, const Array* point_array, double* angles, 
                             bool* angle_constraints, Vec2* tension, double initial_curl, 
                             double final_curl, bool cycle, const Interpolation* widths, 
                             const Interpolation* offsets, bool relative);
void robustpath_arc(RobustPath* robustpath, double radius_x, double radius_y, double initial_angle, 
                   double final_angle, double rotation, const Interpolation* widths, 
                   const Interpolation* offsets);
void robustpath_turn(RobustPath* robustpath, double radius, double angle, 
                    const Interpolation* widths, const Interpolation* offsets);
void robustpath_parametric(RobustPath* robustpath, ParametricVec2 curve_function, void* func_data,
                          ParametricVec2 curve_gradient, void* grad_data, const Interpolation* widths, 
                          const Interpolation* offsets, bool relative);
uint64_t robustpath_commands(RobustPath* robustpath, const CurveInstruction* items, uint64_t count);

// Position and gradient functions (matches C++ API)
Vec2 robustpath_position(const RobustPath* robustpath, double u, bool from_below);
Vec2 robustpath_gradient(const RobustPath* robustpath, double u, bool from_below);

// Width and offset functions (matches C++ API)
void robustpath_width(const RobustPath* robustpath, double u, bool from_below, double* result);
void robustpath_offset(const RobustPath* robustpath, double u, bool from_below, double* result);

// Spine and element center functions
ErrorCode robustpath_spine(const RobustPath* robustpath, Array* result);
ErrorCode robustpath_element_center(const RobustPath* robustpath, const RobustPathElement* element, 
                                   Array* result);

// Transformation functions (matches C++ API)
void robustpath_translate(RobustPath* robustpath, Vec2 v);
void robustpath_scale(RobustPath* robustpath, double scale, Vec2 center);
void robustpath_mirror(RobustPath* robustpath, Vec2 p0, Vec2 p1);
void robustpath_rotate(RobustPath* robustpath, double angle, Vec2 center);
void robustpath_transform(RobustPath* robustpath, double magnification, bool x_reflection,
                         double rotation, Vec2 origin);

// Conversion functions
ErrorCode robustpath_to_polygons(const RobustPath* robustpath, bool filter, Tag tag, 
                                Array* result);
void robustpath_apply_repetition(const RobustPath* robustpath, Array* result);

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
