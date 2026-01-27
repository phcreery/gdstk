#ifndef GDSTK_C_HEADER_STYLE
#define GDSTK_C_HEADER_STYLE

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef uint64_t Tag;

// Style structure for SVG output
typedef struct Style {
    Tag tag;
    char* value;  // SVG style string, e.g., "stroke: #D04030; fill: #D89080;"
} Style;

// StyleMap structure - hash map of styles indexed by tag
typedef struct StyleMap {
    uint64_t capacity;  // allocated capacity
    uint64_t count;     // number of items in the map
    Style* items;       // array with length capacity
} StyleMap;

// StyleMap creation and management
StyleMap* stylemap_new(void);
void stylemap_free(StyleMap* stylemap);
void stylemap_clear(StyleMap* stylemap);
void stylemap_print(const StyleMap* stylemap, bool show_all);

// Copy functions
void stylemap_copy_from(StyleMap* stylemap, const StyleMap* source);
StyleMap* stylemap_copy(const StyleMap* source);

// Style operations
void stylemap_set(StyleMap* stylemap, Tag tag, const char* style);
const char* stylemap_get(const StyleMap* stylemap, Tag tag);
bool stylemap_has_tag(const StyleMap* stylemap, Tag tag);
bool stylemap_remove(StyleMap* stylemap, Tag tag);
uint64_t stylemap_size(const StyleMap* stylemap);

// Style iteration
Style* stylemap_first(const StyleMap* stylemap);
Style* stylemap_next(const StyleMap* stylemap, const Style* current);

// Utility functions
void stylemap_reserve(StyleMap* stylemap, uint64_t capacity);
bool stylemap_empty(const StyleMap* stylemap);

// Style validation
bool style_is_valid(const char* style);
char* style_normalize(const char* style);  // Caller must free

// Common style presets
const char* style_preset_stroke_only(const char* color, double width);
const char* style_preset_fill_only(const char* color);
const char* style_preset_stroke_and_fill(const char* stroke_color, double stroke_width, 
                                         const char* fill_color);
const char* style_preset_dashed(const char* color, double width, const char* dash_pattern);

// Style merging
char* style_merge(const char* base_style, const char* override_style);  // Caller must free

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_STYLE
