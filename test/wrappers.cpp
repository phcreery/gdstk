#include "../include/gdstk/cell.hpp"
#include "../include/gdstk/polygon.hpp"
#include "../include/gdstk/label.hpp"
#include "../include/gdstk/reference.hpp"
#include "../include/gdstk/utils.hpp"
#include "../c/common.h"
#include <cstdlib>
#include <cstring>
#include <new>
#include <cmath>

extern "C" {

// Cell functions
Cell* cell_new(const char* name) {
    gdstk::Cell* cpp_cell = static_cast<gdstk::Cell*>(malloc(sizeof(gdstk::Cell)));
    if (!cpp_cell) return nullptr;
    
    new(cpp_cell) gdstk::Cell();
    
    if (name) {
        cpp_cell->name = (char*)malloc(strlen(name) + 1);
        if (cpp_cell->name) {
            strcpy(cpp_cell->name, name);
        }
    }
    
    return reinterpret_cast<Cell*>(cpp_cell);
}

void cell_free(Cell* cell) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        cpp_cell->~Cell();
        free(cpp_cell);
    }
}

const char* cell_name(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return cpp_cell->name;
    }
    return nullptr;
}

size_t cell_polygon_count(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return (size_t)cpp_cell->polygon_array.count;
    }
    return 0;
}

size_t cell_label_count(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return (size_t)cpp_cell->label_array.count;
    }
    return 0;
}

size_t cell_reference_count(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return (size_t)cpp_cell->reference_array.count;
    }
    return 0;
}

void cell_add_polygon(Cell* cell, Polygon* polygon) {
    if (cell && polygon) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
        cpp_cell->polygon_array.append(cpp_polygon);
    }
}

void cell_add_label(Cell* cell, Label* label) {
    if (cell && label) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_cell->label_array.append(cpp_label);
    }
}

void cell_add_reference(Cell* cell, Reference* reference) {
    if (cell && reference) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_cell->reference_array.append(cpp_reference);
    }
}

Polygon* cell_get_polygon(const Cell* cell, size_t index) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        if (index < cpp_cell->polygon_array.count) {
            return reinterpret_cast<Polygon*>(cpp_cell->polygon_array.items[index]);
        }
    }
    return nullptr;
}

Label* cell_get_label(const Cell* cell, size_t index) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        if (index < cpp_cell->label_array.count) {
            return reinterpret_cast<Label*>(cpp_cell->label_array.items[index]);
        }
    }
    return nullptr;
}

Cell* cell_copy(const Cell* cell, const char* new_name, bool deep_copy) {
    if (!cell) return nullptr;
    
    // Create a new cell with the same contents
    Cell* new_cell = cell_new(new_name);
    if (!new_cell) return nullptr;
    
    const gdstk::Cell* cpp_source = reinterpret_cast<const gdstk::Cell*>(cell);
    gdstk::Cell* cpp_dest = reinterpret_cast<gdstk::Cell*>(new_cell);
    
    // Simple shallow copy - just copy arrays
    for (uint64_t i = 0; i < cpp_source->polygon_array.count; i++) {
        cpp_dest->polygon_array.append(cpp_source->polygon_array[i]);
    }
    for (uint64_t i = 0; i < cpp_source->label_array.count; i++) {
        cpp_dest->label_array.append(cpp_source->label_array[i]);
    }
    for (uint64_t i = 0; i < cpp_source->reference_array.count; i++) {
        cpp_dest->reference_array.append(cpp_source->reference_array[i]);
    }
    
    return new_cell;
}

bool cell_remove_polygon(Cell* cell, Polygon* polygon) {
    if (!cell || !polygon) return false;
    
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
    
    for (uint64_t i = 0; i < cpp_cell->polygon_array.count; i++) {
        if (cpp_cell->polygon_array[i] == cpp_polygon) {
            cpp_cell->polygon_array.remove(i);
            return true;
        }
    }
    return false;
}

bool cell_remove_label(Cell* cell, Label* label) {
    if (!cell || !label) return false;
    
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
    
    for (uint64_t i = 0; i < cpp_cell->label_array.count; i++) {
        if (cpp_cell->label_array[i] == cpp_label) {
            cpp_cell->label_array.remove(i);
            return true;
        }
    }
    return false;
}

