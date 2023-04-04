#include <iostream>
#include <vector>
#ifndef PIXEL_SIZE
#define PIXEL_SIZE 3
#endif
#ifndef PIXEL_H
#define PIXEL_H
/*
Ideas for expansion of this class:
1. A method for converting the color values from RGB (Red, Green, Blue) to another color space, such as HSL (Hue, Saturation, Lightness) or CMYK (Cyan, Magenta, Yellow, Key).
2. A method for calculating the grayscale value of the pixel based on its RGB values.
3. A method for calculating the brightness or luminance value of the pixel based on its RGB values.
4. A method for performing operations on the color values, such as adding or subtracting a certain amount from each value, multiplying or dividing the values by a scalar, or blending the color with another color.
5. A method for comparing two pixels to determine if they are the same or different.
6. A method for printing the color values of the pixel to the console or to a file in a specific format.
*/
class Pixel{
    private:
        uint8_t red {0};
        uint8_t green {0};
        uint8_t blue {0};
    public:
        Pixel(uint8_t r, uint8_t g, uint8_t b): red {r}, green {g}, blue {b} {};
        Pixel() : red(0), green(0), blue(0) {};
        std::vector<uint8_t> getPixel ();
        void setPixel(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
        

};
    
#endif