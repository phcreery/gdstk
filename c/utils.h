#ifndef GDSTK_C_HEADER_UTILS
#define GDSTK_C_HEADER_UTILS

#include "common.h"
#include "vec.h"

#ifdef __cplusplus
extern "C" {
#endif

// Utility functions for floating point comparisons
bool approx_equal(double a, double b, double tolerance);
bool points_equal(Vec2 a, Vec2 b);

// Memory management functions (wrappers around GDSTK allocator)
void* gdstk_allocate(uint64_t size);
void* gdstk_reallocate(void* ptr, uint64_t size);
void gdstk_free(void* ptr);
void* gdstk_allocate_clear(uint64_t size);

// String utility functions
char* copy_string(const char* str, uint64_t* len);

// Math utility functions
double distance_to_line_sq(Vec2 p, Vec2 p1, Vec2 p2);
double distance_to_line(Vec2 p, Vec2 p1, Vec2 p2);
bool is_multiple_of_pi_over_2(double angle, int64_t* m);
uint64_t arc_num_points(double angle, double radius, double tolerance);
double elliptical_angle_transform(double angle, double radius_x, double radius_y);
void segments_intersection(Vec2 p0, Vec2 ut0, Vec2 p1, Vec2 ut1, double* u0, double* u1);

// Bezier curve evaluation functions
Vec2 eval_line(double t, Vec2 p0, Vec2 p1);
Vec2 eval_bezier2(double t, Vec2 p0, Vec2 p1, Vec2 p2);
Vec2 eval_bezier3(double t, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3);

// Endian swap functions (for file I/O)
void big_endian_swap16(uint16_t* buffer, uint64_t n);
void big_endian_swap32(uint32_t* buffer, uint64_t n);
void big_endian_swap64(uint64_t* buffer, uint64_t n);
void little_endian_swap16(uint16_t* buffer, uint64_t n);
void little_endian_swap32(uint32_t* buffer, uint64_t n);
void little_endian_swap64(uint64_t* buffer, uint64_t n);

// Checksum calculation
uint32_t checksum32(uint32_t checksum, const uint8_t* bytes, uint64_t count);

// Hash function for basic types
uint64_t hash_uint64(uint64_t key);
uint64_t hash_string(const char* key);

// String representation of doubles for output
char* double_print(double value, uint32_t precision, char* buffer, uint64_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif // GDSTK_C_HEADER_UTILS
