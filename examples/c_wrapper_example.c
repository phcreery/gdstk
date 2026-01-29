/* 
 * GDSTK C Wrapper Example
 * 
 * This example demonstrates how to use the GDSTK C wrapper
 * built with the updated root Makefile_c.mk.
 *
 * To compile and run this example:
 * 1. Build the library: make -f Makefile_c.mk
 * 2. Compile this example: gcc -std=c11 -I./c -I./include c_wrapper_example.c -L./build/lib -lgdstk_c -lclipper -lqhullstatic_r -lz -lstdc++ -lm -o c_wrapper_example
 * 3. Run: ./c_wrapper_example
 */

#include <stdio.h>
#include <gdstk/gdstk.h>

int main() {
    printf("GDSTK C Wrapper Example\n");
    printf("=======================\n\n");
    
    // Create a simple polygon
    Vec2 points[] = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    Tag tag = make_tag(1, 0);  // Layer 1, datatype 0
    Polygon* rect = polygon_new(points, 4, tag);
    
    if (rect) {
        printf("Created rectangle polygon on layer %u, datatype %u\n", 
               get_layer(tag), get_type(tag));
        
        // Calculate area
        double area = polygon_area(rect);
        printf("Rectangle area: %.2f\n", area);
        
        // Clean up
        polygon_free(rect);
        printf("Polygon freed successfully\n");
    } else {
        printf("Failed to create polygon\n");
        return 1;
    }
    
    // Create a label
    Vec2 label_pos = {5, 5};
    Tag label_tag = make_tag(2, 1);  // Layer 2, texttype 1
    Label* label = label_new("Hello GDSTK!", label_pos, 0.0, label_tag);
    
    if (label) {
        printf("Created label on layer %u, texttype %u\n", 
               get_layer(label_tag), get_type(label_tag));
        
        // Clean up
        label_free(label);
        printf("Label freed successfully\n");
    } else {
        printf("Failed to create label\n");
        return 1;
    }
    
    // Create a cell and add elements
    Cell* cell = cell_new("example_cell");
    if (cell) {
        printf("Created cell: %s\n", cell_name(cell));
        
        // Create and add a polygon to the cell
        Vec2 triangle_points[] = {{15, 0}, {20, 0}, {17.5, 5}};
        Tag triangle_tag = make_tag(3, 0);
        Polygon* triangle = polygon_new(triangle_points, 3, triangle_tag);
        
        if (triangle) {
            cell_add_polygon(cell, triangle);
            printf("Added triangle to cell\n");
        }
        
        // Clean up
        cell_free(cell);
        printf("Cell freed successfully\n");
    } else {
        printf("Failed to create cell\n");
        return 1;
    }
    
    printf("\nGDSTK C wrapper working correctly!\n");
    return 0;
}