void cell_bounding_box(const Cell* cell, Vec2* min, Vec2* max) {
    if (!cell || !min || !max) return;
    
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    
    // Simple bounding box calculation
    if (cpp_cell->polygon_array.count == 0) {
        min->x = min->y = max->x = max->y = 0.0;
        return;
    }
    
    min->x = min->y = 1e30;
    max->x = max->y = -1e30;
    
    for (uint64_t i = 0; i < cpp_cell->polygon_array.count; i++) {
        gdstk::Polygon* poly = cpp_cell->polygon_array[i];
        for (uint64_t j = 0; j < poly->point_array.count; j++) {
            gdstk::Vec2& pt = poly->point_array[j];
            if (pt.x < min->x) min->x = pt.x;
            if (pt.y < min->y) min->y = pt.y;
            if (pt.x > max->x) max->x = pt.x;
            if (pt.y > max->y) max->y = pt.y;
        }
    }
}

double cell_area(const Cell* cell, bool by_spec, Map* result) {
    if (!cell) return 0.0;
    
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    
    double total_area = 0.0;
    for (uint64_t i = 0; i < cpp_cell->polygon_array.count; i++) {
        gdstk::Polygon* poly = cpp_cell->polygon_array[i];
        // Simple polygon area calculation using shoelace formula
        double area = 0.0;
        uint64_t n = poly->point_array.count;
        if (n >= 3) {
            for (uint64_t j = 0; j < n; j++) {
                uint64_t k = (j + 1) % n;
                gdstk::Vec2& p1 = poly->point_array[j];
                gdstk::Vec2& p2 = poly->point_array[k];
                area += p1.x * p2.y - p2.x * p1.y;
            }
            area = fabs(area) * 0.5;
        }
        total_area += area;
    }
    
    return total_area;
}

void cell_flatten(Cell* cell, bool apply_repetitions, Array* removed_references) {
    if (!cell) return;
    
    // Simple stub - just clear references without actually flattening
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    cpp_cell->reference_array.clear();
}

// Polygon functions
Polygon* polygon_new(const Vec2* points, uint64_t num_points, uint32_t layer, uint32_t datatype) {
    if (!points || num_points == 0) return nullptr;
    
    gdstk::Polygon* cpp_polygon = static_cast<gdstk::Polygon*>(malloc(sizeof(gdstk::Polygon)));
    if (!cpp_polygon) return nullptr;
    
    new(cpp_polygon) gdstk::Polygon();
    
    cpp_polygon->tag = ((uint64_t)layer << 16) | datatype;
    
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
        cpp_polygon->~Polygon();
        free(cpp_polygon);
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

size_t polygon_point_count(const Polygon* polygon) {
    if (polygon) {
        const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
        return (size_t)cpp_polygon->point_array.count;
    }
    return 0;
}

uint64_t polygon_size(const Polygon* polygon) {
    return polygon_point_count(polygon);
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

double polygon_area(const Polygon* polygon) {
    if (!polygon) return 0.0;
    
    const gdstk::Polygon* cpp_polygon = reinterpret_cast<const gdstk::Polygon*>(polygon);
    
    // Simple polygon area calculation using shoelace formula
    double area = 0.0;
    uint64_t n = cpp_polygon->point_array.count;
    if (n >= 3) {
        for (uint64_t i = 0; i < n; i++) {
            uint64_t j = (i + 1) % n;
            const gdstk::Vec2& p1 = cpp_polygon->point_array[i];
            const gdstk::Vec2& p2 = cpp_polygon->point_array[j];
            area += p1.x * p2.y - p2.x * p1.y;
        }
        area = fabs(area) * 0.5;
    }
    return area;
}

// Label functions
Label* label_new(const char* text, Vec2 origin, double rotation, uint32_t layer, uint32_t texttype) {
    gdstk::Label* cpp_label = static_cast<gdstk::Label*>(malloc(sizeof(gdstk::Label)));
    if (!cpp_label) return nullptr;
    
    new(cpp_label) gdstk::Label();
    
    if (text) {
        cpp_label->text = (char*)malloc(strlen(text) + 1);
        if (cpp_label->text) {
            strcpy(cpp_label->text, text);
        }
    }
    
    cpp_label->origin.x = origin.x;
    cpp_label->origin.y = origin.y;
    cpp_label->rotation = rotation;
    cpp_label->tag = ((uint64_t)layer << 16) | texttype;
    
    return reinterpret_cast<Label*>(cpp_label);
}

void label_free(Label* label) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->~Label();
        free(cpp_label);
    }
}

