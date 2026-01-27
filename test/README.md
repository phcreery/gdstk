# GDSTK C Bindings Test Suite

This directory contains comprehensive tests for the GDSTK C bindings, demonstrating how to use the C API to perform various geometric operations and cell manipulations.

## Overview

The test suite is a C translation of the Python tests found in `../tests/cell_test.py`, showcasing the following functionality:

- **Cell Operations**: Creating, copying, and manipulating cells
- **Polygon Handling**: Creating polygons, calculating areas, bounding boxes
- **Label Management**: Creating and positioning text labels
- **Reference System**: Cell references with transformations and repetitions
- **Geometric Operations**: Flattening, filtering, and complex geometry handling

## Files

- `gdstk_test.h` - Test framework header with macros and declarations
- `gdstk_test.c` - Test framework implementation with helper functions
- `cell_test.c` - Main test suite with build instructions
- `Makefile` - Build system for compiling and running tests
- `README.md` - This documentation file

## Building and Running

### Prerequisites

1. **GDSTK C++ Library**: The main GDSTK library must be built first
   ```bash
   cd ..
   make  # Build the main library
   ```

2. **C Compiler**: gcc, clang, or MSVC
3. **C Headers**: The C bindings in `../c/` directory

### Quick Start

Using the provided Makefile (recommended):
```bash
# Build and run all tests
make test

# Build and run just the cell test
make quick

# Clean build artifacts
make clean
```

### Manual Building

#### Windows (PowerShell/Command Prompt)
```cmd
# Using gcc/clang
gcc -std=c11 -I../c -I../include -L../lib -o cell_test cell_test.c gdstk_test.c -lgdstk -lstdc++ -lm

# Using MSVC
cl /I..\c /I..\include cell_test.c gdstk_test.c ..\lib\gdstk.lib
```

#### Linux/macOS
```bash
gcc -std=c11 -I../c -I../include -L../lib -o cell_test cell_test.c gdstk_test.c -lgdstk -lstdc++ -lm
```

### Running Tests

After building:
```bash
# Linux/macOS
./cell_test

# Windows
cell_test.exe
```

Expected output:
```
Running GDSTK C Bindings Test Suite
====================================

PASS: test_add_element
PASS: test_copy
PASS: test_remove
PASS: test_area
PASS: test_bounding_box
PASS: test_flatten
PASS: test_polygon_operations
PASS: test_label_operations
PASS: test_reference_operations
PASS: test_complex_geometry

====================================
Test Results: 10/10 tests passed
🎉 All tests passed!
```

## Test Framework

The test suite uses a custom testing framework (`gdstk_test.h/c`) that provides:

### Test Macros
- `TEST_ASSERT(condition, message)` - Assert a condition with failure message
- `TEST_PASS()` - Mark test as passed and return
- `TEST_RUN(test_func)` - Run a test function and track results
- `TEST_SUITE_BEGIN()` - Initialize test suite
- `TEST_SUITE_END()` - Print results and exit with appropriate code

### Helper Functions
- `approx_equal(a, b, tolerance)` - Compare floating point values
- `points_equal(vec2_a, vec2_b)` - Compare 2D points with tolerance

### Framework Philosophy
The test framework provides only the most general-purpose utilities:
- Test execution and assertion macros
- Basic mathematical comparison functions

All other functionality (geometric utilities, test fixtures, domain-specific helpers) should remain in their respective test files to maintain clean separation of concerns and maximum reusability.

### Writing New Tests

To add a new test:

1. Create a test function that returns `int` (1 for pass, 0 for fail)
2. Use `TEST_ASSERT` for conditions and `TEST_PASS()` at the end
3. Add `TEST_RUN(your_test)` to the main function
4. Include your test file in the Makefile

Example test function:
```c
int test_my_feature() {
    gdstk_cell_t* cell = gdstk_cell_new("test");
    // ... test logic ...
    TEST_ASSERT(some_condition, "Description of what should be true");
    gdstk_cell_free(cell);
    TEST_PASS();
}
```

## Test Functions

### 1. `test_add_element()`
Tests adding polygons and labels to cells, verifying that elements are properly stored and counted.

### 2. `test_copy()`
Tests both shallow and deep copying of cells, ensuring that references and contained elements are handled correctly.

### 3. `test_remove()`
Tests removing elements from cells and verifying that counts are updated appropriately.

### 4. `test_area()`
Tests area calculations for cells containing multiple polygons with different layers and datatypes.

### 5. `test_bounding_box()`
Tests bounding box calculations for complex hierarchical cell structures with references and repetitions.

### 6. `test_flatten()`
Tests the flattening operation that resolves all references into actual geometric elements.

### 7. `test_polygon_operations()`
Tests basic polygon operations including creation, property access, and area calculations.

### 8. `test_label_operations()`
Tests label creation, text access, positioning, and layer/texttype properties.

