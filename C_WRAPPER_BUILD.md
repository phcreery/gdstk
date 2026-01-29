# GDSTK C Wrapper Build System

This document describes how to build and use the GDSTK library with C wrapper using the updated root `Makefile_c.mk`.

## Overview

The updated build system creates two libraries:

1. **`libgdstk.a`** - Pure C++ GDSTK library
2. **`libgdstk_c.a`** - Combined library with both C++ core and C wrapper

## Building

### Build All Libraries
```bash
make -f Makefile_c.mk
```

### Build Only C++ Library
```bash
make -f Makefile_c.mk gdstk-only
```

### Build Combined Library with C Wrapper
```bash
make -f Makefile_c.mk c-wrapper
```

### Install Libraries and Headers
```bash
make -f Makefile_c.mk install
```

### Clean Build Artifacts
```bash
make -f Makefile_c.mk clean
```

### Show Build Configuration
```bash
make -f Makefile_c.mk info
```

## Output Files

After building, you will find:

- `build/lib/libgdstk.a` - C++ only library
- `build/lib/libgdstk_c.a` - Combined library with C wrapper
- `build/obj/` - C++ object files
- `build/obj/c/` - C wrapper object files

After installation:

- `lib/libgdstk.a` and `lib/libgdstk_c.a` - Installed libraries
- `include/gdstk_install/gdstk/` - All headers (C++ and C wrapper)

## Using the C Wrapper

### Include Headers
```c
#include <gdstk/gdstk.h>  // Main C wrapper header
```

### Compile Your Application
```bash
gcc -std=c11 -I./c -I./include your_app.c -L./build/lib -lgdstk_c -lclipper -lqhullstatic_r -lz -lstdc++ -lm -o your_app
```

### Example Usage
```c
#include <gdstk/gdstk.h>

int main() {
    // Create polygon with Tag
    Vec2 points[] = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    Tag tag = make_tag(1, 0);  // Layer 1, datatype 0
    Polygon* rect = polygon_new(points, 4, tag);
    
    // Use the polygon...
    double area = polygon_area(rect);
    
    // Clean up
    polygon_free(rect);
    return 0;
}
```

## Testing

The C wrapper test suite has been updated to use the combined library:

```bash
cd tests/c
make clean
make basic_test
./basic_test
```

## Key Features

- **API Alignment**: C wrapper closely matches C++ GDSTK API
- **Tag System**: Uses Tag for layer/datatype management with utility functions
- **Memory Safety**: Proper memory management and cleanup
- **Type Safety**: C-compatible types that map to C++ equivalents
- **No Test Utilities**: Removed non-API functions like `approx_equal`

## Dependencies

The build requires:
- GCC/G++ compiler with C++11 support
- Clipper library (included in external/)
- Qhull library (included in external/)
- Zlib library (included in external/)

## Notes

- The C wrapper provides a C-compatible interface to GDSTK functionality
- All C wrapper functions use `extern "C"` linkage for C compatibility
- The combined library includes both C++ core and C wrapper in a single static library
- Memory management is the user's responsibility - always call appropriate `_free()` functions
