/*
Example usage of the C bindings for gdstk Cell functionality
*/

#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

int main() {
    // Create a new cell
    Cell* cell = cell_new("my_cell");
    if (!cell) {
        printf("Failed to create cell\n");
        return 1;
    }
    
    printf("Created cell successfully\n");
    
    // Print cell information
    cell_print(cell, true);
    
    // Get bounding box
    Vec2 min, max;
    cell_bounding_box(cell, &min, &max);
    printf("Bounding box: (%.2f, %.2f) to (%.2f, %.2f)\n", 
           min.x, min.y, max.x, max.y);
    
    // Test tag utility functions
    Tag tag = make_tag(1, 2);  // layer 1, type 2
    printf("Created tag with layer %u and type %u\n", 
           get_layer(tag), get_type(tag));
    
    // Modify tag
    set_layer(&tag, 5);
    set_type(&tag, 10);
    printf("Modified tag now has layer %u and type %u\n", 
           get_layer(tag), get_type(tag));
    
    // Clean up
    cell_free(cell);
    printf("Cell freed successfully\n");
    
    return 0;
}