### 9. `test_reference_operations()`
Tests cell references with transformations including translation, rotation, and scaling.

### 10. `test_complex_geometry()`
Tests complex geometric operations with overlapping shapes and comprehensive bounding box calculations.

## API Usage Examples

### Creating and Using Cells
```c
#include "gdstk_test.h"  // Includes gdstk.h and test framework

// Create a new cell
gdstk_cell_t* cell = gdstk_cell_new("my_cell");

// Create a polygon using helper function
gdstk_polygon_t* polygon = create_rectangle(0, 0, 1, 1, 0, 0);

// Add polygon to cell
gdstk_cell_add_polygon(cell, polygon);

// Calculate area
double area = gdstk_cell_area(cell, false, NULL);

// Clean up
gdstk_cell_free(cell);
```

### Using Test Framework
```c
#include "gdstk_test.h"

int my_test() {
    gdstk_polygon_t* poly = create_rectangle(0, 0, 2, 2, 1, 0);
    double area = gdstk_polygon_area(poly);
    
    TEST_ASSERT(approx_equal(area, 4.0, TOLERANCE), "Area should be 4.0");
    
    gdstk_polygon_free(poly);
    TEST_PASS();
}

int main() {
    TEST_SUITE_BEGIN();
    TEST_RUN(my_test);
    TEST_SUITE_END();
}
```

### Working with References
```c
// Create reference cell
gdstk_cell_t* ref_cell = gdstk_cell_new("reference");
// ... add elements to ref_cell ...

// Create reference with transformation
gdstk_reference_t* ref = gdstk_reference_new(
    ref_cell, 
    (gdstk_vec2){10, 20},  // translation
    M_PI/4,                // rotation
    2.0,                   // magnification
    false,                 // x_reflection
    0                      // properties
);

// Add repetition
gdstk_reference_set_repetition_rectangular(ref, 3, 2, (gdstk_vec2){5, 5});

// Add to main cell
gdstk_cell_t* main_cell = gdstk_cell_new("main");
gdstk_cell_add_reference(main_cell, ref);
```

## Troubleshooting

### Common Issues

1. **Library Not Found**: Ensure the GDSTK library is built and in `../lib/`
2. **Header Not Found**: Check that C bindings are in `../c/` directory
3. **Linking Errors**: Make sure to link with `-lgdstk -lstdc++ -lm`
4. **Runtime Errors**: Verify that all required dynamic libraries are available

### Debug Building

For debugging, compile with debug symbols and no optimization:
```bash
gcc -std=c11 -g -O0 -I../c -I../include -L../lib -o cell_test_debug cell_test.c -lgdstk -lstdc++ -lm
```

### Memory Debugging

Use valgrind (Linux/macOS) or similar tools to check for memory leaks:
```bash
valgrind --leak-check=full ./cell_test
```

## Integration with Main Project

These tests demonstrate the complete C API and can serve as:

1. **Validation**: Ensuring C bindings work correctly
2. **Documentation**: Showing proper API usage patterns  
3. **Examples**: Reference implementation for C users
4. **Regression Testing**: Detecting API changes and bugs

## Contributing

When adding new tests:

1. Follow the existing test function naming pattern: `test_<feature>()`
2. Use the `TEST_ASSERT` and `TEST_PASS` macros
3. Include proper cleanup to avoid memory leaks
4. Update the main() function to run your new test
5. Document the test purpose in this README

### Extending the Framework

The test framework should remain minimal and focused. To add functionality:

1. **Core Framework Functions**: Only add to `gdstk_test.h/c` if the function is:
   - Universally useful across ALL types of tests
   - Purely mathematical or comparison-based
   - Not domain-specific to geometry, cells, libraries, etc.

2. **Everything Else Goes in Test Files**:
   - Geometric utilities (`create_rectangle`, `create_circle`, etc.)
   - Test fixtures and setup functions
   - Domain-specific helpers
   - Complex data structure creators

Example of what belongs in the framework:
```c
// In gdstk_test.h - Good: universally useful
int arrays_equal(double* a1, double* a2, size_t len, double tolerance);
int strings_equal_ignore_case(const char* s1, const char* s2);
```

Example of what belongs in test files:
```c
// In geometry_test.c - Good: geometry-specific
gdstk_polygon_t* create_circle(double radius, gdstk_vec2 center, int points, 
                              uint32_t layer, uint32_t datatype);

// In library_test.c - Good: library-specific  
gdstk_library_t* create_test_library_with_cells(int num_cells);
```

This keeps the framework lean and prevents it from becoming a dumping ground for miscellaneous utilities.

## Related Files

- `../c/` - C binding headers and implementation
- `../tests/cell_test.py` - Original Python test suite
- `../include/gdstk/` - C++ headers that the bindings wrap
- `../lib/` - Compiled GDSTK library
