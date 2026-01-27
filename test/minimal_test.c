/*
 * Minimal GDSTK Cell Test in C
 * 
 * BUILD INSTRUCTIONS:
 * gcc -std=c11 -I../c -I../include -L../lib -o minimal_test minimal_test.c ../c/cell.cpp ../c/polygon.cpp ../c/label.cpp ../c/reference.cpp -lgdstk -lstdc++ -lm
 */

#include "gdstk_test.h"

// Simple test 1: Create a cell and verify it exists
int test_cell_creation() {
    Cell* cell = cell_new("test_cell");
    TEST_ASSERT(cell != NULL, "Cell should be created successfully");
    cell_free(cell);
    TEST_PASS();
}

// Simple test 2: Create a polygon and verify its properties
int test_polygon_creation() {
    Vec2 points[] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    Polygon* poly = polygon_new(points, 4, 5, 3);
    
    TEST_ASSERT(poly != NULL, "Polygon should be created successfully");
    TEST_ASSERT(polygon_layer(poly) == 5, "Layer should be 5");
    TEST_ASSERT(polygon_datatype(poly) == 3, "Datatype should be 3");
    
    double area = polygon_area(poly);
    TEST_ASSERT(approx_equal(area, 1.0, TOLERANCE), "Area should be 1.0");
    
    polygon_free(poly);
    TEST_PASS();
}

// Simple test 3: Create a label
int test_label_creation() {
    Label* label = label_new("test", (Vec2){1, 2}, 0, 10, 0);
    
    TEST_ASSERT(label != NULL, "Label should be created successfully");
    TEST_ASSERT(label_layer(label) == 10, "Layer should be 10");
    
    Vec2 pos = label_origin(label);
    TEST_ASSERT(points_equal(pos, (Vec2){1, 2}), "Position should be (1, 2)");
    
    label_free(label);
    TEST_PASS();
}

int main() {
    TEST_SUITE_BEGIN();
    
    TEST_RUN(test_cell_creation);
    TEST_RUN(test_polygon_creation);
    TEST_RUN(test_label_creation);
    
    TEST_SUITE_END();
}