const char* label_text(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->text;
    }
    return nullptr;
}

void label_set_text(Label* label, const char* text) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        if (cpp_label->text) {
            free(cpp_label->text);
        }
        if (text) {
            cpp_label->text = (char*)malloc(strlen(text) + 1);
            if (cpp_label->text) {
                strcpy(cpp_label->text, text);
            }
        } else {
            cpp_label->text = nullptr;
        }
    }
}

Vec2 label_origin(const Label* label) {
    Vec2 origin = {0, 0};
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        origin.x = cpp_label->origin.x;
        origin.y = cpp_label->origin.y;
    }
    return origin;
}

void label_set_origin(Label* label, Vec2 origin) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->origin.x = origin.x;
        cpp_label->origin.y = origin.y;
    }
}

Vec2 label_position(const Label* label) {
    Vec2 pos = {0, 0};
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        pos.x = cpp_label->origin.x;
        pos.y = cpp_label->origin.y;
    }
    return pos;
}

uint32_t label_layer(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return (uint32_t)(cpp_label->tag >> 16);
    }
    return 0;
}

uint32_t label_texttype(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return (uint32_t)(cpp_label->tag & 0xFFFF);
    }
    return 0;
}

// Reference functions
Reference* reference_new(Cell* cell, Vec2 origin, double rotation, double magnification, 
                        bool x_reflection, Tag tag) {
    if (!cell) return nullptr;
    
    gdstk::Reference* cpp_reference = static_cast<gdstk::Reference*>(malloc(sizeof(gdstk::Reference)));
    if (!cpp_reference) return nullptr;
    
    new(cpp_reference) gdstk::Reference();
    
    cpp_reference->type = gdstk::ReferenceType::Cell;
    cpp_reference->cell = reinterpret_cast<gdstk::Cell*>(cell);
    cpp_reference->origin.x = origin.x;
    cpp_reference->origin.y = origin.y;
    cpp_reference->rotation = rotation;
    cpp_reference->magnification = magnification;
    cpp_reference->x_reflection = x_reflection;
    cpp_reference->repetition.type = gdstk::RepetitionType::None;
    
    return reinterpret_cast<Reference*>(cpp_reference);
}

void reference_free(Reference* reference) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->~Reference();
        free(cpp_reference);
    }
}

Vec2 reference_origin(const Reference* reference) {
    Vec2 origin = {0, 0};
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        origin.x = cpp_reference->origin.x;
        origin.y = cpp_reference->origin.y;
    }
    return origin;
}

double reference_rotation(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        return cpp_reference->rotation;
    }
    return 0.0;
}

double reference_magnification(const Reference* reference) {
    if (reference) {
        const gdstk::Reference* cpp_reference = reinterpret_cast<const gdstk::Reference*>(reference);
        return cpp_reference->magnification;
    }
    return 1.0;
}

void reference_set_repetition_rectangular(Reference* reference, uint64_t columns, uint64_t rows, Vec2 spacing) {
    if (reference) {
        gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
        cpp_reference->repetition.type = gdstk::RepetitionType::Rectangular;
        cpp_reference->repetition.columns = columns;
        cpp_reference->repetition.rows = rows;
        cpp_reference->repetition.spacing.x = spacing.x;
        cpp_reference->repetition.spacing.y = spacing.y;
    }
}

// Utility functions
bool approx_equal(double a, double b) {
    return fabs(a - b) < 1e-12;
}

bool points_equal(Vec2 a, Vec2 b) {
    return approx_equal(a.x, b.x) && approx_equal(a.y, b.y);
}

// Vec2 utility functions
Vec2 vec2_new(double x, double y) {
    Vec2 result = {x, y};
    return result;
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 result = {a.x + b.x, a.y + b.y};
    return result;
}

Vec2 vec2_subtract(Vec2 a, Vec2 b) {
    Vec2 result = {a.x - b.x, a.y - b.y};
    return result;
}

double vec2_length(Vec2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

}  // extern "C"
