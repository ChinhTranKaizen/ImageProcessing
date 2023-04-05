#include <vector>
#include <string>
#include "pixel.h"
#include <fstream>
#include "bmp_header.h"

#ifndef IMAGE_H
#define IMAGE_H

class Image {
    public:
        int imageWidth {1};
        int imageHeight {1};
        BMPHeader bmpheader{};
        DIBHeader dibheader{};
        std::vector<std::vector<Pixel>> image;
    // public:
        //read in bmp file:
        void readImage(std::string filename);
        bool saveImage(const std::string& outputPath);
        void adjustBrightness(float level);
        long int adjustContrast(double level);
        void resizeImage(int newWidth, int newHeight);
        // void detectEdge();
        // void blurImage(int level); //gaussianBlur the image
};

#endif