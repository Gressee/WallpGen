#include "color_maps.h"
#include "bmp_image.h"

#include <cmath>

void colorMapSmoothEnds(
    BMPImage img,
    int layer,
    bool constAlpha,
    double sharpness_r,
    double sharpness_g,
    double sharpness_b,
    double sharpness_a,
    double amp_r,
    double amp_g,
    double amp_b,
    double amp_a)
    {
    
    // This color maps an entire layer
    // img has to be black and white 
    // The color is the same for 0 and 1 
    
    // constAlpha controlls if the alpha is variable the same way 
    // as colors or if its always 1

    // Set only the colors
    double r, g, b;
    for (int y = 0; y < img.height; y++) {
        for (int x = x; x < img.width; x++) {

        }
    }

    // Set the alpha (is seperated so if is only executed once)
    float a;
    if (constAlpha == true) {
        a = 1.0;
    } else {

    }

    /*
    float r = (float)(amp_r * pow(2.0, - sharpness_r * pow(noise-0.5, 2)));
    float g = (float)(amp_g * pow(2.0, - sharpness_g * pow(noise-0.5, 2)));
    float b = (float)(amp_b * pow(2.0, - sharpness_b * pow(noise-0.5, 2)));
    */
    
}