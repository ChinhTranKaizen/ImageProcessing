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
        void adjustContrast(double level);
        void resizeImage(int newWidth, int newHeight);
        void edgeDetection(double lowThreshold, double highThreshold);
        void blurImage(int kernelSize, double sigma) ; //gaussianBlur the image
        void toGrayscale();
    private:
        std::vector<std::vector<double>> applyKernel(const std::vector<std::vector<double>>& kernel);
        std::vector<std::vector<double>> blurGrayscale(const std::vector<std::vector<double>>& input, int kernelSize, double sigma);
};

#endif