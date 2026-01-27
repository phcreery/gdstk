/*
Comprehensive example demonstrating the GDSTK C bindings
This example shows how to create a simple layout with polygons, labels, and cells
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef SIMPLE_VERSION
// Simple standalone version with mock implementations
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Mock definitions for standalone compilation
typedef struct Vec2 { double x, y; } Vec2;
typedef uint64_t Tag;
typedef struct Library { char* name; double unit, precision; } Library;
typedef struct Cell { char* name; } Cell;
typedef struct Polygon { Tag tag; } Polygon;
typedef struct Label { char* text; Vec2 origin; Tag tag; int anchor; } Label;
typedef struct Reference { Vec2 origin; double magnification, rotation; } Reference;
typedef struct Array { uint64_t size; } Array;

// Mock function implementations
Vec2 vec2_new(double x, double y) { Vec2 v = {x, y}; return v; }
Vec2 vec2_add(Vec2 a, Vec2 b) { Vec2 v = {a.x + b.x, a.y + b.y}; return v; }
double vec2_length(Vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }
double vec2_distance(Vec2 a, Vec2 b) { Vec2 d = {b.x - a.x, b.y - a.y}; return vec2_length(d); }
Tag make_tag(uint32_t layer, uint32_t type) { return ((uint64_t)type << 32) | layer; }
uint32_t get_layer(Tag tag) { return (uint32_t)tag; }
uint32_t get_type(Tag tag) { return (uint32_t)(tag >> 32); }
void set_layer(Tag* tag, uint32_t layer) { *tag = make_tag(layer, get_type(*tag)); }
void set_type(Tag* tag, uint32_t type) { *tag = make_tag(get_layer(*tag), type); }

Library* library_new(const char* name, double unit, double precision) {
    Library* lib = malloc(sizeof(Library));
    lib->name = strdup(name); lib->unit = unit; lib->precision = precision;
    return lib;
}
void library_free(Library* lib) { if (lib) { free(lib->name); free(lib); } }

Cell* cell_new(const char* name) { Cell* cell = malloc(sizeof(Cell)); cell->name = strdup(name); return cell; }
void cell_free(Cell* cell) { if (cell) { free(cell->name); free(cell); } }
void cell_bounding_box(Cell* cell, Vec2* min, Vec2* max) { min->x = 1; max->x = 0; } // empty

Polygon* polygon_rectangle(Vec2 c1, Vec2 c2, Tag tag) { Polygon* p = malloc(sizeof(Polygon)); p->tag = tag; return p; }
Polygon* polygon_cross(Vec2 center, double size, double width, Tag tag) { Polygon* p = malloc(sizeof(Polygon)); p->tag = tag; return p; }
void polygon_free(Polygon* p) { if (p) free(p); }
double polygon_area(const Polygon* p) { return 100.0; }  // mock value
double polygon_perimeter(const Polygon* p) { return 40.0; }  // mock value
bool polygon_contain(const Polygon* p, Vec2 point) { return true; }  // mock
void polygon_bounding_box(const Polygon* p, Vec2* min, Vec2* max) { *min = vec2_new(0,0); *max = vec2_new(10,10); }
void polygon_translate(Polygon* p, Vec2 v) { /* mock */ }

Label* label_new(const char* text) { Label* l = malloc(sizeof(Label)); l->text = strdup(text); l->anchor = 5; return l; }
void label_free(Label* l) { if (l) { free(l->text); free(l); } }

Reference* reference_new_cell(Cell* cell) { Reference* r = malloc(sizeof(Reference)); r->magnification = 1.0; return r; }
void reference_free(Reference* r) { if (r) free(r); }

Array* array_vec2_new(void) { Array* a = malloc(sizeof(Array)); a->size = 0; return a; }
void array_vec2_free(Array* a) { if (a) free(a); }
void array_vec2_append(Array* a, Vec2 item) { a->size++; }
uint64_t array_vec2_size(const Array* a) { return a->size; }
Vec2 array_vec2_get(const Array* a, uint64_t i) { return vec2_new(i*2, i*3); }  // mock data

void gdstk_init(void) { }
void gdstk_cleanup(void) { }

#define GDSTK_ANCHOR_O 5
#define M_PI 3.14159265358979323846

#else
// Full version with actual GDSTK
#include "gdstk.h"
#endif

