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

#define TEST_PASS() \
    do { \
        printf("PASS: %s\n", __func__); \
        return 1; \
    } while(0)

#define TEST_RUN(test_func) \
    do { \
        total_tests++; \
        if ((test_func)()) { \
            passed_tests++; \
        } \
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

#endif // GDSTK_TEST_H
