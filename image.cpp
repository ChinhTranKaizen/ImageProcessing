#include "image.h"
#include "pixel.h"
#include "bmp_header.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <cmath>
void Image::readImage(std::string filename) {
    // read the file as binary (to avoid intepretation as characters)
    // and in input mode (to read data from file) in contrast to ios::out
    // which is for writing a new file
    std::cout << "Loading image: " << filename <<std::endl;

    std::fstream file(filename, std::ios::in | std::ios::binary);

    //Read the information of a bmp image file header
    BMPHeader BMPheader;
    file.read(reinterpret_cast<char*>(&BMPheader), sizeof(BMPheader));

    this->bmpheader = BMPheader;

    //Check if the file is BMP
    if (BMPheader.signature != 0x4D42) {
        std::cerr << "Error: Not a BMP file" << std::endl;
        
    }
    //Read the DIB header of the BMP file
    DIBHeader DIBheader;
    file.read(reinterpret_cast<char*>(&DIBheader), sizeof(DIBheader));
    
    this->dibheader = DIBheader;

    int32_t width = DIBheader.width;
    this->imageWidth = DIBheader.width;
    int32_t height = DIBheader.height;
    this->imageHeight = DIBheader.height;
    uint16_t bitsPerPixel = DIBheader.bits_per_pixel;

    // Calculate row padding
    /* 
    Since each row is padded to be a multiple of 4 bytes in BMP format, if we have
    7 pixels in each row, the number of byte to store 24-bit Pixels is 21, but the
    row size will be 24 (multiple of 4). Therefore, there is a 3 byte padding here.
    To calculate the padding we need to find 24 in the example above by adding 32
    bits to each row to make sure the division of 32 results in the number of 4-byte
    slots in a row. Then multiply by 4 to get the number of bytes by each row.
    bitsPerPixel * width is the number of bits required to store 24-bit Pixels
    */
    int rowSize = ((bitsPerPixel * width + 31) / 32) * 4;
    int padding = rowSize - (width * 3);

    //Create a 2D vector to read in image file
    std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width));

    // move the pointer pass the header to read the pixel data:
    // Skip the headers from the beginning
    file.seekg(BMPheader.data_offset, std::ios::beg); 

    //Starts reading pixel data from the top left corner:
    for (int32_t y = height - 1; y >= 0; --y) {
        for (int32_t x = 0; x < width; ++x) {
            file.read(reinterpret_cast<char*>(&pixels[y][x]), sizeof(Pixel));
        }
    file.seekg(padding, std::ios::cur); // Skip row padding
    }
    
    this->image = pixels;

}

bool Image::saveImage(const std::string& outputPath) {
    
    BMPHeader bmpHeader = this->bmpheader;
    DIBHeader dibHeader = this->dibheader;
    
    int32_t width = dibHeader.width;
    int32_t height = dibHeader.height;
    uint16_t bitsPerPixel = dibHeader.bits_per_pixel;
    int rowSize = ((bitsPerPixel * width + 31) / 32) * 4;
    int padding = rowSize - (width * 3);
    

    std::ofstream outputFile(outputPath, std::ios::binary);

    if (!outputFile) {
        std::cerr << "Error: Cannot create the output file." << std::endl;
        return false;
    }

    outputFile.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    outputFile.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));

    for (int32_t y = height - 1; y >= 0; --y) {
        for (int32_t x = 0; x < width; ++x) {
            outputFile.write(reinterpret_cast<const char*>(&this->image[y][x]), sizeof(Pixel));
        }
        // Write row padding
        for (int i = 0; i < padding; ++i) {
            outputFile.put(0);
        }
    }
    outputFile.close();
    return true;
}

//Adjust brightness of a pixel by multiplying it with a constant (<1 to reduce brightness
//>1 to increase brightness)
void Image::adjustBrightness(float level) {
    // lighten the color follows the advices from : https://stackoverflow.com/questions/141855/programmatically-lighten-a-color
    int threshold {255}, m {0}, total {0};
    double temp {0}, gray {0};
    for (int y {this->dibheader.height-1};y>=0;--y) {
        for (int x{0};x<dibheader.width;++x){
            int new_red {image[y][x].getPixel()[0]};
            int new_green {image[y][x].getPixel()[1]};
            int new_blue {image[y][x].getPixel()[2]};
            new_red = static_cast<int>(new_red*level);
            new_green = static_cast<int>(new_green*level);
            new_blue = static_cast<int>(new_blue*level);
            image[y][x].setPixel(static_cast<uint8_t>(std::min(static_cast<int>(255),new_red)), static_cast<uint8_t>(std::min(static_cast<int>(255),new_green)), static_cast<uint8_t>(std::min(static_cast<int>(255),new_blue)));
            // if (level < 1.0) {
            //     new_red = static_cast<int>(new_red*level);
            //     new_green = static_cast<int>(new_green*level);
            //     new_blue = static_cast<int>(new_blue*level);
            //     image[y][x].setPixel(static_cast<uint8_t>(std::min(static_cast<int>(255),new_red)), static_cast<uint8_t>(std::min(static_cast<int>(255),new_green)), static_cast<uint8_t>(std::min(static_cast<int>(255),new_blue)));
            
            // } else if (level >1.0) {
            //     m = std::max(std::max(new_red, new_green), new_blue);
            //     if (m<=threshold) {
            //         image[y][x].setPixel(static_cast<uint8_t>(new_red),static_cast<uint8_t>(new_green),static_cast<uint8_t>(new_blue));
            //     }
            //     total = new_red + new_green + new_blue;
            //     if(total >= 3*threshold) {
            //         image[y][x].setPixel(static_cast<uint8_t>(threshold),static_cast<uint8_t>(threshold),static_cast<uint8_t>(threshold));
            //     }
            //     temp = (3.0*threshold-static_cast<double>(total))/(3.0*m-static_cast<double>(total));
            //     gray = static_cast<double>(threshold) - temp * static_cast<double>(m);
            //     image[y][x].setPixel(static_cast<uint8_t>(gray+temp*new_red),static_cast<uint8_t>(gray+temp*new_green),static_cast<uint8_t>(gray+temp*new_blue));
            // }
            
            
        }
    }
}

