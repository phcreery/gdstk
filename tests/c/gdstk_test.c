/*
 * GDSTK C Test Framework Implementation
 * 
 * This file implements the testing framework for the GDSTK C bindings,
 * providing helper functions and test fixtures for geometric operations.
 * 
 * Copyright 2020 Lucas Heitzmann Gabrielli.
 * This file is part of gdstk, distributed under the terms of the
 * Boost Software License - Version 1.0.
 */

#include "gdstk_test.h"

// Global test counters
static int total_tests = 0;
static int passed_tests = 0;

// Initialize test results
void init_test_results() {
    total_tests = 0;
    passed_tests = 0;
}

// Increment test counters (called by RUN_TEST macro)
void increment_test_counters() {
    total_tests++;
    passed_tests++;
}

// Print final test results
int print_test_results() {
    printf("\n====================================\n");
    printf("Test Results: %d/%d tests passed\n", passed_tests, total_tests);
    if (passed_tests == total_tests) {
        printf("🎉 All tests passed!\n");
        return 0;
    } else {
        printf("❌ %d tests failed\n", total_tests - passed_tests);
        return 1;
    }
}

// Helper function to compare Vec2 points
bool vec2_equal(Vec2 a, Vec2 b) {
    return fabs(a.x - b.x) < TOLERANCE && fabs(a.y - b.y) < TOLERANCE;
}

// Helper function to compare floating point numbers
static bool approx_equal(double a, double b, double tolerance) {
    return fabs(a - b) <= tolerance;
}
