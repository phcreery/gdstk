#ifndef GDSTK_C_HEADER_REPETITION
#define GDSTK_C_HEADER_REPETITION

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct Vec2 Vec2;
typedef struct Array Array;
typedef struct Property Property;
typedef enum ErrorCode ErrorCode;

// Repetition types
typedef enum {
    GDSTK_REPETITION_NONE = 0,     // No repetition
    GDSTK_REPETITION_RECTANGULAR,  // columns × rows matrix of copies spaced by spacing
    GDSTK_REPETITION_REGULAR,      // columns × rows matrix along directions v1 and v2
    GDSTK_REPETITION_EXPLICIT,     // explicit repetition at offsets
    GDSTK_REPETITION_EXPLICIT_X,   // explicit repetitions along the x axis at coords
    GDSTK_REPETITION_EXPLICIT_Y    // explicit repetitions along the y axis at coords
} RepetitionType;

// Repetition structure
typedef struct Repetition {
    RepetitionType type;
    union {
        struct {               // Rectangular and Regular
            uint64_t columns;  // Along x or v1
            uint64_t rows;     // Along y or v2
            union {
                Vec2 spacing;  // Rectangular spacing
                struct {
                    Vec2 v1;   // Regular axis 1
                    Vec2 v2;   // Regular axis 2
                };
            };
        };
        // The original element should not be explicitly included, i.e., (0, 0)
        Array* offsets;        // Explicit - Array of Vec2
        Array* coords;         // ExplicitX and ExplicitY - Array of double
    };
} Repetition;

// Repetition creation functions
Repetition* repetition_new_none(void);
Repetition* repetition_new_rectangular(uint64_t columns, uint64_t rows, Vec2 spacing);
Repetition* repetition_new_regular(uint64_t columns, uint64_t rows, Vec2 v1, Vec2 v2);
Repetition* repetition_new_explicit(const Array* offsets);
Repetition* repetition_new_explicit_x(const Array* x_coords);
Repetition* repetition_new_explicit_y(const Array* y_coords);

// Repetition management functions
void repetition_free(Repetition* repetition);
void repetition_clear(Repetition* repetition);
void repetition_print(const Repetition* repetition);

// Copy function
void repetition_copy_from(Repetition* repetition, const Repetition* source);

// Repetition properties
uint64_t repetition_get_count(const Repetition* repetition);
void repetition_get_offsets(const Repetition* repetition, Array* result);

// Repetition type queries
bool repetition_is_none(const Repetition* repetition);
bool repetition_is_rectangular(const Repetition* repetition);
bool repetition_is_regular(const Repetition* repetition);
bool repetition_is_explicit(const Repetition* repetition);

// Repetition parameter access for rectangular
void repetition_get_rectangular_params(const Repetition* repetition, 
                                     uint64_t* columns, uint64_t* rows, Vec2* spacing);
void repetition_set_rectangular_params(Repetition* repetition, 
                                     uint64_t columns, uint64_t rows, Vec2 spacing);

// Repetition parameter access for regular
void repetition_get_regular_params(const Repetition* repetition, 
                                 uint64_t* columns, uint64_t* rows, Vec2* v1, Vec2* v2);
void repetition_set_regular_params(Repetition* repetition, 
                                 uint64_t columns, uint64_t rows, Vec2 v1, Vec2 v2);

// Repetition parameter access for explicit
Array* repetition_get_explicit_offsets(const Repetition* repetition);
void repetition_set_explicit_offsets(Repetition* repetition, const Array* offsets);

Array* repetition_get_explicit_coords(const Repetition* repetition);
void repetition_set_explicit_coords(Repetition* repetition, const Array* coords);

// Repetition transformation functions
void repetition_translate(Repetition* repetition, Vec2 offset);
void repetition_scale(Repetition* repetition, Vec2 scale, Vec2 center);
void repetition_rotate(Repetition* repetition, double angle, Vec2 center);
void repetition_mirror(Repetition* repetition, Vec2 p0, Vec2 p1);
void repetition_transform(Repetition* repetition, double magnification, bool x_reflection,
                         double rotation, Vec2 origin);

// Repetition validation functions
bool repetition_is_valid(const Repetition* repetition);
ErrorCode repetition_validate(const Repetition* repetition);

// Repetition utility functions
Repetition* repetition_clone(const Repetition* repetition);
bool repetition_equal(const Repetition* rep1, const Repetition* rep2);

// Repetition optimization functions
void repetition_optimize_rectangular(Repetition* repetition);
void repetition_optimize_regular(Repetition* repetition);

// Repetition bounding box
void repetition_bounding_box(const Repetition* repetition, Vec2 element_min, Vec2 element_max,
                           Vec2* total_min, Vec2* total_max);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_REPETITION
