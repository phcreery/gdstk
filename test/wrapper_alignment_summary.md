# GDSTK C Wrapper API Alignment Summary

## Overview
This document summarizes the changes made to align the GDSTK C wrapper headers with the actual C++ API exposed in the GDSTK headers.

## Key Changes Made

### 1. FlexPath (c/flexpath.h)
- **Enums Updated**: Fixed JoinType, EndType, and BendType enums to match C++ values exactly:
  - JoinType: Added GDSTK_JOIN_NATURAL (0), GDSTK_JOIN_SMOOTH
  - EndType: Added GDSTK_END_HALFWIDTH, GDSTK_END_SMOOTH, reordered values
  - BendType: Values already matched
- **Creation Functions**: Updated to match C++ init overloads:
  - Added tolerance parameter to all creation functions
  - Added multi-element and separated element variants
- **Path Building Functions**: Updated to match C++ method signatures:
  - Added horizontal/vertical array variants
  - Added segment array variants
  - Added cubic_smooth, quadratic_smooth array variants
  - Updated parametric function signature
  - Added commands() method
- **Removed Non-API Functions**: Removed geometry property functions (area, length, bounding_box, position, gradient) that don't exist in C++ FlexPath
- **Function Pointer Types**: Updated to match C++ pathcommon.hpp exactly

### 2. RobustPath (c/robustpath.h)
- **Structure Completely Redesigned**: Updated to match C++ RobustPath structure:
  - Added end_point, tolerance, max_evals, width_scale, offset_scale, trafo[6]
  - Updated element structure to use Interpolation arrays instead of single values
- **Interpolation System**: Added proper Interpolation structure matching C++:
  - InterpolationType enum with Constant, Linear, Smooth, Parametric
  - Interpolation union structure for different interpolation types
- **SubPath System**: Added SubPath structure and SubPathType enum
- **Creation Functions**: Updated to match C++ init overloads with tolerance and max_evals
- **Path Building Functions**: Updated all functions to use Interpolation* parameters instead of double* arrays
- **Position Functions**: Updated position/gradient functions to use parametric u coordinates with from_below parameter
- **Added Missing Methods**: Added width(), offset(), element_center(), commands()

### 3. Polygon (c/polygon.h)
- **Constructor Updated**: Changed from layer/datatype parameters to Tag parameter
- **Removed Accessor Functions**: Removed separate layer/datatype getter/setter functions (encoded in Tag)
- **Added Factory Functions**: Added all factory functions from C++ API:
  - regular_polygon, ellipse, racetrack
  - text (creates multiple polygons)
  - contour function
- **Added Multi-Polygon Functions**: Added inside functions for checking points against multiple polygons

### 4. Label (c/label.h)  
- **Constructor Updated**: Changed from layer/texttype parameters to Tag parameter
- **Removed Accessor Functions**: Removed individual property getter/setter functions
- **Structure**: Kept pointer-based structure for C wrapper compatibility

### 5. Reference (c/reference.h)
- **Structure**: Maintained existing structure, already mostly aligned with C++ API
- **Minor Updates**: Ensured all transformation functions match C++ signatures

### 6. Curve (c/curve.h) - Previously Updated
- **Removed Non-API Functions**: Removed convenience functions (curve_segment_to, curve_segment_by) 
- **Added Missing Methods**: Added curve_remove, curve_ensure_slots, curve_parametric, etc.
- **Updated Enums**: Fixed function pointer types to match C++ API
- **Test Updates**: Updated tests to match actual C++ behavior (no tolerance-based filtering in segment)

### 7. Common Infrastructure (c/common.h)
- **Added Types**: Added RaithData forward declaration
- **Added CurveInstruction**: Moved CurveInstruction structure to common header for shared use
- **Function Pointers**: Ensured all function pointer types match C++ signatures

## Structural Design Decisions

### C Wrapper vs C++ API Alignment
- **Function Signatures**: All C wrapper functions now match the C++ method signatures as closely as possible
- **Enum Values**: All enums use the exact same numeric values as the C++ enums
- **Parameter Types**: Updated parameter types to match C++ (e.g., Interpolation* instead of double*)
- **Missing C++ Methods**: Added all public C++ methods that were missing from C wrappers
- **Non-Existent Methods**: Removed C wrapper functions that have no equivalent in the C++ API

### Memory Management
- **Pointer-based Structures**: Kept pointer-based approach for C wrappers since:
  - C structures can't embed C++ objects directly
  - Allows for opaque implementation hiding C++ objects
  - Maintains ABI compatibility
- **Embedded vs Pointer Members**: Used pointers for complex types (Repetition, Curve, etc.) to avoid incomplete type issues

### API Completeness
- **Full Coverage**: Ensured all public C++ methods have corresponding C wrapper functions
- **Correct Signatures**: All function signatures now accurately reflect the C++ API
- **Proper Types**: Used correct types for all parameters (Tag instead of layer/datatype, Interpolation* instead of double*, etc.)

## Test Results
- **Structure Layout Test**: Passes - confirms headers compile correctly
- **Enum Value Test**: Passes - confirms enum values match C++ exactly
- **Previous Tests**: Curve test still passes with updated expectations matching actual C++ behavior

## Benefits of These Changes
1. **API Accuracy**: C wrapper now accurately exposes the true GDSTK C++ API
2. **Behavioral Consistency**: Test expectations now match actual C++ library behavior
3. **Complete Coverage**: All public C++ methods are now available through C wrapper
4. **Type Safety**: Proper types used throughout (Tag, Interpolation, etc.)
5. **Future-Proof**: Structure closely follows C++ API, making it easier to maintain
