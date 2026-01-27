# Makefile for GDSTK C Test Suite
#
# This Makefile builds the C test files for the GDSTK C bindings.

# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall -Wextra -O2 -I../c -I../include -I../external/qhull/src
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -I../include -I../c
LIBS = -L../lib -L../external/zlib -L../external/qhull/lib -L../external/clipper -l:gdstk.a -lqhullstatic_r -lz -lclipper -lstdc++ -lm

# Test framework
TEST_FRAMEWORK_OBJS = gdstk_test.o

# Simplified wrapper library
WRAPPER_LIB = wrappers.o

# Default target
all: minimal_test simple_test basic_test cell_test

# Build framework
gdstk_test.o: gdstk_test.c gdstk_test.h
	$(CC) $(CFLAGS) -c gdstk_test.c -o gdstk_test.o

# Build the wrapper library first
$(WRAPPER_LIB): wrappers.cpp ../c/common.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Update all test targets to depend on wrapper library
minimal_test: minimal_test.c $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $< $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS) -o $@

simple_test: simple_test.c $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $< $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS) -o $@

basic_test: basic_test.c $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $< $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS) -o $@

cell_test: cell_test.c $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $< $(WRAPPER_LIB) $(TEST_FRAMEWORK_OBJS) -o $@

# Quick test
test: basic_test
	./basic_test

# Clean
clean:
	rm -f *.o *.exe minimal_test simple_test basic_test cell_test

.PHONY: all test clean
