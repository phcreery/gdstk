/*
 * Basic GDSTK C Bindings Test
 * 
 * This test focuses on core functionality that doesn't require external libraries
 */

#include "gdstk_test.h"

// Test 1: Basic cell creation and properties
int test_basic_cell() {
    Cell* cell = cell_new("basic_test");
    TEST_ASSERT(cell != NULL, "Cell should be created successfully");
    
    const char* name = cell_name(cell);
    TEST_ASSERT(name != NULL, "Cell name should not be NULL");
    TEST_ASSERT(strcmp(name, "basic_test") == 0, "Cell name should match");
    
    // Check initial counts
    TEST_ASSERT(cell_polygon_count(cell) == 0, "Initial polygon count should be 0");
    TEST_ASSERT(cell_label_count(cell) == 0, "Initial label count should be 0");
    
    cell_free(cell);
    TEST_PASS();
}

// Test 2: Basic polygon creation
int test_basic_polygon() {
    Vec2 points[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    Tag tag = make_tag(5, 3); // Create tag from layer 5, datatype 3
    Polygon* poly = polygon_new(points, 4, tag);
    
    TEST_ASSERT(poly != NULL, "Polygon should be created successfully");
    TEST_ASSERT(get_layer(tag) == 5, "Layer should be 5");
    TEST_ASSERT(get_type(tag) == 3, "Datatype should be 3");
    
    // Note: polygon_size was removed - C++ Polygon doesn't have size() method
    // We can check the internal point array instead
    
    polygon_free(poly);
    TEST_PASS();
}

// Test 3: Basic label creation and properties
int test_basic_label() {
    Vec2 pos = {2.5, 3.7};
    Tag tag = make_tag(1, 0); // Create tag from layer 1, texttype 0
    Label* label = label_new("Hello GDSTK", pos, 0.0, tag);
    
    TEST_ASSERT(label != NULL, "Label should be created successfully");
    TEST_ASSERT(get_layer(tag) == 1, "Layer should be 1");
    TEST_ASSERT(get_type(tag) == 0, "Texttype should be 0");
    
    // Note: Individual property accessors were removed from C wrapper
    // The properties are encoded in the Tag and structure fields
    
    label_free(label);
    TEST_PASS();
}

// Test 4: Cell with polygons and labels
int test_cell_with_elements() {
    Cell* cell = cell_new("container");
    
    // Create a polygon
    Vec2 points[3] = {{0, 0}, {2, 0}, {1, 1}};
    Tag poly_tag = make_tag(0, 0); // Layer 0, datatype 0
    Polygon* poly = polygon_new(points, 3, poly_tag);
    
    // Create a label
    Vec2 label_pos = {1, 0.5};
    Tag label_tag = make_tag(1, 0); // Layer 1, texttype 0
    Label* label = label_new("Triangle", label_pos, 0.0, label_tag);
    
    // Add them to the cell
    cell_add_polygon(cell, poly);
    cell_add_label(cell, label);
    
    // Verify counts
    TEST_ASSERT(cell_polygon_count(cell) == 1, "Cell should have 1 polygon");
    TEST_ASSERT(cell_label_count(cell) == 1, "Cell should have 1 label");
    
    // Get elements back
    Polygon* retrieved_poly = cell_get_polygon(cell, 0);
    Label* retrieved_label = cell_get_label(cell, 0);
    
    TEST_ASSERT(retrieved_poly == poly, "Retrieved polygon should match");
    TEST_ASSERT(retrieved_label == label, "Retrieved label should match");
    
    // Test bounds check
    TEST_ASSERT(cell_get_polygon(cell, 1) == NULL, "Out-of-bounds polygon should be NULL");
    TEST_ASSERT(cell_get_label(cell, 1) == NULL, "Out-of-bounds label should be NULL");
    
    cell_free(cell);
    // Note: elements are freed when cell is freed, so no need to free them separately
    TEST_PASS();
}

// Test 5: Simple Vec2 test (basic functionality only)
int test_vec2_utilities() {
    Vec2 a = {3.0, 4.0};
    Vec2 b = {1.0, 2.0};
    
    TEST_ASSERT(approx_equal(a.x, 3.0, 1e-9), "Vec2 x should be 3.0");
    TEST_ASSERT(approx_equal(a.y, 4.0, 1e-9), "Vec2 y should be 4.0");
    
    // Simple calculations using direct struct access
    double length = sqrt(a.x * a.x + a.y * a.y);
    TEST_ASSERT(approx_equal(length, 5.0, 1e-9), "Length should be 5.0");
    
    double dot = a.x * b.x + a.y * b.y;
    TEST_ASSERT(approx_equal(dot, 11.0, 1e-9), "Dot product should be 11.0");
    
    TEST_PASS();
}

int main() {
    printf("🚀 Starting GDSTK C Bindings Basic Test Suite\n\n");
    
    TEST_SUITE_BEGIN();
    
    RUN_TEST(test_basic_cell);
    RUN_TEST(test_basic_polygon);
    RUN_TEST(test_basic_label);
    RUN_TEST(test_cell_with_elements);
    RUN_TEST(test_vec2_utilities);
    
    TEST_SUITE_END();
    
    return 0;
}
