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
        //read in bmp file:
        void readImage(std::string filename);
        bool saveImage(const std::string& outputPath);
        void adjustBrightness(float level);
        void adjustContrast(double level);
        void resizeImage(int newWidth, int newHeight);
        void blurImage(int kernelSize, double sigma) ; //gaussianBlur the image
        void toGrayscale();
        void applyColorFilter(double redScale, double greenScale, double blueScale);
        void cannyEdgeDetection(double lowerThreshold, double upperThreshold, int kernelSize, double sigma); 
    // private:
        //helper functions for canny edge detector:
        std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> sobelOperator();
        void nonMaximumSuppression(std::vector<std::vector<double>>& gradient_magnitude, std::vector<std::vector<double>>& gradient_direction);
        void thresholding(std::vector<std::vector<double>>& gradient_magnitude, double lowerThreshold, double upperThreshold);
        void hysteresis(std::vector<std::vector<double>>& gradient_magnitude, int x, int y, double lowerThreshold);
};

#endif