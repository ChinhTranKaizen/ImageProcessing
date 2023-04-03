#include <iostream>
#ifndef PIXEL_SIZE
#define PIXEL_SIZE 3
#endif
#ifndef PIXEL_H
#define PIXEL_H

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