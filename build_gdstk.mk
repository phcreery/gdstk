# GDSTK Build Makefile
# Builds GDSTK library and dependencies from source

# Compiler settings
CC = gcc
CXX = g++
AR = ar

# Base directories
GDSTK_ROOT = .
SRC_DIR = $(GDSTK_ROOT)/src
INCLUDE_DIR = $(GDSTK_ROOT)/include
EXTERNAL_DIR = $(GDSTK_ROOT)/external
BUILD_DIR = build_new
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(EXTERNAL_DIR)/include

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -fPIC $(INCLUDES)
CFLAGS = -std=c99 -Wall -Wextra -O2 -fPIC $(INCLUDES)

# GDSTK source files
GDSTK_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
GDSTK_OBJECTS = $(GDSTK_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Target library
GDSTK_LIBRARY = $(LIB_DIR)/libgdstk.a

# Default target
all: $(GDSTK_LIBRARY)

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR): $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# Build GDSTK objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the library
$(GDSTK_LIBRARY): $(GDSTK_OBJECTS) | $(LIB_DIR)
	$(AR) rcs $@ $(GDSTK_OBJECTS)
	@echo "GDSTK library built successfully: $@"

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Install (copy to standard locations)
install: $(GDSTK_LIBRARY)
	mkdir -p lib include
	cp $(GDSTK_LIBRARY) lib/libgdstk.a
	cp -r $(INCLUDE_DIR)/gdstk include/

# Show build info
info:
	@echo "GDSTK Build Configuration"
	@echo "========================="
	@echo "Source directory: $(SRC_DIR)"
	@echo "Include directory: $(INCLUDE_DIR)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Compiler: $(CXX) $(CXXFLAGS)"
	@echo "GDSTK sources: $(words $(GDSTK_SOURCES)) files"

.PHONY: all clean install info
