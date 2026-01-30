# GDSTK Build Makefile
# Builds GDSTK library and dependencies from source

# Compiler settings
CC = gcc
CXX = g++
AR = ar

# Base directories
GDSTK_ROOT = .
SRC_DIR = $(GDSTK_ROOT)/src
C_WRAPPER_DIR = $(GDSTK_ROOT)/c
INCLUDE_DIR = $(GDSTK_ROOT)/include
EXTERNAL_DIR = $(GDSTK_ROOT)/external
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
C_WRAPPER_OBJ_DIR = $(BUILD_DIR)/obj/c

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(C_WRAPPER_DIR) -I$(EXTERNAL_DIR)/include -I$(EXTERNAL_DIR)

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Wshadow -Wvla -Wformat -Wno-missing-field-initializers -Wno-missing-braces -Wno-cast-function-type -Wno-unused-parameter -O2 -fPIC $(INCLUDES)
CFLAGS = -std=c99 -Wall -Wextra -Wshadow -Wvla -Wformat -Wno-missing-field-initializers -Wno-missing-braces -Wno-cast-function-type -Wno-unused-parameter -O2 -fPIC $(INCLUDES)

# GDSTK source files
GDSTK_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
GDSTK_OBJECTS = $(GDSTK_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# C wrapper source files
C_WRAPPER_SOURCES = $(wildcard $(C_WRAPPER_DIR)/*.cpp)
C_WRAPPER_OBJECTS = $(C_WRAPPER_SOURCES:$(C_WRAPPER_DIR)/%.cpp=$(C_WRAPPER_OBJ_DIR)/%.o)

# All objects
ALL_OBJECTS = $(GDSTK_OBJECTS) $(C_WRAPPER_OBJECTS)

# Target library
GDSTK_LIBRARY = $(LIB_DIR)/libgdstk.a
GDSTK_WITH_C_LIBRARY = $(LIB_DIR)/libgdstk_c.a

# Default target
all: $(GDSTK_LIBRARY) $(GDSTK_WITH_C_LIBRARY)

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR): $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

$(C_WRAPPER_OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(C_WRAPPER_OBJ_DIR)

# Build GDSTK objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build C wrapper objects
$(C_WRAPPER_OBJ_DIR)/%.o: $(C_WRAPPER_DIR)/%.cpp | $(C_WRAPPER_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the libraries
$(GDSTK_LIBRARY): $(GDSTK_OBJECTS) | $(LIB_DIR)
	$(AR) rcs $@ $(GDSTK_OBJECTS)
	@echo "GDSTK library built successfully: $@"

$(GDSTK_WITH_C_LIBRARY): $(ALL_OBJECTS) | $(LIB_DIR)
	$(AR) rcs $@ $(ALL_OBJECTS)
	@echo "GDSTK library with C wrapper built successfully: $@"

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Install (copy to standard locations)
install: $(GDSTK_LIBRARY) $(GDSTK_WITH_C_LIBRARY)
	mkdir -p lib include
	cp $(GDSTK_LIBRARY) lib/libgdstk.a
	cp $(GDSTK_WITH_C_LIBRARY) lib/libgdstk_c.a
#	cp -r $(INCLUDE_DIR)/gdstk include/
	cp $(C_WRAPPER_DIR)/*.h include/gdstk/

# Show build info
info:
	@echo "GDSTK Build Configuration"
	@echo "========================="
	@echo "Source directory: $(SRC_DIR)"
	@echo "C wrapper directory: $(C_WRAPPER_DIR)"
	@echo "Include directory: $(INCLUDE_DIR)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Compiler: $(CXX) $(CXXFLAGS)"
	@echo "GDSTK sources: $(words $(GDSTK_SOURCES)) files"
	@echo "C wrapper sources: $(words $(C_WRAPPER_SOURCES)) files"

# Additional targets
gdstk-only: $(GDSTK_LIBRARY)
	@echo "GDSTK C++ library only built: $(GDSTK_LIBRARY)"

c-wrapper: $(GDSTK_WITH_C_LIBRARY)
	@echo "GDSTK with C wrapper built: $(GDSTK_WITH_C_LIBRARY)"

.PHONY: all clean install info gdstk-only c-wrapper
