#include "cell.h"
#include "../include/gdstk/cell.hpp"
#include "../include/gdstk/polygon.hpp"
#include <cstdlib>
#include <cstring>
#include <new>

// Cell C wrapper functions

Cell* cell_new(const char* name) {
    gdstk::Cell* cpp_cell = static_cast<gdstk::Cell*>(malloc(sizeof(gdstk::Cell)));
    if (!cpp_cell) return nullptr;
    
    // Initialize the C++ object in-place
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
        cpp_cell->~Cell(); // Call destructor
        free(cpp_cell);
    }
}

void cell_clear(Cell* cell) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        cpp_cell->clear();
    }
}

void cell_print(const Cell* cell, bool all) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        cpp_cell->print(all);
    }
}

const char* cell_name(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return cpp_cell->name;
    }
    return nullptr;
}

void cell_set_name(Cell* cell, const char* name) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        if (cpp_cell->name) {
            free(cpp_cell->name);
        }
        if (name) {
            cpp_cell->name = (char*)malloc(strlen(name) + 1);
            if (cpp_cell->name) {
                strcpy(cpp_cell->name, name);
            }
        } else {
            cpp_cell->name = nullptr;
        }
    }
}

// Element count functions
size_t cell_polygon_count(const Cell* cell) {
    if (!cell) return 0;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    return cpp_cell->polygon_array.count;
}

size_t cell_label_count(const Cell* cell) {
    if (!cell) return 0;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    return cpp_cell->label_array.count;
}

size_t cell_reference_count(const Cell* cell) {
    if (!cell) return 0;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    return cpp_cell->reference_array.count;
}

size_t cell_flexpath_count(const Cell* cell) {
    if (!cell) return 0;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    return cpp_cell->flexpath_array.count;
}

size_t cell_robustpath_count(const Cell* cell) {
    if (!cell) return 0;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    return cpp_cell->robustpath_array.count;
}

// Element access functions
Polygon* cell_get_polygon(const Cell* cell, size_t index) {
    if (!cell) return nullptr;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    if (index >= cpp_cell->polygon_array.count) return nullptr;
    return reinterpret_cast<Polygon*>(cpp_cell->polygon_array.items[index]);
}

Label* cell_get_label(const Cell* cell, size_t index) {
    if (!cell) return nullptr;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    if (index >= cpp_cell->label_array.count) return nullptr;
    return reinterpret_cast<Label*>(cpp_cell->label_array.items[index]);
}

Reference* cell_get_reference(const Cell* cell, size_t index) {
    if (!cell) return nullptr;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    if (index >= cpp_cell->reference_array.count) return nullptr;
    return reinterpret_cast<Reference*>(cpp_cell->reference_array.items[index]);
}

FlexPath* cell_get_flexpath(const Cell* cell, size_t index) {
    if (!cell) return nullptr;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    if (index >= cpp_cell->flexpath_array.count) return nullptr;
    return reinterpret_cast<FlexPath*>(cpp_cell->flexpath_array.items[index]);
}

RobustPath* cell_get_robustpath(const Cell* cell, size_t index) {
    if (!cell) return nullptr;
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    if (index >= cpp_cell->robustpath_array.count) return nullptr;
    return reinterpret_cast<RobustPath*>(cpp_cell->robustpath_array.items[index]);
}

// Copy function
Cell* cell_copy(const Cell* cell, const char* new_name, bool deep_copy) {
    if (!cell || !new_name) return NULL;
    
    Cell* new_cell = (Cell*)malloc(sizeof(Cell));
    memset(new_cell, 0, sizeof(Cell));
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    gdstk::Cell* cpp_new_cell = (gdstk::Cell*)new_cell;
    cpp_new_cell->copy_from(*cpp_cell, new_name, deep_copy);
    
    return new_cell;
}

// Element removal functions
bool cell_remove_polygon(Cell* cell, Polygon* polygon) {
    if (!cell || !polygon) return false;
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    gdstk::Polygon* cpp_polygon = (gdstk::Polygon*)polygon;
    
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
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    gdstk::Label* cpp_label = (gdstk::Label*)label;
    
    for (uint64_t i = 0; i < cpp_cell->label_array.count; i++) {
        if (cpp_cell->label_array[i] == cpp_label) {
            cpp_cell->label_array.remove(i);
            return true;
        }
    }
    return false;
}

bool cell_remove_reference(Cell* cell, Reference* reference) {
    if (!cell || !reference) return false;
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    gdstk::Reference* cpp_reference = (gdstk::Reference*)reference;
    
    for (uint64_t i = 0; i < cpp_cell->reference_array.count; i++) {
        if (cpp_cell->reference_array[i] == cpp_reference) {
            cpp_cell->reference_array.remove(i);
            return true;
        }
    }
    return false;
}

