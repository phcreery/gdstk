#include "common.h"
#include "cell.h" 
#include "polygon.h"
#include "label.h"

int main() {
    // Just test that headers compile
    Cell* cell = cell_new("test");
    if (cell) {
        cell_free(cell);
    }
    return 0;
}