//Adjust the contrast of the image negative float number -> more contrast (brighter),
// positive float number -> less contrast (darker)
long int Image::adjustContrast(double level){
    //Calculate the sum of all the values of all the pixels in an image:
    long int sum {0};
    for (int y {this->dibheader.height-1}; y>=0; --y){
        for (int x {0}; x<this->dibheader.width; ++x) {
            for (int temp : image[y][x].getPixel()) {
                sum += temp;
            }
        }
    }
    // return sum;
    //Calculate the average brightness
    long double avgBrightness {static_cast<long double>(sum)/(this->dibheader.width*this->dibheader.height)};
    //Adjust contrast for the picture
    for (int y {this->dibheader.height-1}; y>=0; --y){
        for (int x {0}; x<this->dibheader.width; ++x) {
            int dr = image[y][x].getPixel()[0] - avgBrightness;
            int dg = image[y][x].getPixel()[1] - avgBrightness;
            int db = image[y][x].getPixel()[2] - avgBrightness;
            int new_r = image[y][x].getPixel()[0] + static_cast<int>(dr * level);
            int new_g = image[y][x].getPixel()[1] + static_cast<int>(dr * level);
            int new_b = image[y][x].getPixel()[2] + static_cast<int>(dr * level);
            new_r = (new_r > 255) ? 255:new_r;
            new_g = (new_g > 255) ? 255:new_g;
            new_b = (new_b > 255) ? 255:new_b;
            new_r = (new_r <0) ? 0:new_r;
            new_g = (new_g <0) ? 0:new_g;
            new_b = (new_b <0) ? 0:new_b;
            image[y][x].setPixel(static_cast<uint8_t>(new_r), static_cast<uint8_t>(new_g), static_cast<uint8_t>(new_b));
            
        }
    }
}

void Image::resizeImage(int newWidth, int newHeight) {
    //Resizing image using bilinear interpolation

    // Create a new 2D vector of pixels for the resized image
    std::vector<std::vector<Pixel>> resizedImage(newHeight, std::vector<Pixel>(newWidth));

    // Calculate the scaling factor for the width and height
    double xFactor = (double)this->dibheader.width / newWidth;
    double yFactor = (double)this->dibheader.height / newHeight;

    // Iterate over each pixel in the resized image
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            // Calculate the corresponding pixel coordinates in the original image
            double origX = xFactor * x;
            double origY = yFactor * y;

            // Calculate the four nearest pixel coordinates in the original image
            int left = floor(origX);
            int right = ceil(origX);
            int top = floor(origY);
            int bottom = ceil(origY);

            // Calculate the fractional distance between the original pixel coordinates and the nearest pixel coordinates
            double xFraction = origX - left;
            double yFraction = origY - top;

            // Calculate the fractional distance between the original pixel coordinates and the nearest pixel coordinates
            double xFraction = origX - left;
            double yFraction = origY - top;

            // Get the pixel data for the four nearest pixels
            std::vector<uint8_t> topLeft = this->image[top][left].getPixel();
            std::vector<uint8_t> topRight = this->image[top][right].getPixel();
            std::vector<uint8_t> bottomLeft = this->image[bottom][left].getPixel();
            std::vector<uint8_t> bottomRight = this->image[bottom][right].getPixel();

            // Calculate the pixel data for the resized pixel using bilinear interpolation
            std::vector<uint8_t> pixelData(3);
            for (int i = 0; i < 3; i++) {
                double topValue = (1 - xFraction) * topLeft[i] + xFraction * topRight[i];
                double bottomValue = (1 - xFraction) * bottomLeft[i] + xFraction * bottomRight[i];
                double finalValue = (1 - yFraction) * topValue + yFraction * bottomValue;
                pixelData[i] = (uint8_t)finalValue;
            }

            // Set the pixel data for the resized pixel
            Pixel resizedPixel(pixelData[0], pixelData[1], pixelData[2]);
            resizedImage[y][x] = resizedPixel;
        }
    }

    // Update the image width and height to the resized values
    this->dibheader.width = newWidth;
    this->dibheader.height = newHeight;

    // Update the image pixel data to the resized values
    this->image = resizedImage;

    // Update the BMP file size and pixel array size in the BMP header
    this->bmpheader.file_size = this->bmpheader.data_offset + this->dibheader.image_size;
    this->dibheader.image_size = newWidth * newHeight * (this->dibheader.bits_per_pixel / 8);
    this->bmpheader.data_offset = sizeof(BMPHeader) + sizeof(DIBHeader);
}
