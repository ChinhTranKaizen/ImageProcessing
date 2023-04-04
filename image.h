#include <vector>
#include <string>
#include "pixel.h"
#include <fstream>

#ifndef IMAGE_H
#define IMAGE_H

class Image {
    private: 
        int imageWidth {1};
        int imageHeight {1};
        std::vector<std::vector<Pixel>> image;
    public:
        //read in bmp file:
        void readImage(std::string filename);
        // void saveImage(Image i);
        // void adjustBrightness(int level);
        // void adjustContrast(int level);
        // void resizeImage(int percentage);
        // void detectEdge();
        // void blurImage(int level); //gaussianBlur the image
};

#endif