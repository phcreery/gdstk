# Makefile for GDSTK C Test Suite
#
# This Makefile builds the C test files for the GDSTK C bindings.

# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -std=c11 -Wall -Wextra -O2 -I../c -I../include
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -I../include -I../c
LIBS = -L../build_new/lib -L../external/lib -lgdstk -lclipper -lqhullstatic_r -lz -lstdc++ -lm

# Test framework
TEST_FRAMEWORK_OBJS = gdstk_test.o

# C wrapper sources from the c/ directory
WRAPPER_SOURCES = ../c/cell.cpp ../c/polygon.cpp ../c/label.cpp ../c/reference.cpp \
                  ../c/library.cpp ../c/utils.cpp ../c/curve.cpp
WRAPPER_OBJS = $(WRAPPER_SOURCES:../c/%.cpp=../c/%.o)

# Default target
all: basic_test cell_test curve_test

# Build framework
gdstk_test.o: gdstk_test.c gdstk_test.h
	$(CC) $(CFLAGS) -c gdstk_test.c -o gdstk_test.o

# Build C wrapper objects
../c/%.o: ../c/%.cpp ../c/%.h ../c/common.h
	$(CXX) $(CXXFLAGS) -I../include -c $< -o $@

basic_test: basic_test.c $(WRAPPER_OBJS) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(WRAPPER_OBJS) $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

cell_test: cell_test.c $(WRAPPER_OBJS) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(WRAPPER_OBJS) $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

curve_test: curve_test.c $(WRAPPER_OBJS) $(TEST_FRAMEWORK_OBJS)
	$(CC) $(CFLAGS) $< $(WRAPPER_OBJS) $(TEST_FRAMEWORK_OBJS) $(LIBS) -o $@

# Quick test
test: basic_test
	./basic_test

# Clean
clean:
	rm -f *.o *.exe basic_test cell_test curve_test
	rm -f ../c/*.o

.PHONY: all test clean
