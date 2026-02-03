/*
 * Test library array access with new getter functions
 * Demonstrates how users can access library internals safely
 */

#include "gdstk_test.h"

int test_library_access() {
    // Create library
    Library* lib = library_new("test_lib", 1e-6, 1e-9);
    TEST_ASSERT(lib != NULL, "Library should be created");
    
    // Test basic getters
    const char* name = library_name(lib);
    TEST_ASSERT(strcmp(name, "test_lib") == 0, "Library name should match");
    
    double unit = library_unit(lib);
    TEST_ASSERT(approx_equal(unit, 1e-6, 1e-12), "Library unit should match");
    
    double precision = library_precision(lib);
    TEST_ASSERT(approx_equal(precision, 1e-9, 1e-15), "Library precision should match");
    
    // Test array access
    Array* cell_array = library_cell_array(lib);
    TEST_ASSERT(cell_array != NULL, "Cell array should be accessible");
    
    Array* rawcell_array = library_rawcell_array(lib);
    TEST_ASSERT(rawcell_array != NULL, "RawCell array should be accessible");
    
    // Test counts
    uint64_t cell_count = library_cell_count(lib);
    TEST_ASSERT(cell_count == 0, "Initial cell count should be 0");
    
    uint64_t rawcell_count = library_rawcell_count(lib);
    TEST_ASSERT(rawcell_count == 0, "Initial rawcell count should be 0");
    
    // Test array operations
    uint64_t arr_size = array_size(cell_array);
    TEST_ASSERT(arr_size == 0, "Array size should match cell count");
    
    library_free(lib);
    TEST_PASS();
}

int main() {
    TEST_SUITE_BEGIN();
    
    printf("Testing Library Access with Getter Functions\n");
    printf("============================================\n");
    
    TEST_RUN(test_library_access);
    
    TEST_SUITE_END();
}
