# GDSTK C Header Bindings

This directory contains comprehensive C header bindings for the GDSTK C++ library, providing a complete C interface for working with GDSII and OASIS layout files.

## Files

### Core Headers
- `gdstk.h` - Main header that includes all components
- `cell.h` - Cell structures and operations
- `polygon.h` - Polygon creation and manipulation
- `library.h` - Library management and I/O operations
- `label.h` - Text labels and annotations
- `reference.h` - Cell references and transformations
- `utils.h` - Utility functions and basic data structures

### Implementation and Examples
- `cell.cpp` - C++ implementation wrapper for Cell functionality
- `simple_example.c` - Standalone example demonstrating the interface
- `example.c` - Full example using the complete GDSTK library

## Overview

The C bindings provide a comprehensive C-compatible interface to the GDSTK C++ library. The bindings:

- Convert C++ classes to opaque pointers for C compatibility
- Provide C-style function interfaces for all major GDSTK features
- Handle type conversions between C and C++ types
- Include complete error handling with C-compatible error codes
- Support all major GDSTK operations including I/O, geometry, and transformations

## Architecture

### Memory Management
All complex objects are represented as opaque pointers in C, with explicit allocation and deallocation functions:

```c
// Create objects
Cell* cell = cell_new("my_cell");
Polygon* poly = polygon_new();
Library* lib = library_new("my_lib", 1e-6, 1e-9);

// Use objects...

// Free objects
cell_free(cell);
polygon_free(poly);
library_free(lib);
```

### Error Handling
All functions that can fail return an `ErrorCode` enum:

```c
typedef enum {
    GDSTK_NO_ERROR = 0,
    GDSTK_BOOLEAN_ERROR,
    GDSTK_INVALID_FILE,
    // ... more error codes
} ErrorCode;

ErrorCode result = library_write_gds(lib, "output.gds", 4000, NULL);
if (result != GDSTK_NO_ERROR) {
    printf("Error: %s\n", gdstk_error_string(result));
}
```

### Type Safety
- Simple structures like `Vec2` are exposed directly
- Complex C++ containers use generic `Array`, `Map`, `Set` wrappers
- Function pointers maintain type safety where possible

## Key Features by Component

### Cell Operations (`cell.h`)

Cell management and hierarchy:

```c
Cell* cell = cell_new("transistor");
cell_print(cell, true);

// Get bounding box
Vec2 min, max;
cell_bounding_box(cell, &min, &max);

// Copy cells
Cell* copy = cell_new("transistor_copy");
cell_copy_from(copy, cell, "new_name", true);

cell_free(cell);
cell_free(copy);
```

### Polygon Operations (`polygon.h`)

Polygon creation and manipulation:

```c
// Create rectangle
Polygon* rect = polygon_rectangle(vec2_new(0, 0), vec2_new(10, 5), make_tag(1, 0));

// Get area and perimeter
double area = polygon_area(rect);
double perimeter = polygon_perimeter(rect);

// Check point containment
bool inside = polygon_contain(rect, vec2_new(5, 2.5));

// Transform polygon
polygon_translate(rect, vec2_new(10, 10));
polygon_rotate(rect, M_PI/4, vec2_new(5, 2.5));

polygon_free(rect);
```

### Library I/O (`library.h`)

File operations and library management:

```c
Library* lib = library_new("my_design", 1e-6, 1e-9);

// Add cells to library
// (cell creation and population...)

// Write to GDSII file
ErrorCode result = library_write_gds(lib, "design.gds", 4000, NULL);
if (result != GDSTK_NO_ERROR) {
    printf("Write failed: %s\n", gdstk_error_string(result));
}

// Read from GDSII file
Library* loaded = library_new("loaded", 1e-6, 1e-9);
result = library_read_gds("design.gds", 1e-6, 1e-9, loaded);

library_free(lib);
library_free(loaded);
```

### Labels (`label.h`)

Text annotation support:

```c
Label* label = label_new("Pin A");
label->origin = vec2_new(0, 0);
label->tag = make_tag(10, 0);
label->anchor = GDSTK_ANCHOR_O;  // Center anchor
label->rotation = M_PI/6;        // 30 degrees

// Transform label
label_transform(label, 1.5, false, M_PI/4, vec2_new(5, 5));

label_free(label);
```

### References (`reference.h`)

Cell instantiation and arrays:

