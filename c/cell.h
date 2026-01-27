#ifndef GDSTK_C_HEADER_CELL
#define GDSTK_C_HEADER_CELL

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// GeometryInfo structure for caching bounding box and convex hull results
typedef struct GeometryInfo {
    Array* convex_hull;      // Array of Vec2
    Vec2 bounding_box_min;
    Vec2 bounding_box_max;
    bool convex_hull_valid;
    bool bounding_box_valid;
} GeometryInfo;

// Cell structure
typedef struct Cell {
    char* name;
    
    // Element arrays
    Array* polygon_array;    // Array of Polygon*
    Array* reference_array;  // Array of Reference*
    Array* flexpath_array;   // Array of FlexPath*
    Array* robustpath_array; // Array of RobustPath*
    Array* label_array;      // Array of Label*
    
    Property* properties;
    void* owner;  // For Python interface
} Cell;

// Cell functions
Cell* cell_new(const char* name);
void cell_free(Cell* cell);
void cell_clear(Cell* cell);
void cell_print(const Cell* cell, bool all);

// Cell accessors
const char* cell_name(const Cell* cell);

// Copy function
Cell* cell_copy(const Cell* cell, const char* new_name, bool deep_copy);

// Element management
void cell_add_polygon(Cell* cell, Polygon* polygon);
void cell_add_flexpath(Cell* cell, FlexPath* flexpath);
void cell_add_robustpath(Cell* cell, RobustPath* robustpath);
void cell_add_reference(Cell* cell, Reference* reference);
void cell_add_label(Cell* cell, Label* label);

// Element removal
bool cell_remove_polygon(Cell* cell, Polygon* polygon);
bool cell_remove_flexpath(Cell* cell, FlexPath* flexpath);
bool cell_remove_robustpath(Cell* cell, RobustPath* robustpath);
bool cell_remove_reference(Cell* cell, Reference* reference);
bool cell_remove_label(Cell* cell, Label* label);

// Element count functions
size_t cell_polygon_count(const Cell* cell);
size_t cell_flexpath_count(const Cell* cell);
size_t cell_robustpath_count(const Cell* cell);
size_t cell_reference_count(const Cell* cell);
size_t cell_label_count(const Cell* cell);

// Element access functions
Polygon* cell_get_polygon(const Cell* cell, size_t index);
FlexPath* cell_get_flexpath(const Cell* cell, size_t index);
RobustPath* cell_get_robustpath(const Cell* cell, size_t index);
Reference* cell_get_reference(const Cell* cell, size_t index);
Label* cell_get_label(const Cell* cell, size_t index);

// Geometry functions
void cell_bounding_box(const Cell* cell, Vec2* min, Vec2* max);
double cell_area(const Cell* cell, bool by_spec, Map* result);
GeometryInfo* cell_bounding_box_cached(const Cell* cell, Map* cache);

// Convex hull functions
void cell_convex_hull(const Cell* cell, Array* result);
GeometryInfo* cell_convex_hull_cached(const Cell* cell, Map* cache);

// Copy function
void cell_copy_from(Cell* cell, const Cell* source, const char* new_name, bool deep_copy);

// Get elements functions
void cell_get_polygons(const Cell* cell, bool apply_repetitions, bool include_paths, 
                      int64_t depth, bool filter, Tag tag, Array* result);
void cell_get_flexpaths(const Cell* cell, bool apply_repetitions, int64_t depth, 
                       bool filter, Tag tag, Array* result);
void cell_get_robustpaths(const Cell* cell, bool apply_repetitions, int64_t depth, 
                         bool filter, Tag tag, Array* result);
void cell_get_labels(const Cell* cell, bool apply_repetitions, int64_t depth, 
                    bool filter, Tag tag, Array* result);

// Dependencies functions
void cell_get_dependencies(const Cell* cell, bool recursive, Map* result);
void cell_get_raw_dependencies(const Cell* cell, bool recursive, Map* result);

// Tags functions
void cell_get_shape_tags(const Cell* cell, Set* result);
void cell_get_label_tags(const Cell* cell, Set* result);

// Cell manipulation functions
void cell_flatten(Cell* cell, bool apply_repetitions, Array* removed_references);
void cell_remap_tags(Cell* cell, const TagMap* map);

// Output functions
ErrorCode cell_to_gds(const Cell* cell, FILE* out, double scaling, uint64_t max_points, 
                     double precision, const struct tm* timestamp);
ErrorCode cell_to_svg(const Cell* cell, FILE* out, double scaling, uint32_t precision, 
                     const char* attributes, PolygonComparisonFunction comp);
ErrorCode cell_write_svg(const Cell* cell, const char* filename, double scaling, 
                        uint32_t precision, StyleMap* shape_style, StyleMap* label_style, 
                        const char* background, double pad, bool pad_as_percentage, 
                        PolygonComparisonFunction comp);

// GeometryInfo functions
GeometryInfo* geometry_info_new(void);
void geometry_info_free(GeometryInfo* info);
void geometry_info_clear(GeometryInfo* info);

// Tag utility functions
Tag make_tag(uint32_t layer, uint32_t type);
uint32_t get_layer(Tag tag);
uint32_t get_type(Tag tag);
void set_layer(Tag* tag, uint32_t layer);
void set_type(Tag* tag, uint32_t type);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_CELL