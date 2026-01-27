#ifndef GDSTK_C_HEADER_PROPERTY
#define GDSTK_C_HEADER_PROPERTY

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OasisStream OasisStream;
typedef struct OasisState OasisState;
typedef enum ErrorCode ErrorCode;

// Property value types
typedef enum {
    GDSTK_PROPERTY_UNSIGNED_INTEGER = 0,
    GDSTK_PROPERTY_INTEGER,
    GDSTK_PROPERTY_REAL, 
    GDSTK_PROPERTY_STRING
} PropertyType;

// Property value structure (linked list node)
typedef struct PropertyValue {
    PropertyType type;
    union {
        uint64_t unsigned_integer;
        int64_t integer;
        double real;
        struct {
            uint64_t count;
            uint8_t* bytes;
        };
    };
    struct PropertyValue* next;
} PropertyValue;

// Property structure (linked list node)
typedef struct Property {
    char* name;
    PropertyValue* value;
    struct Property* next;
} Property;

// Property management functions
Property* property_new(const char* name);
void property_free(Property* property);
void properties_clear(Property** properties);
Property* properties_copy(const Property* properties);
void properties_print(const Property* properties);

// Property value management
PropertyValue* property_value_new(PropertyType type);
void property_value_free(PropertyValue* value);
PropertyValue* property_values_copy(const PropertyValue* values);

// Property setting functions
void property_set_unsigned_integer(Property** properties, const char* name, 
                                  uint64_t unsigned_integer, bool create_new);
void property_set_integer(Property** properties, const char* name, 
                         int64_t integer, bool create_new);
void property_set_real(Property** properties, const char* name, 
                      double real, bool create_new);
void property_set_string(Property** properties, const char* name, 
                        const char* string, bool create_new);
void property_set_bytes(Property** properties, const char* name, 
                       const uint8_t* bytes, uint64_t count, bool create_new);

// GDS-specific property functions
void property_set_gds(Property** properties, uint16_t attribute, 
                     const char* value, uint64_t count);

// Property removal functions
uint64_t property_remove(Property** properties, const char* name, bool all_occurences);
void property_remove_all(Property** properties);

// Property lookup functions
Property* property_find(const Property* properties, const char* name);
PropertyValue* property_get_value(const Property* properties, const char* name);
uint64_t property_get_unsigned_integer(const Property* properties, const char* name, 
                                      uint64_t default_value);
int64_t property_get_integer(const Property* properties, const char* name, 
                            int64_t default_value);
double property_get_real(const Property* properties, const char* name, 
                        double default_value);
const char* property_get_string(const Property* properties, const char* name);
const uint8_t* property_get_bytes(const Property* properties, const char* name, 
                                 uint64_t* count);

// Property iteration functions
Property* property_first(const Property* properties);
Property* property_next(const Property* property);
uint64_t property_count(const Property* properties);

// Property value iteration functions
PropertyValue* property_value_first(const Property* property);
PropertyValue* property_value_next(const PropertyValue* value);
uint64_t property_value_count(const Property* property);

// Property comparison functions
bool property_equal(const Property* prop1, const Property* prop2);
bool property_value_equal(const PropertyValue* val1, const PropertyValue* val2);

// Property serialization functions
ErrorCode properties_to_gds(const Property* properties, FILE* out);
ErrorCode properties_to_oas(const Property* properties, OasisStream* out, 
                           OasisState* state);

// Property validation functions
bool property_name_valid(const char* name);
bool property_name_valid_oasis(const char* name);

// Property utility functions
char* property_to_string(const Property* property);  // Caller must free
char* property_value_to_string(const PropertyValue* value);  // Caller must free
void property_dump(const Property* properties, FILE* out);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_PROPERTY