```c
// Create reference to a cell
Reference* ref = reference_new_cell(some_cell);
ref->origin = vec2_new(100, 100);
ref->magnification = 2.0;
ref->rotation = M_PI/2;

// Transform reference
reference_scale(ref, 1.5, vec2_new(0, 0));

// Get polygons from reference (with transformations applied)
Array* polygons = array_ptr_new();
reference_get_polygons(ref, true, true, -1, false, 0, polygons);

reference_free(ref);
array_ptr_free(polygons);
```

### Utilities (`utils.h`)

Basic data structures and math:

```c
// Vec2 operations
Vec2 a = vec2_new(3, 4);
Vec2 b = vec2_new(1, 1);
Vec2 sum = vec2_add(a, b);
double length = vec2_length(a);  // Returns 5.0

// Array operations
Array* points = array_vec2_new();
array_vec2_append(points, vec2_new(0, 0));
array_vec2_append(points, vec2_new(10, 10));
uint64_t count = array_vec2_size(points);

// Tag operations
Tag tag = make_tag(5, 2);  // Layer 5, datatype 2
uint32_t layer = get_layer(tag);
uint32_t type = get_type(tag);

array_vec2_free(points);
```

## Building

### Quick Test (Standalone)

For testing the interface without full GDSTK:

```bash
cd d:\myprojects\gdstk\c
gcc simple_example.c -o simple_example.exe
.\simple_example.exe
```

### Full Integration

For complete functionality with GDSTK library:

```bash
cd d:\myprojects\gdstk

# Method 1: Using CMake (Recommended)
mkdir build_c
cd build_c
cmake ..
cmake --build . --config Release

# Method 2: Manual compilation (requires dependencies)
cd c
g++ -I../include -std=c++11 -c cell.cpp -o cell.o
gcc -c example.c -o example.o
g++ example.o cell.o ../lib/gdstk.a [dependency libraries] -o example.exe
```

## Complete API Reference

### Cell Functions
- `cell_new()`, `cell_free()`, `cell_clear()`, `cell_print()`
- `cell_bounding_box()`, `cell_convex_hull()`
- `cell_get_polygons()`, `cell_get_labels()`, etc.
- `cell_flatten()`, `cell_remap_tags()`

### Polygon Functions
- `polygon_new()`, `polygon_free()`, `polygon_rectangle()`, `polygon_cross()`
- `polygon_area()`, `polygon_perimeter()`, `polygon_contain()`
- `polygon_translate()`, `polygon_rotate()`, `polygon_scale()`
- `polygon_fillet()`, `polygon_fracture()`

### Library Functions
- `library_new()`, `library_free()`, `library_clear()`
- `library_write_gds()`, `library_read_gds()`
- `library_write_oas()`, `library_write_svg()`
- `library_get_cell()`, `library_rename_cell()`

### Label Functions
- `label_new()`, `label_free()`, `label_transform()`
- `label_bounding_box()`, `label_apply_repetition()`

### Reference Functions
- `reference_new_cell()`, `reference_new_name()`
- `reference_transform()`, `reference_get_polygons()`
- `reference_bounding_box()`, `reference_convex_hull()`

### Utility Functions
- Vec2 math: `vec2_add()`, `vec2_length()`, `vec2_rotate()`
- Array operations: `array_vec2_*()`, `array_ptr_*()`
- Map/Set operations: `map_*()`, `set_*()`
- Tag utilities: `make_tag()`, `get_layer()`, `get_type()`

## Design Philosophy

- **Minimal overhead**: Direct mapping to C++ functionality
- **Memory safety**: Explicit allocation/deallocation with clear ownership
- **Error handling**: Consistent error codes and string descriptions
- **Type safety**: Strong typing where possible, opaque pointers where needed
- **Completeness**: Full API coverage for all major GDSTK features
- **Compatibility**: Works with standard C99 and later

## Limitations

- Template features require separate typed functions
- Some C++ operator overloads converted to explicit functions
- Complex iterators replaced with index-based access
- Exception handling converted to error return codes
- STL containers replaced with custom Array/Map/Set wrappers

## Integration Notes

The bindings are designed for:
- Integration into C projects
- FFI bindings for other languages (Python ctypes, Go cgo, etc.)
- Embedded systems requiring C interfaces
- Legacy C codebases needing GDSII/OASIS support
