#include <iostream>
#include "pixel.h"
#include "image.h"
#include <vector>
#include <string>

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
    evenmoreblurredSnail.blurImage(5, 3.0);
    evenmoreblurredSnail.saveImage(".\\evenmoreblurredSnail.bmp");

    Image mostblurredSnail {snail};
    mostblurredSnail.blurImage(5, 4.0);
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
    
    // Image artsySnail3 {snail};
    // artsySnail3.applyColorFilter(0.7, 1.3, 1.0); //(1.0, 1.3, 1.0)
    // artsySnail3.saveImage(".\\artsySnail3.bmp");

    // Image artsySnail4 {snail};
    // artsySnail4.applyColorFilter(1.3, 1.3, 1.0);
    // artsySnail4.saveImage(".\\artsySnail4.bmp");

    Image artsySnail5 {snail};
    artsySnail5.applyColorFilter(1.3, 0.7, 0.0);
    artsySnail5.saveImage(".\\artsySnail5.bmp");

    // Image artsySnail6 {snail};
    // artsySnail6.applyColorFilter(0.7, 1.3, 1.3);
    // artsySnail6.saveImage(".\\artsySnail6.bmp");

    return 0;
}

