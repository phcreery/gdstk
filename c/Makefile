# Makefile for GDSTK C Bindings
# Provides different build targets for testing and full integration

CC = gcc
CXX = g++
CFLAGS = -std=c99 -Wall -Wextra -O2
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -I../include

# Targets
.PHONY: all clean simple full test help

# Default target
all: simple

# Simple standalone examples (no GDSTK library required)
simple: comprehensive_example_simple.exe

# Full examples (requires GDSTK library)
full: example.exe comprehensive_example.exe

# Test target
test: comprehensive_example_simple.exe
	@echo "Running simple comprehensive example..."
	./comprehensive_example_simple.exe

# Help target
help:
	@echo "GDSTK C Bindings Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  simple  - Build standalone examples (default)"
	@echo "  full    - Build examples with GDSTK library"
	@echo "  test    - Run simple example"
	@echo "  clean   - Remove all built files"
	@echo "  help    - Show this help"
	@echo ""
	@echo "Simple examples work without the full GDSTK library."
	@echo "Full examples require the GDSTK library to be built first."

# Simple standalone example
# (Remove this section since we don't have simple_example.c)

# Simple version of comprehensive example
comprehensive_example_simple.exe: comprehensive_example.c
	$(CC) $(CFLAGS) -DSIMPLE_VERSION $< -o $@ -lm

# Full example with GDSTK (requires library)
example.exe: example.c cell.o
	$(CXX) $(CXXFLAGS) example.c cell.o ../lib/gdstk.a -o $@

# Full comprehensive example with GDSTK
comprehensive_example.exe: comprehensive_example.c cell.o
	$(CXX) $(CXXFLAGS) comprehensive_example.c cell.o ../lib/gdstk.a -o $@

# C++ wrapper object file
cell.o: cell.cpp cell.h
	$(CXX) $(CXXFLAGS) -c cell.cpp -o cell.o

# Clean target
clean:
	rm -f *.exe *.o

# Notes about dependencies:
# The 'full' target requires:
# 1. GDSTK library to be built (../lib/gdstk.a)
# 2. All external dependencies (zlib, clipper, qhull) to be available
# 
# For development/testing, use 'make simple' which builds standalone versions
# that demonstrate the interface without requiring the full GDSTK build.

# Platform-specific notes:
# On Windows with MinGW/MSYS2:
#   make simple
# On Linux/macOS:
#   make simple
# 
# For full GDSTK integration, first build GDSTK with CMake:
#   cd ..
#   mkdir build && cd build
#   cmake ..
#   cmake --build .
#   cd ../c
#   make full
