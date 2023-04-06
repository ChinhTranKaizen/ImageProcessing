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
    int32_t height = DIBheader.height;
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
void Image::adjustContrast(double level){
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

// The bilinearInterpolation function takes an image (2D vector of Pixel objects) and the floating-point
// x and y coordinates as input, and returns the interpolated color at the given coordinates using
// bilinear interpolation.
Pixel bilinearInterpolation(std::vector<std::vector<Pixel>>& image, double x, double y) {
    // Convert the floating-point coordinates x and y to integer coordinates.
    int x1 = static_cast<int>(x);
    int y1 = static_cast<int>(y);

    // Calculate the next integer coordinates.
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    // Calculate the differences (dx and dy) between the floating-point and integer coordinates.
    double dx = x - x1;
    double dy = y - y1;

    // Retrieve the pixel colors at the four surrounding integer coordinates.
    std::vector<uint8_t> p1 = image[y1][x1].getPixel();
    std::vector<uint8_t> p2 = image[y1][x2].getPixel();
    std::vector<uint8_t> p3 = image[y2][x1].getPixel();
    std::vector<uint8_t> p4 = image[y2][x2].getPixel();

    // Create a vector to store the resulting interpolated color.
    std::vector<uint8_t> result(3);

    // Loop through the color channels (R, G, and B).
    for (int i = 0; i < 3; ++i) {
        // Perform bilinear interpolation for the current color channel.
        double val = (1 - dx) * (1 - dy) * p1[i] +
                     dx * (1 - dy) * p2[i] +
                     (1 - dx) * dy * p3[i] +
                     dx * dy * p4[i];

        // Clamp the resulting value to the range [0, 255] and store it in the result vector.
        result[i] = static_cast<uint8_t>(std::min(std::max(val, 0.0), 255.0));
    }

    // Return the interpolated color as a Pixel object.
    return Pixel(result[0], result[1], result[2]);
}

void Image::resizeImage(int newWidth, int newHeight) {
    //Resizing image using bilinear interpolation

    std::vector<std::vector<Pixel>> newImage(newHeight, std::vector<Pixel>(newWidth));
    double scaleX = static_cast<double>(dibheader.width) / newWidth;
    double scaleY = static_cast<double>(dibheader.height) / newHeight;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            double srcX = std::min(x * scaleX, static_cast<double>(dibheader.width - 2));
            double srcY = std::min(y * scaleY, static_cast<double>(dibheader.height - 2));
            newImage[y][x] = bilinearInterpolation(image, srcX, srcY);
        }
    }

    // Update the image data and the DIB header
    image = newImage;
    dibheader.width = newWidth;
    dibheader.height = newHeight;
    dibheader.image_size = newWidth * newHeight * dibheader.bits_per_pixel / 8;
    bmpheader.file_size = dibheader.image_size + bmpheader.data_offset;
}

// Helper function to generate a Gaussian kernel
std::vector<std::vector<double>> generateGaussianKernel(int kernelSize, double sigma) {
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double twoSigmaSquared = 2 * sigma * sigma;
    double sum = 0.0;

    int halfKernelSize = kernelSize / 2;

    for (int y = -halfKernelSize; y <= halfKernelSize; ++y) {
        for (int x = -halfKernelSize; x <= halfKernelSize; ++x) {
            double g = std::exp(-(x * x + y * y) / twoSigmaSquared) / (M_PI * twoSigmaSquared);
            kernel[y + halfKernelSize][x + halfKernelSize] = g;
            sum += g;
        }
    }

    // Normalize the kernel
    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            kernel[y][x] /= sum;
        }
    }

    return kernel;
}

