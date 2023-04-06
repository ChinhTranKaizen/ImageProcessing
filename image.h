#include <vector>
#include <string>
#include "pixel.h"
#include <fstream>
#include "bmp_header.h"

#ifndef IMAGE_H
#define IMAGE_H

class Image {
    // private: 
    public:
        BMPHeader bmpheader{};
        DIBHeader dibheader{};
        std::vector<std::vector<Pixel>> image;
    // public:
        //read in bmp file:
        void readImage(std::string filename);
        bool saveImage(const std::string& outputPath);
        void adjustBrightness(float level);
        void adjustContrast(double level);
        void resizeImage(int newWidth, int newHeight);
        void blurImage(int kernelSize, double sigma) ; //gaussianBlur the image
        void toGrayscale();
        void applyColorFilter(double redScale, double greenScale, double blueScale);
    private:
        std::vector<std::vector<double>> applyKernel(const std::vector<std::vector<double>>& kernel);
};

#endif