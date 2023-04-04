#include <iostream>
#include "pixel.h"
#include "image.h"
#include <vector>
#include <string>

int main() {

    Image snail;

    snail.readImage("snail.bmp");
    int y {92};
    int x {42};
    std::cout << "RGB values at y = " << y << ", and x = " <<x <<" is"<<std::endl ; 
    for (size_t i {0}; i < PIXEL_SIZE; i++) {
        
        
        std::cout << *(snail.image[y][x].getPixel()+i) <<std::endl;
    }

    std::cout << "Image height in pixels is: "<< snail.imageHeight << std::endl;
    std::cout << "Image weidth in pixels is: "<< snail.imageHeight << std::endl;

    std::string outputPath = ".\\newsnail.bmp";

    if (snail.saveImage(outputPath)) {
        std::cout << "Image saved successfully to " << outputPath << std::endl;
    } else {
        std::cerr << "Error: Failed to save the image." << std::endl;
        return 1;
    }
    ;

    return 0;
}