// Geometry functions
void cell_bounding_box(const Cell* cell, Vec2* min, Vec2* max) {
    if (!cell || !min || !max) return;
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    gdstk::Vec2 cpp_min, cpp_max;
    cpp_cell->bounding_box(cpp_min, cpp_max);
    
    min->x = cpp_min.x;
    min->y = cpp_min.y;
    max->x = cpp_max.x;
    max->y = cpp_max.y;
}

double cell_area(const Cell* cell, bool by_spec, Map* result) {
    if (!cell) return 0.0;
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    gdstk::Array<gdstk::Polygon*> polygons = {0};
    cpp_cell->get_polygons(true, true, -1, false, 0, polygons);
    
    double total_area = 0.0;
    for (uint64_t i = 0; i < polygons.count; i++) {
        total_area += polygons[i]->area();
    }
    
    polygons.clear();
    return total_area;
}

void cell_flatten(Cell* cell, bool apply_repetitions, Array* removed_references) {
    if (!cell) return;
    
    gdstk::Cell* cpp_cell = (gdstk::Cell*)cell;
    if (removed_references) {
        gdstk::Array<gdstk::Reference*>& cpp_removed = *(gdstk::Array<gdstk::Reference*>*)removed_references;
        cpp_cell->flatten(apply_repetitions, cpp_removed);
    } else {
        gdstk::Array<gdstk::Reference*> temp_removed = {};
        cpp_cell->flatten(apply_repetitions, temp_removed);
        temp_removed.clear();
    }
}

// Element management functions
void cell_add_polygon(Cell* cell, Polygon* polygon) {
    if (!cell || !polygon) return;
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::Polygon* cpp_polygon = reinterpret_cast<gdstk::Polygon*>(polygon);
    cpp_cell->polygon_array.append(cpp_polygon);
}

void cell_add_label(Cell* cell, Label* label) {
    if (!cell || !label) return;
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
    cpp_cell->label_array.append(cpp_label);
}

void cell_add_reference(Cell* cell, Reference* reference) {
    if (!cell || !reference) return;
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::Reference* cpp_reference = reinterpret_cast<gdstk::Reference*>(reference);
    cpp_cell->reference_array.append(cpp_reference);
}

void cell_add_flexpath(Cell* cell, FlexPath* flexpath) {
    if (!cell || !flexpath) return;
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::FlexPath* cpp_flexpath = reinterpret_cast<gdstk::FlexPath*>(flexpath);
    cpp_cell->flexpath_array.append(cpp_flexpath);
}

void cell_add_robustpath(Cell* cell, RobustPath* robustpath) {
    if (!cell || !robustpath) return;
    gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
    gdstk::RobustPath* cpp_robustpath = reinterpret_cast<gdstk::RobustPath*>(robustpath);
    cpp_cell->robustpath_array.append(cpp_robustpath);
}

// Get elements functions (match C++ API)
void cell_get_polygons(const Cell* cell, bool apply_repetitions, bool include_paths, 
                      int64_t depth, bool filter, Tag tag, Array* result) {
    if (!cell || !result) return;
    
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    gdstk::Array<gdstk::Polygon*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Polygon*>*>(result);
    cpp_cell->get_polygons(apply_repetitions, include_paths, depth, filter, tag, *cpp_result);
}

void cell_get_flexpaths(const Cell* cell, bool apply_repetitions, int64_t depth, 
                       bool filter, Tag tag, Array* result) {
    if (!cell || !result) return;
    
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    gdstk::Array<gdstk::FlexPath*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::FlexPath*>*>(result);
    cpp_cell->get_flexpaths(apply_repetitions, depth, filter, tag, *cpp_result);
}

void cell_get_robustpaths(const Cell* cell, bool apply_repetitions, int64_t depth, 
                         bool filter, Tag tag, Array* result) {
    if (!cell || !result) return;
    
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    gdstk::Array<gdstk::RobustPath*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::RobustPath*>*>(result);
    cpp_cell->get_robustpaths(apply_repetitions, depth, filter, tag, *cpp_result);
}

void cell_get_labels(const Cell* cell, bool apply_repetitions, int64_t depth, 
                    bool filter, Tag tag, Array* result) {
    if (!cell || !result) return;
    
    const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
    gdstk::Array<gdstk::Label*>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Label*>*>(result);
    cpp_cell->get_labels(apply_repetitions, depth, filter, tag, *cpp_result);
}
