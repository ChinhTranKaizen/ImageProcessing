#include "pixel.h"
#include <vector>
std::vector<uint8_t> Pixel::getPixel (){
    std::vector<uint8_t> rgbvector {red,green,blue};
    return rgbvector;
}

void Pixel::setPixel(uint8_t r, uint8_t g, uint8_t b) {
    red = r;
    green = g;
    blue = b;
}