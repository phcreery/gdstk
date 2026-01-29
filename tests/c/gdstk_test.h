/*
 * GDSTK C Test Framework Header
 * 
 * This file provides a simple testing framework for the GDSTK C bindings,
 * including assertion macros, helper functions, and common test fixtures.
 * 
 * Copyright 2020 Lucas Heitzmann Gabrielli.
 * This file is part of gdstk, distributed under the terms of the
 * Boost Software License - Version 1.0.
 */

#ifndef GDSTK_TEST_H
#define GDSTK_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gdstk.h"

// Testing constants
#define TOLERANCE 1e-9

// Test framework macros
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s - %s\n", __func__, message); \
            return 0; \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_WITHIN(tolerance, expected, actual, message) \
    do { \
        if (fabs((expected) - (actual)) > (tolerance)) { \
            printf("FAIL: %s - %s (expected: %g, actual: %g, diff: %g)\n", \
                   __func__, message, (double)(expected), (double)(actual), \
                   fabs((double)(expected) - (double)(actual))); \
            return; \
        } \
    } while(0)

#define TEST_PASS() \
    do { \
        printf("PASS: %s\n", __func__); \
        return 1; \
    } while(0)

#define RUN_TEST(test_func) \
    do { \
        printf("Running " #test_func "...\n"); \
        (test_func)(); \
        increment_test_counters(); \
    } while(0)

#define TEST_SUITE_BEGIN() \
    int total_tests = 0; \
    int passed_tests = 0; \
    printf("Running GDSTK C Bindings Test Suite\n"); \
    printf("====================================\n\n");

#define TEST_SUITE_END() \
    printf("\n====================================\n"); \
    printf("Test Results: %d/%d tests passed\n", passed_tests, total_tests); \
    if (passed_tests == total_tests) { \
        printf("🎉 All tests passed!\n"); \
        return 0; \
    } else { \
        printf("❌ %d tests failed\n", total_tests - passed_tests); \
        return 1; \
    }

// Test framework functions
void init_test_results(void);
void increment_test_counters(void);
int print_test_results(void);

// Helper functions
bool vec2_equal(Vec2 a, Vec2 b);

// Utility function (use static in source files to avoid multiple definition)
// static bool approx_equal(double a, double b, double tolerance);

#endif // GDSTK_TEST_H
