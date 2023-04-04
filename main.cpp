#include <iostream>
#include "pixel.h"

int main() {
    Pixel rOnly {255, 0, 0};

    for (size_t i {0}; i < PIXEL_SIZE; i++) {
        std::cout << *(rOnly.getPixel()+i) << std::endl;
    }
    

    rOnly.setPixel(254);

    for (size_t i {0}; i < PIXEL_SIZE; i++) {
        std::cout << *(rOnly.getPixel()+i) << std::endl;
    }

    return 0;
}