//bmp header struct
#include <cstdint>
#ifndef BMPHEADER_H
#define BMPHEADER_H
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;         // signature "BM"
    uint32_t file_size;         // total size of the BMP file in bytes
    uint32_t reserved;          // typically set to zero
    uint32_t data_offset;       // offset to the beginning of the pixel data in bytes
};

#pragma pack(pop)

struct DIBHeader {
    uint32_t header_size;       // size of the header in bytes
    int32_t width;              // width of the image in pixels
    int32_t height;             // height of the image in pixels
    uint16_t num_planes;        // number of color planes (typically set to 1)
    uint16_t bits_per_pixel;    // number of bits per pixel
    uint32_t compression;       // compression method (0 for uncompressed)
    uint32_t image_size;        // size of the compressed image data in bytes
    int32_t x_pixels_per_meter; // horizontal resolution in pixels per meter
    int32_t y_pixels_per_meter; // vertical resolution in pixels per meter
    uint32_t num_colors;        // number of colors in the color palette (0 for true-color images)
    uint32_t important_colors;  // number of important colors in the color palette (0 for all colors)
};
#endif
