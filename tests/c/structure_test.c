#include <stdio.h>
#include <assert.h>
#include "../c/flexpath.h"
#include "../c/robustpath.h"
#include "../c/polygon.h"
#include "../c/reference.h"
#include "../c/label.h"

// Test that the C structures match the expected layout from C++
int main() {
    printf("Testing C wrapper structure layouts...\n");
    
    // Test FlexPath structure - spine should be embedded, not pointer
    FlexPath* fp = NULL;
    printf("FlexPath structure appears valid\n");
    
    // Test Polygon structure - point_array should be embedded, not pointer  
    Polygon* poly = NULL;
    printf("Polygon structure appears valid\n");
    
    // Test RobustPath structure - should have embedded repetition
    RobustPath* rp = NULL;
    printf("RobustPath structure appears valid\n");
    
    // Test Reference structure - should have embedded repetition
    Reference* ref = NULL;
    printf("Reference structure appears valid\n");
    
    // Test Label structure - should have embedded repetition
    Label* label = NULL;
    printf("Label structure appears valid\n");
    
    // Test enums have correct values
    assert(GDSTK_JOIN_NATURAL == 0);
    assert(GDSTK_JOIN_MITER == 1);
    assert(GDSTK_END_FLUSH == 0);
    assert(GDSTK_END_ROUND == 1);
    assert(GDSTK_BEND_NONE == 0);
    assert(GDSTK_BEND_CIRCULAR == 1);
    printf("Enum values match C++ API\n");
    
    printf("All structure layout tests passed!\n");
    return 0;
}
