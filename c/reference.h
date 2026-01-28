#ifndef GDSTK_C_HEADER_REFERENCE
#define GDSTK_C_HEADER_REFERENCE

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations for types not in common.h
typedef struct Cell Cell;
typedef struct RawCell RawCell;
typedef struct GeometryInfo GeometryInfo;
typedef struct Repetition Repetition;
typedef struct Property Property;

// Reference type enumeration
typedef enum {
    GDSTK_REFERENCE_CELL = 0,
    GDSTK_REFERENCE_RAWCELL,
    GDSTK_REFERENCE_NAME
} ReferenceType;

// Reference structure (C wrapper for C++ Reference)
typedef struct Reference {
    ReferenceType type;
    union {
        Cell* cell;
        RawCell* rawcell;
        char* name;
    };
    Vec2 origin;
    double rotation;      // in radians
    double magnification;
    bool x_reflection;
    Repetition* repetition;
    Property* properties;
    void* owner;  // For Python interface
} Reference;

// Reference functions
Reference* reference_new_cell(Cell* cell);
Reference* reference_new_rawcell(RawCell* rawcell);
Reference* reference_new_name(const char* name);
void reference_free(Reference* reference);
void reference_clear(Reference* reference);
void reference_print(const Reference* reference);

// Copy function
void reference_copy_from(Reference* reference, const Reference* source);

// Bounding box functions
void reference_bounding_box(const Reference* reference, Vec2* min, Vec2* max);
GeometryInfo* reference_bounding_box_cached(const Reference* reference, Map* cache);

// Convex hull functions
void reference_convex_hull(const Reference* reference, Array* result);
GeometryInfo* reference_convex_hull_cached(const Reference* reference, Map* cache);

// Transformation functions
void reference_translate(Reference* reference, Vec2 v);
void reference_scale(Reference* reference, double factor, Vec2 center);
void reference_mirror(Reference* reference, Vec2 p0, Vec2 p1);
void reference_rotate(Reference* reference, double angle, Vec2 center);
void reference_transform(Reference* reference, double magnification, bool x_reflection,
                        double rotation, Vec2 origin);

// Element extraction functions
void reference_get_polygons(const Reference* reference, bool apply_repetitions, 
                           bool include_paths, int64_t depth, bool filter, 
                           uint64_t tag, Array* result);
void reference_get_flexpaths(const Reference* reference, bool apply_repetitions,
                            int64_t depth, bool filter, uint64_t tag, Array* result);
void reference_get_robustpaths(const Reference* reference, bool apply_repetitions,
                              int64_t depth, bool filter, uint64_t tag, Array* result);
void reference_get_labels(const Reference* reference, bool apply_repetitions,
                          int64_t depth, bool filter, uint64_t tag, Array* result);

// Apply repetition
void reference_apply_repetition(Reference* reference, Array* result);

// Output functions
ErrorCode reference_to_gds(const Reference* reference, FILE* out, double scaling);
ErrorCode reference_to_svg(const Reference* reference, FILE* out, double scaling, 
                           uint32_t precision);

// Reference properties
Vec2 reference_origin(const Reference* reference);
double reference_rotation(const Reference* reference);
double reference_magnification(const Reference* reference);

// Basic operations
Reference* reference_new(Cell* cell, Vec2 origin, double rotation, double magnification, 
                        bool x_reflection, Tag tag);

// Repetition management
void reference_set_repetition_rectangular(Reference* reference, uint64_t columns, 
                                         uint64_t rows, Vec2 spacing);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_REFERENCE
