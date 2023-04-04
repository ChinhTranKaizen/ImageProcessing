#include "pixel.h"

int* Pixel::getPixel (){
    static int wholePixel[3] {red,green,blue};
    return wholePixel;
}

void Pixel::setPixel(uint8_t r, uint8_t g, uint8_t b) {
    red = r;
    green = g;
    blue = b;
}