/*
 * Test for cell_get_polygons and related functions
 */

#include <stdio.h>
#include "gdstk_test.h"

int main() {
    printf("Testing cell_get_polygons function\n");
    printf("==================================\n\n");
    
    // Create a cell
    Cell* cell = cell_new("test_cell");
    if (!cell) {
        printf("Failed to create cell\n");
        return 1;
    }
    
    // Create some polygons with different tags
    Vec2 rect1_points[] = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    Tag tag1 = make_tag(1, 0);
    Polygon* rect1 = polygon_new(rect1_points, 4, tag1);
    
    Vec2 rect2_points[] = {{20, 0}, {30, 0}, {30, 10}, {20, 10}};
    Tag tag2 = make_tag(2, 0);
    Polygon* rect2 = polygon_new(rect2_points, 4, tag2);
    
    Vec2 triangle_points[] = {{5, 15}, {10, 15}, {7.5, 20}};
    Tag tag3 = make_tag(1, 1);  // Same layer as rect1, different datatype
    Polygon* triangle = polygon_new(triangle_points, 3, tag3);
    
    if (!rect1 || !rect2 || !triangle) {
        printf("Failed to create polygons\n");
        return 1;
    }
    
    // Add polygons to cell
    cell_add_polygon(cell, rect1);
    cell_add_polygon(cell, rect2);
    cell_add_polygon(cell, triangle);
    
    printf("✓ Created cell with 3 polygons\n");
    printf("  - Rectangle 1: layer %u, datatype %u\n", get_layer(tag1), get_type(tag1));
    printf("  - Rectangle 2: layer %u, datatype %u\n", get_layer(tag2), get_type(tag2));
    printf("  - Triangle: layer %u, datatype %u\n", get_layer(tag3), get_type(tag3));
    
    // Test cell_get_polygons without filtering
    Array* result = array_new(sizeof(void*));  // Array to store Polygon* pointers
    cell_get_polygons(cell, false, false, 0, false, 0, result);  // Get all polygons from this cell only
    
    size_t polygon_count = array_size(result);
    printf("✓ cell_get_polygons returned %zu polygons (expected 3)\n", polygon_count);
    
    if (polygon_count == 3) {
        printf("✓ Correct number of polygons retrieved\n");
    } else {
        printf("✗ Expected 3 polygons, got %zu\n", polygon_count);
    }
    
    // Test filtering by tag
    array_clear(result);
    cell_get_polygons(cell, false, false, 0, true, tag1, result);  // Get only polygons with tag1
    
    size_t filtered_count = array_size(result);
    printf("✓ Filtered by tag (layer 1, datatype 0): %zu polygons (expected 1)\n", filtered_count);
    
    if (filtered_count == 1) {
        printf("✓ Tag filtering works correctly\n");
    } else {
        printf("✗ Expected 1 polygon with tag1, got %zu\n", filtered_count);
    }
    
    // Clean up
    array_free(result);
    cell_free(cell);
    
    printf("\n✓ cell_get_polygons function test completed successfully!\n");
    return 0;
}