int main() {
    printf("=== GDSTK C Bindings Comprehensive Example ===\n\n");
    
    // Initialize library
    gdstk_init();
    
    // Create a library
    Library* lib = library_new("example_lib", 1e-6, 1e-9);
    if (!lib) {
        printf("Failed to create library\n");
        return 1;
    }
    printf("Created library: %s\n", lib->name);
    printf("Unit: %g, Precision: %g\n", lib->unit, lib->precision);
    
    // Create a main cell
    Cell* main_cell = cell_new("main");
    if (!main_cell) {
        printf("Failed to create main cell\n");
        library_free(lib);
        return 1;
    }
    printf("Created main cell\n");
    
    // Create some polygons
    printf("\n--- Creating Polygons ---\n");
    
    // Rectangle
    Polygon* rect = polygon_rectangle(vec2_new(0, 0), vec2_new(20, 10), make_tag(1, 0));
    printf("Created rectangle: (0,0) to (20,10) on layer %u\n", get_layer(rect->tag));
    
    // Cross shape
    Polygon* cross = polygon_cross(vec2_new(50, 5), 15, 3, make_tag(2, 0));
    printf("Created cross at (50,5) on layer %u\n", get_layer(cross->tag));
    
    // Calculate polygon properties
    double rect_area = polygon_area(rect);
    double rect_perimeter = polygon_perimeter(rect);
    printf("Rectangle area: %.2f, perimeter: %.2f\n", rect_area, rect_perimeter);
    
    // Test point containment
    Vec2 test_point = vec2_new(10, 5);
    bool inside = polygon_contain(rect, test_point);
    printf("Point (%.1f,%.1f) is %s rectangle\n", 
           test_point.x, test_point.y, inside ? "inside" : "outside");
    
    // Transform a polygon
    polygon_translate(cross, vec2_new(10, 10));
    printf("Translated cross by (10,10)\n");
    
    // Create labels
    printf("\n--- Creating Labels ---\n");
    
    Label* title_label = label_new("GDSTK Example");
    title_label->origin = vec2_new(35, 25);
    title_label->tag = make_tag(255, 0);  // Text layer
    title_label->anchor = GDSTK_ANCHOR_O; // Center anchor
    printf("Created title label at (%.1f,%.1f)\n", 
           title_label->origin.x, title_label->origin.y);
    
    Label* info_label = label_new("Rectangle and Cross");
    info_label->origin = vec2_new(35, -5);
    info_label->tag = make_tag(255, 0);
    info_label->anchor = GDSTK_ANCHOR_O;
    printf("Created info label\n");
    
    // Create a subcell
    printf("\n--- Creating Subcell ---\n");
    
    Cell* sub_cell = cell_new("subcell");
    
    // Add a small rectangle to subcell
    Polygon* small_rect = polygon_rectangle(vec2_new(0, 0), vec2_new(5, 5), make_tag(3, 0));
    // Note: In a full implementation, you would add the polygon to the cell's array
    printf("Created subcell with small rectangle\n");
    
    // Create a reference to the subcell
    Reference* ref = reference_new_cell(sub_cell);
    ref->origin = vec2_new(80, 5);
    ref->magnification = 2.0;
    ref->rotation = M_PI / 4;  // 45 degrees
    printf("Created reference to subcell at (%.1f,%.1f), mag=%.1f, rot=%.1f°\n",
           ref->origin.x, ref->origin.y, ref->magnification, ref->rotation * 180 / M_PI);
    
    // Calculate bounding boxes
    printf("\n--- Bounding Box Calculations ---\n");
    
    Vec2 rect_min, rect_max;
    polygon_bounding_box(rect, &rect_min, &rect_max);
    printf("Rectangle bbox: (%.1f,%.1f) to (%.1f,%.1f)\n",
           rect_min.x, rect_min.y, rect_max.x, rect_max.y);
    
    Vec2 cross_min, cross_max;
    polygon_bounding_box(cross, &cross_min, &cross_max);
    printf("Cross bbox: (%.1f,%.1f) to (%.1f,%.1f)\n",
           cross_min.x, cross_min.y, cross_max.x, cross_max.y);
    
    Vec2 main_min, main_max;
    cell_bounding_box(main_cell, &main_min, &main_max);
    if (main_min.x > main_max.x) {
        printf("Main cell is empty\n");
    } else {
        printf("Main cell bbox: (%.1f,%.1f) to (%.1f,%.1f)\n",
               main_min.x, main_min.y, main_max.x, main_max.y);
    }
    
    // Demonstrate utility functions
    printf("\n--- Utility Functions ---\n");
    
    // Vec2 math
    Vec2 v1 = vec2_new(3, 4);
    Vec2 v2 = vec2_new(1, 1);
    Vec2 sum = vec2_add(v1, v2);
    double length = vec2_length(v1);
    double distance = vec2_distance(v1, v2);
    printf("Vec2 math: (%.1f,%.1f) + (%.1f,%.1f) = (%.1f,%.1f)\n",
           v1.x, v1.y, v2.x, v2.y, sum.x, sum.y);
    printf("Length of (%.1f,%.1f) = %.2f\n", v1.x, v1.y, length);
    printf("Distance between vectors = %.2f\n", distance);
    
    // Tag operations
    Tag demo_tag = make_tag(10, 5);
    printf("Tag for layer %u, type %u = %lu\n", 
           get_layer(demo_tag), get_type(demo_tag), demo_tag);
    
    set_layer(&demo_tag, 15);
    set_type(&demo_tag, 8);
    printf("Modified tag: layer %u, type %u\n", 
           get_layer(demo_tag), get_type(demo_tag));
    
    // Demonstrate arrays
    printf("\n--- Array Operations ---\n");
    
    Array* points = array_vec2_new();
    array_vec2_append(points, vec2_new(0, 0));
    array_vec2_append(points, vec2_new(10, 0));
    array_vec2_append(points, vec2_new(10, 10));
    array_vec2_append(points, vec2_new(0, 10));
    
    printf("Created array with %lu points:\n", array_vec2_size(points));
    for (uint64_t i = 0; i < array_vec2_size(points); i++) {
        Vec2 p = array_vec2_get(points, i);
        printf("  Point %lu: (%.1f,%.1f)\n", i, p.x, p.y);
    }
    
    // File I/O demonstration (would work with full implementation)
    printf("\n--- File I/O (Interface Demo) ---\n");
    printf("Would write to GDSII file: library_write_gds(lib, \"example.gds\", 4000, NULL)\n");
    printf("Would write to SVG file: library_write_svg(lib, \"example.svg\", 1.0, 6, ...)\n");
    
    // Clean up
    printf("\n--- Cleanup ---\n");
    
    array_vec2_free(points);
    reference_free(ref);
    polygon_free(small_rect);
    cell_free(sub_cell);
    label_free(info_label);
    label_free(title_label);
    polygon_free(cross);
    polygon_free(rect);
    cell_free(main_cell);
    library_free(lib);
    
    printf("All objects freed successfully\n");
    
    gdstk_cleanup();
    printf("\nExample completed successfully!\n");
    
    return 0;
}
