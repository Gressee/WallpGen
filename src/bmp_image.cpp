/*
Code from
https://www.youtube.com/watch?v=vqT5j38bWGg&t=1142s
*/

#include "bmp_image.h"
#include <bits/stdc++.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

/* BASIC IMAGE FUNCTIONS */

// BMPImage Constructor
BMPImage::BMPImage( int imgLayers, int imgWidth, int imgHeight) {
    layers = imgLayers;
    width = imgWidth;
    height = imgHeight;

    // Init pixel vector with all values zero
    pixels = vector<Pixel>(layers*width*height, {0.0f, 0.0f, 0.0f, 0.0f});
    
}

// BMPImage Destructor
BMPImage::~BMPImage() {
    // skip
}

// Get color of the image at x, y
Pixel BMPImage::getPixel(int layer, int x, int y) const {
    // pixels is an 1D vector -> get x y pos with this formula
    return pixels[layer*width*height + y*width + x];
}

// Set a color of the image
void BMPImage::setPixel(int layer, int x, int y, Pixel color) {
    
    // Check bounds
    if (layer >= layers || layer < 0 || x >= width || x < 0 || y >= height || y < 0) return;

    // Set Color
    pixels[layer*height*width + y*width + x] = color;
}

// Compose the different layers
Pixel BMPImage::getAlphaCompositionPixel(int x, int y) const {
    // This method takes a pos of the image and composes all layers
    // With differnt alpha alues to a sigle pixel
    // The Alpha Channel of the output pixel is NOT used !!!
    // For the math see  https://en.wikipedia.org/wiki/Alpha_compositing
    // This works from the ground up. First layer 0 and 1 are composed
    // The result of that is composed with layer 2 etc

    // This uses premultiplied alpha

    // l is always the index of the bottom layer
    int l;

    // p_0 is the pixel from the bottom layer
    // all layers get composed to that layer
    // p_1 is the pixel from the top layer
    Pixel p_0, p_1;

    // Get initial values for the bottom pixel
    p_0 = getPixel(0, x, y);

    for (l = 0; l < layers-1; l++) {

        // Get the pixel for the new top layer
        p_1 = getPixel(l+1, x, y);

        // New premutiplied
        p_0.a = p_1.a + p_0.a * (1 - p_1.a);
        p_0.r = p_1.r + p_0.r * (1 - p_1.a);
        p_0.g = p_1.g + p_0.g * (1 - p_1.a);
        p_0.b = p_1.b + p_0.b * (1 - p_1.a);
    }

    // Return the composed pixel
    return p_0;
}

// Bluer a single layer with the Gausian blur algorithm
void BMPImage::blurlayer(int layer, int blurRadius) {
    
    // This uses the Gausian Blur algorithm 
    // that means that each value for the nwe Pixel is 
    // the average of all the sourrounding pixels (in the sqaure
    // with the lenght "blurRadius"

    // First the layer to bleur gets copied to a new vector 
    // to save the original pixel data
    // Then clac the the new pixel values and write them to the original 
    // vector

    // Init a new buffer vector
    vector<Pixel> buffer;
    for (int i = layer * width * height; i < (layer+1) * width * height; i++) {
        buffer.push_back(pixels[i]);
    }

    Pixel blurPixel, p;
    float total_r, total_g, total_b, total_a = 0;
    float pixelNum = 0;
    
    int x_sur_min, x_sur_max, y_sur_min, y_sur_max;

    // This loops through every pixel to blur
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            total_r = 0;
            total_g = 0;
            total_b = 0;
            total_a = 0;
            pixelNum = 0;
            
            x_sur_min = clamp(x - blurRadius/2, 0, width-1);
            x_sur_max = clamp(x + blurRadius/2, 0, width-1);
            
            y_sur_min = clamp(y - blurRadius/2, 0, height-1);
            y_sur_max = clamp(y + blurRadius/2, 0, height-1);

            // This loops through every pixel around the blur pixel
            for (int y_surrounding = y_sur_min; y_surrounding <= y_sur_max; y_surrounding++) {
                for (int x_surrounding = x_sur_min; x_surrounding <= x_sur_max; x_surrounding++) {
                    // Get the pixel date from the buffer vector
                    //cout << "Index: " << (y_surrounding) * width + (x_surrounding) << endl;
                    int index = (y_surrounding) * width + (x_surrounding);
                    p = buffer[(y_surrounding) * width + (x_surrounding)];
                    total_r += p.r;
                    total_g += p.g;
                    total_b += p.b;
                    total_a += p.a;
                    pixelNum ++;
                }
            }

            // Clac the averages and write to the original
            blurPixel = {
                total_r/pixelNum,
                total_g/pixelNum,
                total_b/pixelNum,
                total_a/pixelNum
            };
            setPixel(layer, x, y, blurPixel);
        }
    }

}

