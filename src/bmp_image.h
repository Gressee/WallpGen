/*
Code from
https://www.youtube.com/watch?v=vqT5j38bWGg&t=1142s
*/

#pragma once

#include <vector>
#include <cstdint>


using namespace std;


// strcut that stores th rgb values of a pixel
// The range of rgb is 0.0 - 1.0
struct Pixel {
    double r;
    double g; 
    double b;
    double a;
};

class BMPImage {

    // IMPORTANT:
    // The origin of the coords system is at 
    // the BOTTOM LEFT

public:


    // Store dimensions of img
    int layers;
    int width;
    int height;

    // This vec stores every Pixel in every layer
    // The first width*height belong to layer 0
    // The next width*height belong to layer 1 etc
    vector<Pixel> pixels;

    // Constructor
    BMPImage(int imgLayers, int imgWidth, int imgHeight);
    
    // Destructor
    ~BMPImage();
    
    /*  BASIC FUNCTIONS */

    // Get data of one pixel
    Pixel getPixel(int layer, int x, int y) const;

    // Set the on pixel
    void setPixel(int layer, int x, int y, Pixel color);
    
    // Compose the different layers
    Pixel getAlphaCompositionPixel(int x, int y) const;

    // Bluer a single layer with the Gausian blur algorithm
    void blurlayer(int layer, int blurRadius);

    // Export the image to a .bmp file
    void exportToFile(const char * path) const;


    /* DRAW FUNCTIONS */

    // Basic single color shapes
    // The Pixel arg is just the color that the shape should be in
    void drawRect(Pixel color, int layer, int pos_x, int pos_y, int rect_width, int rect_height);
    void drawCircle(Pixel color, int layer, int center_x, int center_y, int rad);
    void drawTriangle(Pixel color, int layer, int x1, int y1, int x2, int y2, int x3, int y3);

};
