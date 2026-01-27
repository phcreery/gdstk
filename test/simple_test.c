#include "../c/common.h"
#include "../c/cell.h" 
#include "../c/polygon.h"
#include "../c/label.h"

int main() {
    // Just test that headers compile
    Cell* cell = cell_new("test");
    if (cell) {
        cell_free(cell);
    }
    
    // Test creating a polygon
    Vec2 points[] = {{{0, 0}}, {{1, 0}}, {{1, 1}}, {{0, 1}}};
    Polygon* poly = polygon_new(points, 4, 5, 3);
    if (poly) {
        polygon_free(poly);
    }
    
    // Test creating a label
    Vec2 origin = {1, 2};
    Label* label = label_new("test", origin, 0, 10, 0);
    if (label) {
        label_free(label);
    }
    
    return 0;
}