void Image::blurImage(int kernelSize, double sigma) {
    std::vector<std::vector<double>> kernel = generateGaussianKernel(kernelSize, sigma);
    std::vector<std::vector<Pixel>> newImage(dibheader.height, std::vector<Pixel>(dibheader.width));

    int halfKernelSize = kernelSize / 2;

    for (int y = 0; y < dibheader.height; ++y) {
        for (int x = 0; x < dibheader.width; ++x) {
            double rSum = 0.0;
            double gSum = 0.0;
            double bSum = 0.0;

            for (int ky = -halfKernelSize; ky <= halfKernelSize; ++ky) {
                for (int kx = -halfKernelSize; kx <= halfKernelSize; ++kx) {
                    int ny = y + ky;
                    int nx = x + kx;

                    if (ny >= 0 && ny < dibheader.height && nx >= 0 && nx < dibheader.width) {
                        std::vector<uint8_t> pixel = image[ny][nx].getPixel();
                        double kernelVal = kernel[ky + halfKernelSize][kx + halfKernelSize];

                        rSum += pixel[0] * kernelVal;
                        gSum += pixel[1] * kernelVal;
                        bSum += pixel[2] * kernelVal;
                    }
                }
            }

            newImage[y][x] = Pixel(static_cast<uint8_t>(std::min(std::max(rSum, 0.0), 255.0)),
                                   static_cast<uint8_t>(std::min(std::max(gSum, 0.0), 255.0)),
                                   static_cast<uint8_t>(std::min(std::max(bSum, 0.0), 255.0)));
        }
    }

    image = newImage;
}

// Helper function to apply a kernel to the image (convolution operation)
std::vector<std::vector<double>> Image::applyKernel(const std::vector<std::vector<double>>& kernel) {
    int kernelSize = kernel.size();
    int kernelCenter = kernelSize / 2;
    std::vector<std::vector<double>> result(dibheader.height, std::vector<double>(dibheader.width));

    for (int y = 0; y < dibheader.height; ++y) {
        for (int x = 0; x < dibheader.width; ++x) {
            double sum = 0.0;

            for (int ky = -kernelCenter; ky <= kernelCenter; ++ky) {
                for (int kx = -kernelCenter; kx <= kernelCenter; ++kx) {
                    int imgY = std::min(std::max(y + ky, 0), dibheader.height - 1);
                    int imgX = std::min(std::max(x + kx, 0), dibheader.width - 1);
                    std::vector<uint8_t> color = image[imgY][imgX].getPixel();
                    double gray = 0.299 * color[0] + 0.587 * color[1] + 0.114 * color[2];
                    sum += gray * kernel[ky + kernelCenter][kx + kernelCenter];
                }
            }

            result[y][x] = sum;
        }
    }

    return result;
}

void Image::toGrayscale() {
    std::vector<std::vector<Pixel>> grayscale(dibheader.height, std::vector<Pixel>(dibheader.width));

    for (int y = 0; y < dibheader.height; ++y) {
        for (int x = 0; x < dibheader.width; ++x) {
            std::vector<uint8_t> color = image[y][x].getPixel();
            uint8_t grayValue = static_cast<uint8_t>(0.299 * color[0] + 0.587 * color[1] + 0.114 * color[2]);
            grayscale[y][x] = Pixel(grayValue, grayValue, grayValue);
        }
    }

    image = grayscale;
}

void Image::applyColorFilter(double redScale, double greenScale, double blueScale)
{
    // Iterate over each pixel in the image and adjust its RGB values
    for (int y = 0; y < dibheader.height; y++) {
        for (int x = 0; x < dibheader.width; x++) {
            const auto& pixel = image[y][x].getPixel();
            uint8_t r = std::min(static_cast<uint8_t>(pixel[2] * redScale), static_cast<uint8_t>(255));
            uint8_t g = std::min(static_cast<uint8_t>(pixel[1] * greenScale), static_cast<uint8_t>(255));
            uint8_t b = std::min(static_cast<uint8_t>(pixel[0] * blueScale), static_cast<uint8_t>(255));
            image[y][x].setPixel(r, g, b);
        }
    }
}

