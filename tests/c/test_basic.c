#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "vec.h"

// Simple test functions that don't depend on full GDSTK library
bool approx_equal(double a, double b, double tolerance) {
    return fabs(a - b) <= tolerance;
}

bool points_equal(Vec2 a, Vec2 b) {
    return approx_equal(a.x, b.x, 1e-9) && approx_equal(a.y, b.y, 1e-9);
}

int main() {
    printf("Testing Basic C Utils and Vec2 Functions\n");
    printf("========================================\n");
    
    // Test floating point comparison
    printf("Testing floating point comparisons...\n");
    assert(approx_equal(1.0, 1.000000001, 1e-8));
    assert(!approx_equal(1.0, 1.1, 1e-8));
    printf("  ✓ approx_equal works\n");
    
    Vec2 p1 = {1.0, 2.0};
    Vec2 p2 = {1.0, 2.0};  // Exact same values
    Vec2 p3 = {1.1, 2.1};
    assert(points_equal(p1, p2));
    assert(!points_equal(p1, p3));
    printf("  ✓ points_equal works\n");
    
    // Test Vec2 operations
    printf("\nTesting Vec2 operations...\n");
    Vec2 v1 = {3.0, 4.0};
    Vec2 v2 = {1.0, 2.0};
    
    Vec2 sum = vec2_add(v1, v2);
    assert(approx_equal(sum.x, 4.0, 1e-10));
    assert(approx_equal(sum.y, 6.0, 1e-10));
    printf("  ✓ vec2_add works: (%.1f, %.1f)\n", sum.x, sum.y);
    
    Vec2 diff = vec2_subtract(v1, v2);
    assert(approx_equal(diff.x, 2.0, 1e-10));
    assert(approx_equal(diff.y, 2.0, 1e-10));
    printf("  ✓ vec2_subtract works: (%.1f, %.1f)\n", diff.x, diff.y);
    
    Vec2 scaled = vec2_multiply_scalar(v1, 2.0);
    assert(approx_equal(scaled.x, 6.0, 1e-10));
    assert(approx_equal(scaled.y, 8.0, 1e-10));
    printf("  ✓ vec2_multiply_scalar works: (%.1f, %.1f)\n", scaled.x, scaled.y);
    
    double len = vec2_length(v1);
    assert(approx_equal(len, 5.0, 1e-10));  // sqrt(3^2 + 4^2) = 5
    printf("  ✓ vec2_length works: %.1f\n", len);
    
    double dot = vec2_dot(v1, v2);
    assert(approx_equal(dot, 11.0, 1e-10));  // 3*1 + 4*2 = 11
    printf("  ✓ vec2_dot works: %.1f\n", dot);
    
    // Test normalize
    Vec2 unit = vec2_normalize(v1);
    double unit_len = vec2_length(unit);
    assert(approx_equal(unit_len, 1.0, 1e-10));
    printf("  ✓ vec2_normalize works: length = %.6f\n", unit_len);
    
    printf("\n========================================\n");
    printf("All basic tests passed! C utils foundation is working.\n");
    return 0;
}
