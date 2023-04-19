#include <iostream>
#include "pixel.h"
#include "image.h"
#include <vector>
#include <string>
#include <cassert>

void test_black_image() {
    Image img;
    img.image = std::vector<std::vector<Pixel>>(5, std::vector<Pixel>(5, Pixel(0, 0, 0)));

    auto gradient_magnitude = img.sobelOperator();

    for (const auto &row : gradient_magnitude) {
        for (double value : row) {
            assert(value == 0);
        }
    }
}

void test_white_image() {
    Image img;
    img.image = std::vector<std::vector<Pixel>>(5, std::vector<Pixel>(5, Pixel(255, 255, 255)));

    auto gradient_magnitude = img.sobelOperator();

    for (const auto &row : gradient_magnitude) {
        for (double value : row) {
            assert(value == 0);
        }
    }
}

void test_vertical_line() {
    Image img;
    img.image = std::vector<std::vector<Pixel>>(5, std::vector<Pixel>(5, Pixel(0, 0, 0)));

    for (size_t i = 0; i < img.image.size(); ++i) {
        img.image[i][2] = Pixel(255, 255, 255);
    }

    auto gradient_magnitude = img.sobelOperator();

    for (size_t i = 0; i < gradient_magnitude.size(); ++i) {
        for (size_t j = 0; j < gradient_magnitude[i].size(); ++j) {
            if (j == 2) {
                assert(gradient_magnitude[i][j] >= 255 * sqrt(2));
            } else {
                assert(gradient_magnitude[i][j] == 0);
            }
        }
    }
}

void test_horizontal_line() {
    // Implement this test case similarly to the vertical line test.
}

void test_diagonal_line() {
    // Implement this test case similarly to the vertical line test.
}

int main() {

    Image snail;

    snail.readImage("snail.bmp");
    
    Image brighterSnail {snail};
    brighterSnail.adjustBrightness(1.1);
    brighterSnail.saveImage(".\\brighterSnail.bmp");

    Image brightestSnail {snail};
    brightestSnail.adjustBrightness(1.3);
    brightestSnail.saveImage(".\\brightestSnail.bmp");

    Image darkerSnail {snail};
    darkerSnail.adjustBrightness(0.9);
    darkerSnail.saveImage(".\\darkerSnail.bmp");

    Image darkestSnail {snail};
    darkestSnail.adjustBrightness(0.6);
    darkestSnail.saveImage(".\\darkestSnail.bmp");
    
    Image moreContrastSnail {snail};
    moreContrastSnail.adjustContrast(-0.1);
    moreContrastSnail.saveImage(".\\moreContrastSnail.bmp");

    Image mostContrastSnail {snail};
    mostContrastSnail.adjustContrast(-0.3);
    mostContrastSnail.saveImage(".\\mostContrastSnail.bmp");

    Image lessContrastSnail {snail};
    lessContrastSnail.adjustContrast(0.1);
    lessContrastSnail.saveImage(".\\lessContrastSnail.bmp");

    Image leastContrastSnail {snail};
    leastContrastSnail.adjustContrast(0.3);
    leastContrastSnail.saveImage(".\\leastContrastSnail.bmp");

    Image biggerSnail {snail};
    biggerSnail.resizeImage(512,512);
    biggerSnail.saveImage(".\\biggerSnail.bmp");

    Image biggestSnail {snail};
    biggestSnail.resizeImage(1024,1024);
    biggestSnail.saveImage(".\\biggestSnail.bmp");

    Image smallerSnail {snail};
    smallerSnail.resizeImage(128,128);
    smallerSnail.saveImage(".\\smallerSnail.bmp");

    Image smallestSnail {snail};
    smallestSnail.resizeImage(64,64);
    smallestSnail.saveImage(".\\smallestSnail.bmp");

    Image blurredSnail {snail};
    blurredSnail.blurImage(5, 1.0);
    blurredSnail.saveImage(".\\blurredSnail.bmp");

    Image moreblurredSnail {snail};
    moreblurredSnail.blurImage(5, 2.0);
    moreblurredSnail.saveImage(".\\moreblurredSnail.bmp");

    Image evenmoreblurredSnail {snail};
    evenmoreblurredSnail.blurImage(7, 3.0);
    evenmoreblurredSnail.saveImage(".\\evenmoreblurredSnail.bmp");

    Image mostblurredSnail {snail};
    mostblurredSnail.blurImage(9, 4.0);
    mostblurredSnail.saveImage(".\\mostblurredSnail.bmp");

    Image greySnail {snail};
    greySnail.toGrayscale();
    greySnail.saveImage(".\\greySnail.bmp");

    Image artsySnail1 {snail};
    artsySnail1.applyColorFilter(1.0, 0.7, 0.7); //(1.0, 1.3, 1.3) 
    artsySnail1.saveImage(".\\artsySnail1.bmp");

    Image artsySnail2 {snail};
    artsySnail2.applyColorFilter(1.0, 1.0, 1.3);
    artsySnail2.saveImage(".\\artsySnail2.bmp");
    
    Image artsySnail5 {snail};
    artsySnail5.applyColorFilter(1.3, 0.7, 0.0);
    artsySnail5.saveImage(".\\artsySnail5.bmp");



    return 0;
}

