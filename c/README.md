# GDSTK C Header Bindings

This directory contains comprehensive C header bindings for the GDSTK C++ library, providing a complete C interface for working with GDSII and OASIS layout files.

## Example

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
