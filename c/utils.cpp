#define _USE_MATH_DEFINES

#include "utils.h"
#include <cmath>
#include <cstdlib>
#include "../include/gdstk/utils.hpp"
#include "../include/gdstk/allocator.hpp"

// Thin wrapper around GDSTK C++ utility functions

bool approx_equal(double a, double b, double tolerance) {
    return fabs(a - b) <= tolerance;
}

bool points_equal(Vec2 a, Vec2 b) {
    return approx_equal(a.x, b.x, 1e-9) && approx_equal(a.y, b.y, 1e-9);
}

// Memory allocation wrappers around GDSTK allocator
void* gdstk_allocate(uint64_t size) {
    return gdstk::allocate(size);
}

void* gdstk_reallocate(void* ptr, uint64_t size) {
    return gdstk::reallocate(ptr, size);
}

void gdstk_free(void* ptr) {
    gdstk::free_allocation(ptr);
}

void* gdstk_allocate_clear(uint64_t size) {
    return gdstk::allocate_clear(size);
}

// String utility wrapper
char* copy_string(const char* str, uint64_t* len) {
    return gdstk::copy_string(str, len);
}

// Math utility wrappers
double distance_to_line_sq(Vec2 p, Vec2 p1, Vec2 p2) {
    gdstk::Vec2 gp = {p.x, p.y};
    gdstk::Vec2 gp1 = {p1.x, p1.y};
    gdstk::Vec2 gp2 = {p2.x, p2.y};
    return gdstk::distance_to_line_sq(gp, gp1, gp2);
}

double distance_to_line(Vec2 p, Vec2 p1, Vec2 p2) {
    gdstk::Vec2 gp = {p.x, p.y};
    gdstk::Vec2 gp1 = {p1.x, p1.y};
    gdstk::Vec2 gp2 = {p2.x, p2.y};
    return gdstk::distance_to_line(gp, gp1, gp2);
}

bool is_multiple_of_pi_over_2(double angle, int64_t* m) {
    int64_t temp_m;
    bool result = gdstk::is_multiple_of_pi_over_2(angle, temp_m);
    if (m) *m = temp_m;
    return result;
}

uint64_t arc_num_points(double angle, double radius, double tolerance) {
    return gdstk::arc_num_points(angle, radius, tolerance);
}

double elliptical_angle_transform(double angle, double radius_x, double radius_y) {
    return gdstk::elliptical_angle_transform(angle, radius_x, radius_y);
}

void segments_intersection(Vec2 p0, Vec2 ut0, Vec2 p1, Vec2 ut1, double* u0, double* u1) {
    gdstk::Vec2 gp0 = {p0.x, p0.y};
    gdstk::Vec2 gut0 = {ut0.x, ut0.y};
    gdstk::Vec2 gp1 = {p1.x, p1.y};
    gdstk::Vec2 gut1 = {ut1.x, ut1.y};
    
    double temp_u0, temp_u1;
    gdstk::segments_intersection(gp0, gut0, gp1, gut1, temp_u0, temp_u1);
    
    if (u0) *u0 = temp_u0;
    if (u1) *u1 = temp_u1;
}

// Bezier curve evaluation wrappers
Vec2 eval_line(double t, Vec2 p0, Vec2 p1) {
    gdstk::Vec2 gp0 = {p0.x, p0.y};
    gdstk::Vec2 gp1 = {p1.x, p1.y};
    gdstk::Vec2 result = gdstk::eval_line(t, gp0, gp1);
    return {result.x, result.y};
}

Vec2 eval_bezier2(double t, Vec2 p0, Vec2 p1, Vec2 p2) {
    gdstk::Vec2 gp0 = {p0.x, p0.y};
    gdstk::Vec2 gp1 = {p1.x, p1.y};
    gdstk::Vec2 gp2 = {p2.x, p2.y};
    gdstk::Vec2 result = gdstk::eval_bezier2(t, gp0, gp1, gp2);
    return {result.x, result.y};
}

Vec2 eval_bezier3(double t, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3) {
    gdstk::Vec2 gp0 = {p0.x, p0.y};
    gdstk::Vec2 gp1 = {p1.x, p1.y};
    gdstk::Vec2 gp2 = {p2.x, p2.y};
    gdstk::Vec2 gp3 = {p3.x, p3.y};
    gdstk::Vec2 result = gdstk::eval_bezier3(t, gp0, gp1, gp2, gp3);
    return {result.x, result.y};
}

// Endian swap function wrappers
void big_endian_swap16(uint16_t* buffer, uint64_t n) {
    gdstk::big_endian_swap16(buffer, n);
}

void big_endian_swap32(uint32_t* buffer, uint64_t n) {
    gdstk::big_endian_swap32(buffer, n);
}

void big_endian_swap64(uint64_t* buffer, uint64_t n) {
    gdstk::big_endian_swap64(buffer, n);
}

void little_endian_swap16(uint16_t* buffer, uint64_t n) {
    gdstk::little_endian_swap16(buffer, n);
}

void little_endian_swap32(uint32_t* buffer, uint64_t n) {
    gdstk::little_endian_swap32(buffer, n);
}

void little_endian_swap64(uint64_t* buffer, uint64_t n) {
    gdstk::little_endian_swap64(buffer, n);
}

// Checksum calculation wrapper
uint32_t checksum32(uint32_t checksum, const uint8_t* bytes, uint64_t count) {
    return gdstk::checksum32(checksum, bytes, count);
}

// Hash function wrappers
uint64_t hash_uint64(uint64_t key) {
    return gdstk::hash(key);
}

uint64_t hash_string(const char* key) {
    return gdstk::hash(key);
}

// String representation wrapper
char* double_print(double value, uint32_t precision, char* buffer, uint64_t buffer_size) {
    return gdstk::double_print(value, precision, buffer, buffer_size);
}
