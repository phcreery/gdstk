#include "cell.h"
#include "../include/gdstk/cell.hpp"
#include "../include/gdstk/polygon.hpp"
#include <cstdlib>
#include <cstring>

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

uint64_t cell_polygon_count(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return cpp_cell->polygon_array.count;
    }
    return 0;
}

uint64_t cell_reference_count(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return cpp_cell->reference_array.count;
    }
    return 0;
}

uint64_t cell_label_count(const Cell* cell) {
    if (cell) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        return cpp_cell->label_array.count;
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

Polygon* cell_get_polygon(Cell* cell, uint64_t index) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        if (index < cpp_cell->polygon_array.count) {
            return reinterpret_cast<Polygon*>(cpp_cell->polygon_array.items[index]);
        }
    }
    return nullptr;
}

Label* cell_get_label(Cell* cell, uint64_t index) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        if (index < cpp_cell->label_array.count) {
            return reinterpret_cast<Label*>(cpp_cell->label_array.items[index]);
        }
    }
    return nullptr;
}

Reference* cell_get_reference(Cell* cell, uint64_t index) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        if (index < cpp_cell->reference_array.count) {
            return reinterpret_cast<Reference*>(cpp_cell->reference_array.items[index]);
        }
    }
    return nullptr;
}

void cell_bounding_box(const Cell* cell, Vec2* min, Vec2* max) {
    if (cell && min && max) {
        const gdstk::Cell* cpp_cell = reinterpret_cast<const gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_min, cpp_max;
        cpp_cell->bounding_box(cpp_min, cpp_max);
        min->x = cpp_min.x;
        min->y = cpp_min.y;
        max->x = cpp_max.x;
        max->y = cpp_max.y;
    }
}

void cell_translate(Cell* cell, Vec2 v) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_vec = {v.x, v.y};
        cpp_cell->translate(cpp_vec);
    }
}

void cell_scale(Cell* cell, Vec2 scale, Vec2 center) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_scale = {scale.x, scale.y};
        gdstk::Vec2 cpp_center = {center.x, center.y};
        cpp_cell->scale(cpp_scale, cpp_center);
    }
}

void cell_mirror(Cell* cell, Vec2 p0, Vec2 p1) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_p0 = {p0.x, p0.y};
        gdstk::Vec2 cpp_p1 = {p1.x, p1.y};
        cpp_cell->mirror(cpp_p0, cpp_p1);
    }
}

void cell_rotate(Cell* cell, double angle, Vec2 center) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_center = {center.x, center.y};
        cpp_cell->rotate(angle, cpp_center);
    }
}

void cell_transform(Cell* cell, double magnification, bool x_reflection,
                   double rotation, Vec2 origin) {
    if (cell) {
        gdstk::Cell* cpp_cell = reinterpret_cast<gdstk::Cell*>(cell);
        gdstk::Vec2 cpp_origin = {origin.x, origin.y};
        cpp_cell->transform(magnification, x_reflection, rotation, cpp_origin);
    }
}
