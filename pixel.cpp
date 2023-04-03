#include "pixel.h"

int* Pixel::getPixel (){
    static int wholePixel[3] {red,green,blue};
    return wholePixel;
}

void Pixel::setPixel( int r, int g, int b) {
    red = r;
    green = g;
    blue = b;
}