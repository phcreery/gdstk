#ifndef GDSTK_C_HEADER_TAGMAP
#define GDSTK_C_HEADER_TAGMAP

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef uint64_t Tag;

// TagMap item structure
typedef struct TagMapItem {
    Tag key;
    Tag value;
} TagMapItem;

// TagMap structure - hash map between tags
// Items with the value equal to the key are ignored
typedef struct TagMap {
    uint64_t capacity;  // allocated capacity
    uint64_t count;     // number of items in the map
    TagMapItem* items;  // array with length capacity
} TagMap;

// TagMap creation and management
TagMap* tagmap_new(void);
void tagmap_free(TagMap* tagmap);
void tagmap_clear(TagMap* tagmap);
void tagmap_print(const TagMap* tagmap, bool show_all);

// Copy functions
void tagmap_copy_from(TagMap* tagmap, const TagMap* source);
TagMap* tagmap_copy(const TagMap* source);

// TagMap operations
void tagmap_set(TagMap* tagmap, Tag key, Tag value);
Tag tagmap_get(const TagMap* tagmap, Tag key, Tag default_value);
bool tagmap_has_key(const TagMap* tagmap, Tag key);
bool tagmap_remove(TagMap* tagmap, Tag key);
uint64_t tagmap_size(const TagMap* tagmap);

// TagMap iteration
TagMapItem* tagmap_first(const TagMap* tagmap);
TagMapItem* tagmap_next(const TagMap* tagmap, const TagMapItem* current);

// Utility functions
void tagmap_reserve(TagMap* tagmap, uint64_t capacity);
bool tagmap_empty(const TagMap* tagmap);

// TagMap merging and operations
void tagmap_merge(TagMap* dest, const TagMap* source, bool overwrite);
TagMap* tagmap_invert(const TagMap* tagmap);  // Swap keys and values

// Layer/datatype specific operations
void tagmap_set_layer_type(TagMap* tagmap, uint32_t key_layer, uint32_t key_type,
                          uint32_t value_layer, uint32_t value_type);
Tag tagmap_get_layer_type(const TagMap* tagmap, uint32_t layer, uint32_t type, 
                         uint32_t default_layer, uint32_t default_type);

// Bulk operations
void tagmap_remap_layers(TagMap* tagmap, uint32_t old_layer, uint32_t new_layer);
void tagmap_remap_types(TagMap* tagmap, uint32_t old_type, uint32_t new_type);

// TagMap validation
bool tagmap_is_valid(const TagMap* tagmap);

// TagMap statistics
double tagmap_load_factor(const TagMap* tagmap);
void tagmap_stats(const TagMap* tagmap, uint64_t* collisions, uint64_t* max_chain_length);

// TagMap conversion
void tagmap_to_arrays(const TagMap* tagmap, Tag** keys, Tag** values, uint64_t* count);  // Caller must free

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_TAGMAP
