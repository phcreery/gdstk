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
    Polygon* poly = polygon_new(points, 4, 5, 3);
    
    TEST_ASSERT(poly != NULL, "Polygon should be created successfully");
    TEST_ASSERT(polygon_layer(poly) == 5, "Layer should be 5");
    TEST_ASSERT(polygon_datatype(poly) == 3, "Datatype should be 3");
    TEST_ASSERT(polygon_size(poly) == 4, "Polygon should have 4 points");
    
    // Test property setters
    polygon_set_layer(poly, 10);
    polygon_set_datatype(poly, 7);
    TEST_ASSERT(polygon_layer(poly) == 10, "Layer should be updated to 10");
    TEST_ASSERT(polygon_datatype(poly) == 7, "Datatype should be updated to 7");
    
    polygon_free(poly);
    TEST_PASS();
}

// Test 3: Basic label creation and properties
int test_basic_label() {
    Vec2 pos = {2.5, 3.7};
    Label* label = label_new("Hello GDSTK", pos, 0.0, 1, 0);
    
    TEST_ASSERT(label != NULL, "Label should be created successfully");
    TEST_ASSERT(label_layer(label) == 1, "Layer should be 1");
    TEST_ASSERT(label_texttype(label) == 0, "Texttype should be 0");
    
    const char* text = label_text(label);
    TEST_ASSERT(text != NULL, "Label text should not be NULL");
    TEST_ASSERT(strcmp(text, "Hello GDSTK") == 0, "Label text should match");
    
    Vec2 origin = label_origin(label);
    TEST_ASSERT(approx_equal(origin.x, 2.5, 1e-9), "X position should be 2.5");
    TEST_ASSERT(approx_equal(origin.y, 3.7, 1e-9), "Y position should be 3.7");
    
    // Test property updates
    label_set_text(label, "Updated text");
    text = label_text(label);
    TEST_ASSERT(strcmp(text, "Updated text") == 0, "Label text should be updated");
    
    Vec2 new_pos = {10.0, 20.0};
    label_set_origin(label, new_pos);
    origin = label_origin(label);
    TEST_ASSERT(approx_equal(origin.x, 10.0, 1e-9), "X position should be updated");
    TEST_ASSERT(approx_equal(origin.y, 20.0, 1e-9), "Y position should be updated");
    
    label_free(label);
    TEST_PASS();
}

// Test 4: Cell with polygons and labels
int test_cell_with_elements() {
    Cell* cell = cell_new("container");
    
    // Create a polygon
    Vec2 points[3] = {{0, 0}, {2, 0}, {1, 1}};
    Polygon* poly = polygon_new(points, 3, 0, 0);
    
    // Create a label
    Vec2 label_pos = {1, 0.5};
    Label* label = label_new("Triangle", label_pos, 0.0, 1, 0);
    
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

// Test 5: Vec2 utility functions
int test_vec2_utilities() {
    Vec2 a = vec2_new(3.0, 4.0);
    Vec2 b = vec2_new(1.0, 2.0);
    
    TEST_ASSERT(approx_equal(a.x, 3.0, 1e-9), "Vec2 x should be 3.0");
    TEST_ASSERT(approx_equal(a.y, 4.0, 1e-9), "Vec2 y should be 4.0");
    
    Vec2 sum = vec2_add(a, b);
    TEST_ASSERT(approx_equal(sum.x, 4.0, 1e-9), "Sum x should be 4.0");
    TEST_ASSERT(approx_equal(sum.y, 6.0, 1e-9), "Sum y should be 6.0");
    
    Vec2 diff = vec2_subtract(a, b);
    TEST_ASSERT(approx_equal(diff.x, 2.0, 1e-9), "Diff x should be 2.0");
    TEST_ASSERT(approx_equal(diff.y, 2.0, 1e-9), "Diff y should be 2.0");
    
    double length = vec2_length(a);
    TEST_ASSERT(approx_equal(length, 5.0, 1e-9), "Length should be 5.0");
    
    double dot = vec2_dot(a, b);
    TEST_ASSERT(approx_equal(dot, 11.0, 1e-9), "Dot product should be 11.0");
    
    TEST_ASSERT(points_equal(a, a), "Point should equal itself");
    TEST_ASSERT(!points_equal(a, b), "Different points should not be equal");
    
    TEST_PASS();
}

int main() {
    printf("🚀 Starting GDSTK C Bindings Basic Test Suite\n\n");
    
    TEST_SUITE_BEGIN();
    
    TEST_RUN(test_basic_cell);
    TEST_RUN(test_basic_polygon);
    TEST_RUN(test_basic_label);
    TEST_RUN(test_cell_with_elements);
    TEST_RUN(test_vec2_utilities);
    
    TEST_SUITE_END();
    
    return 0;
}
