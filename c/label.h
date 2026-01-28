#ifndef GDSTK_C_HEADER_LABEL
#define GDSTK_C_HEADER_LABEL

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Anchor enumeration
typedef enum {
    GDSTK_ANCHOR_NW = 0,  // Northwest
    GDSTK_ANCHOR_N = 1,   // North
    GDSTK_ANCHOR_NE = 2,  // Northeast
    GDSTK_ANCHOR_W = 4,   // West
    GDSTK_ANCHOR_O = 5,   // Origin/Center
    GDSTK_ANCHOR_E = 6,   // East
    GDSTK_ANCHOR_SW = 8,  // Southwest
    GDSTK_ANCHOR_S = 9,   // South
    GDSTK_ANCHOR_SE = 10  // Southeast
} Anchor;

// Label structure (C wrapper for C++ Label)
typedef struct Label {
    Tag tag;
    char* text;           // NULL-terminated text string
    Vec2 origin;
    Anchor anchor;        // Text anchor (not supported by OASIS)
    double rotation;      // in radians (not supported by OASIS)
    double magnification; // (not supported by OASIS)
    bool x_reflection;    // (not supported by OASIS)
    Repetition* repetition;
    Property* properties;
    void* owner;  // For Python interface
} Label;

// Label functions
Label* label_new(const char* text, Vec2 origin, double rotation, Tag tag);
void label_free(Label* label);
void label_clear(Label* label);
void label_print(const Label* label);

// Copy function
void label_copy_from(Label* label, const Label* source);

// Bounding box function
void label_bounding_box(const Label* label, Vec2* min, Vec2* max);

// Transformation function
void label_transform(Label* label, double magnification, bool x_reflection, 
                    double rotation, Vec2 origin);

// Apply repetition
void label_apply_repetition(Label* label, Array* result);

// Output functions
ErrorCode label_to_gds(const Label* label, FILE* out, double scaling);
ErrorCode label_to_svg(const Label* label, FILE* out, double scaling, uint32_t precision);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_LABEL
