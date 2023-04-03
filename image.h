#include <vector>
#include <string>
#include "pixel.h"
#include <fstream>

#ifndef IMAGE_H
#define IMAGE_H

class Image {
    public: 
        int imageWidth {1};
        int imageHeight {1};
        std::vector<std::vector<Pixel>> image;
    private:
        //read in bmp file:
        void readBMP(std::string filename);

};

#endif