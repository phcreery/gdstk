#ifndef GDSTK_C_HEADER_SET
#define GDSTK_C_HEADER_SET

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef uint64_t Tag;

// Set item structure
typedef struct SetItem {
    void* value;        // Generic value pointer
    size_t value_size;  // Size of the value
    bool valid;         // Whether this slot is valid
} SetItem;

// Generic Set structure
typedef struct Set {
    uint64_t capacity;  // allocated capacity
    uint64_t count;     // number of items in the set
    SetItem* items;     // array with length capacity
    size_t value_size;  // size of each value
} Set;

// Generic set functions
Set* set_new(size_t value_size);
void set_free(Set* set);
void set_clear(Set* set);
void set_print(const Set* set, bool show_all);

// Set operations
bool set_add(Set* set, const void* value);     // Returns true if value was newly added
bool set_remove(Set* set, const void* value);  // Returns true if value was found and removed
bool set_contains(const Set* set, const void* value);
uint64_t set_size(const Set* set);
uint64_t set_capacity(const Set* set);
bool set_empty(const Set* set);

// Set iteration
SetItem* set_first_valid(const Set* set);
SetItem* set_next_valid(const Set* set, const SetItem* current);
void set_to_array(const Set* set, void** values, uint64_t* count);  // Caller must free

// Set operations
Set* set_copy(const Set* source);
void set_copy_from(Set* dest, const Set* source);
Set* set_union(const Set* set1, const Set* set2);
Set* set_intersection(const Set* set1, const Set* set2);
Set* set_difference(const Set* set1, const Set* set2);
Set* set_symmetric_difference(const Set* set1, const Set* set2);

// Set comparison
bool set_equal(const Set* set1, const Set* set2);
bool set_is_subset(const Set* set1, const Set* set2);
bool set_is_superset(const Set* set1, const Set* set2);
bool set_is_disjoint(const Set* set1, const Set* set2);

// Specialized set types for common uses

// Set of integers (int64_t)
Set* set_int_new(void);
bool set_int_add(Set* set, int64_t value);
bool set_int_remove(Set* set, int64_t value);
bool set_int_contains(const Set* set, int64_t value);

// Set of unsigned integers (uint64_t)
Set* set_uint64_new(void);
bool set_uint64_add(Set* set, uint64_t value);
bool set_uint64_remove(Set* set, uint64_t value);
bool set_uint64_contains(const Set* set, uint64_t value);

// Set of doubles
Set* set_double_new(void);
bool set_double_add(Set* set, double value);
bool set_double_remove(Set* set, double value);
bool set_double_contains(const Set* set, double value);

// Set of strings (const char*)
Set* set_string_new(void);
bool set_string_add(Set* set, const char* value);
bool set_string_remove(Set* set, const char* value);
bool set_string_contains(const Set* set, const char* value);

// Set of pointers (void*)
Set* set_ptr_new(void);
bool set_ptr_add(Set* set, void* value);
bool set_ptr_remove(Set* set, void* value);
bool set_ptr_contains(const Set* set, void* value);

// Specialized sets for GDSTK types

// Set of Tags
Set* set_tag_new(void);
bool set_tag_add(Set* set, Tag value);
bool set_tag_remove(Set* set, Tag value);
bool set_tag_contains(const Set* set, Tag value);
void set_tag_to_array(const Set* set, Tag** values, uint64_t* count);  // Caller must free

// Set utility functions
void set_reserve(Set* set, uint64_t capacity);
void set_shrink_to_fit(Set* set);

// Set validation and statistics
bool set_is_valid(const Set* set);
double set_load_factor(const Set* set);
void set_stats(const Set* set, uint64_t* collisions, uint64_t* max_chain_length);

// Set transformation functions
Set* set_map(const Set* source, void (*map_func)(const void* input, void* output, void* data), 
            size_t output_value_size, void* data);
Set* set_filter(const Set* source, bool (*filter_func)(const void* value, void* data), void* data);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_SET
