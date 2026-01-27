# Makefile for GDSTK C Bindings
# Provides different build targets for testing and full integration

# Notes about dependencies:
# The 'full' target requires:
# 1. GDSTK library to be built (../lib/gdstk.a)
# 2. All external dependencies (zlib, clipper, qhull) to be available

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

CC = gcc
CXX = g++
CFLAGS = -std=c99 -Wall -Wextra -O2
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -I../include

# Targets
.PHONY: all clean simple full test

# Default target
all: 

# C++ wrapper object file
cell.o: cell.cpp cell.h
	$(CXX) $(CXXFLAGS) -c cell.cpp -o cell.o

# Clean target
clean:
	rm -f *.exe *.o