// Export the image as a bitmap file
void BMPImage::exportToFile(const char * path) const {

    
    // Open output file
    ofstream f;
    f.open(path, ios::out | ios::binary);

    // Check if succesfull
    if (!f.is_open()) {
        cout << "File open Error\n";
        return;
    }

    // Padding for the bmp format
    // Look YT video for reason
    unsigned char bmpPad[] = {0, 0, 0, 0};
    const int paddingAmount = ((4 - (width * 3) % 4) % 4);

    const int fileHeaderSize = 14;  
    const int infoHeaderSize = 40;

    const int fileColorSize = width*height*3 + paddingAmount*height; // in bytes
    const int fileSize = fileHeaderSize + infoHeaderSize + fileColorSize;

    
    // Define the File Header
    unsigned char fileHeader[fileHeaderSize];
    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // Not used
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Pixel date offset
    fileHeader[10] = fileHeaderSize + infoHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    // Define the information header
    unsigned char infoHeader[infoHeaderSize];
    // Header size
    infoHeader[0] = infoHeaderSize;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;
    // Image width
    infoHeader[4] = width;
    infoHeader[5] = width >> 8;
    infoHeader[6] = width >> 16;
    infoHeader[7] = width >> 24;
    // Image height
    infoHeader[8] = height;
    infoHeader[9] = height >> 8;
    infoHeader[10] = height >> 16;
    infoHeader[11] = height >> 24;
    // Planes 
    infoHeader[12] = 1;
    infoHeader[13] = 0;
    // Bits per pixel
    infoHeader[14] = 24;
    infoHeader[15] = 0;
    // Compression
    infoHeader[16] = 0;
    infoHeader[17] = 0;
    infoHeader[18] = 0;
    infoHeader[19] = 0;
    // Image size (no compression)
    infoHeader[20] = 0;
    infoHeader[21] = 0;
    infoHeader[22] = 0;
    infoHeader[23] = 0;
    // X Pixels per meter
    infoHeader[24] = 0;
    infoHeader[25] = 0;
    infoHeader[26] = 0;
    infoHeader[27] = 0;
    // Y pixels per meter
    infoHeader[28] = 0;
    infoHeader[29] = 0;
    infoHeader[30] = 0;
    infoHeader[31] = 0;
    // Total colors
    infoHeader[32] = 0;
    infoHeader[33] = 0;
    infoHeader[34] = 0;
    infoHeader[35] = 0;
    // Important colors
    infoHeader[36] = 0;
    infoHeader[37] = 0;
    infoHeader[38] = 0;
    infoHeader[39] = 0;
    
    // Write headers
    f.write(reinterpret_cast<char *>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char *>(infoHeader), infoHeaderSize);


    // write color data
    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            
            // Get the alpha composed pixel data
            Pixel p = getAlphaCompositionPixel(x, y);

            // Convert to bmp range
            unsigned char r = static_cast<unsigned char>(p.r * 255.0f);
            unsigned char g = static_cast<unsigned char>(p.g * 255.0f);
            unsigned char b = static_cast<unsigned char>(p.b * 255.0f);
            
            unsigned char color[] = {b, g, r};  // This has to be g r b  per bmp definition

            f.write(reinterpret_cast<char*>(&color), 3);
        }

        // Write the padding at the end of a pixel line
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    f.close();

    cout << "File created\n";

}


/* DRAW FUNCTIONS */

// Rectangle with bot left corner at x, y
void BMPImage::drawRect(Pixel color, int layer, int pos_x, int pos_y, int rect_width, int rect_height) {
    
    int x_min = clamp(pos_x, 0, width-1);
    int x_max = clamp(pos_x+rect_width, 0, width-1);

    int y_min = clamp(pos_y, 0, height-1);
    int y_max = clamp(pos_y+rect_height, 0, height-1);


    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            setPixel(layer, x ,y, color);
        }
    }
}

// Circle around x, y with radius rad
void BMPImage::drawCircle(Pixel color, int layer, int center_x, int center_y, int rad) {
    // This goes through evey pixel in the surrunding 
    // square and checks if it is in the circle
    
    int x_min = clamp(center_x - rad, 0, width-1);
    int x_max = clamp(center_x + rad, 0, width-1);

    int y_min = clamp(center_y - rad, 0, height-1);
    int y_max = clamp(center_y + rad, 0, height-1);

    int y_sq;
    int rad_sq = rad*rad;
    
    for (int y = y_min; y < y_max; y++) {
    
        y_sq = (y-center_y)*(y-center_y);  // Performance !!!
        
        for (int x = x_min; x < x_max; x++) {
            
            if ((x-center_x)*(x-center_x) + y_sq <= rad_sq) {
                setPixel(layer, x, y, color);
            }
        }
    }
}
