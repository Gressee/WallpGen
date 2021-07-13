/*
Code from
https://www.youtube.com/watch?v=vqT5j38bWGg&t=1142s
*/

#include "image.h"
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
    pixels = vector<Pixel>(layers*width*height, {0,0,0,0});
    
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
void BMPImage::setPixel(int layer, int x, int y, float r, float g, float b, float a) {
    
    // Check bounds
    if (layer >= layer || x >= width || y >= height) return;

    // Set Colors
    pixels[layer*height*width + y*width + x].r = r;
    pixels[layer*height*width + y*width + x].g = g;
    pixels[layer*height*width + y*width + x].b = b;
    pixels[layer*height*width + y*width + x].a = a;
}

// Compose the different layers
Pixel BMPImage::getAlphaCompositionPixel(int x, int y) const {
    // This method takes a pos of the image and composes all layers
    // With differnt alpha alues to a sigle pixel
    // The Alpha Channel of the output pixel is NOT used !!!
    // For the math see  https://en.wikipedia.org/wiki/Alpha_compositing
    // This works from the ground up. Firsz layer 0 and 1 are composed
    // The result of that is composed with layer 2 etc

    // If the final pixel alpha is later interpreted as 1 even 
    // if it is not after alpha comosition

    // a_0 is alpha of the layer below 
    // a_1 is alpha of the layer above
    // a_new is the alpha of the composed layers
    // p_0 is pixel that the two layers get composed to and which gets returned at the end
    // p_1 is the pixel that has the data for the above layer


    int l = 0;
    float a_0, a_1, a_new;
    Pixel p_0, p_1; 
    
    // Initial values because for now there is now 
    // peviously composed layer
    a_0 = getPixel(l, x, y).a;
    p_0 = getPixel(l, x, y);

    for (l = 0; l < layers-1; l ++) {
        // l is in this loop the index of the bottom layer
        a_1 = getPixel(l+1, x, y).a;
        a_new = a_1 + a_0*(1-a_1);

        // Get color data from to layer above
        p_1 = getPixel(l+1, x, y);

        // Red
        p_0.r = (p_1.r * a_1 + p_0.r * a_0 * (1-a_1))/a_new;
        // Green
        p_0.g = (p_1.g * a_1 + p_0.g * a_0 * (1-a_1))/a_new;
        // Blue
        p_0.b = (p_1.b * a_1 + p_0.b * a_0 * (1-a_1))/a_new;

        // Update the new alpha for the now composed layer
        p_0.a = a_new;
        a_0 = a_new;
    }

    return p_0;
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
void BMPImage::drawRect(Pixel color, int layer, int posX, int posY, int width, int height) {
    
    for (int y = posY; y < posY+height; y++) {
        for (int x = posX; x < posX+width; x++) {
            setPixel(layer, x ,y, color.r, color.g, color.b, color.a);
        }
    }
}

// Circle around x, y with radius rad
void BMPImage::drawCircle(Pixel color, int layer, int centerX, int centerY, int rad) {
    // This goes through evey pixel in the surrunding 
    // square and checks if it is in the circle
    
    int y_sq;
    int rad_sq = rad*rad;
    
    for (int y= centerY-rad; y < centerY+rad; y++) {
    
        y_sq = (y-centerY)*(y-centerY);  // Performance !!!
        
        for (int x = centerX-rad; x < centerX+rad; x++) {
            
            if ((x-centerX)*(x-centerX) + y_sq <= rad_sq) {
                setPixel(layer, x, y, color.r, color.g, color.b, color.a);
            }
        }
    }
}
