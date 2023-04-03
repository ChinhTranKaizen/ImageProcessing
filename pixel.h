#include <iostream>
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
        int red {0};
        int green {0};
        int blue {0};
    public:
        Pixel(int r, int g, int b): red {r}, green {g}, blue {b} {
            std::cout << "constructor called" << std::endl;
        };
        int *getPixel();
        void setPixel(int r = 0, int g = 0, int b = 0);
        

};
    
#endif