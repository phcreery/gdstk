# Makefile for GDSTK C Test Suite
#
# This Makefile builds the C test files for the GDSTK C bindings.

# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall -Wextra -O2 -I../../c -I../../include
CXXFLAGS = -std=c++11 -Wall -Wextra -Wshadow -Wvla -Wformat -Wno-missing-field-initializers -Wno-missing-braces -Wno-cast-function-type -Wno-unused-parameter -O2 -I../../include -I../../c
LIBS = -L../../build/lib -L../../external/lib -lgdstk_c -lclipper -lqhullstatic_r -lz -lstdc++ -lm

# Test framework
TEST_FRAMEWORK_OBJS = gdstk_test.o

# Default target - build working tests
all: basic_test test_basic cell_test curve_test test_library_api test_get_polygons

# Build framework
gdstk_test.o: gdstk_test.c gdstk_test.h
	$(CC) $(CFLAGS) -c gdstk_test.c -o gdstk_test.o

# Build tests using the combined library (C wrapper is already included in libgdstk_c.a)
basic_test: basic_test.c $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

test_basic: test_basic.c
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@

cell_test: cell_test.c $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

curve_test: curve_test.c $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

test_library_api: test_library_api.c $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

test_get_polygons: test_get_polygons.c $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

# Quick test
test: basic_test
	./basic_test

# Clean
clean:
	rm -f *.o *.exe basic_test test_basic cell_test curve_test test_library_api test_get_polygons

.PHONY: all test clean
