#include "image.h"
#include "pixel.h"
#include "bmp_header.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

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
        