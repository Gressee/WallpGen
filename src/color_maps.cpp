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
    double r, g, b, a, n;
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            // This uses the red value but it doesnt really matter
            // bc the image should be black and white
            n = img.getPixel(layer, x, y).r;
         
            r = amp_r * pow(2.0, - sharpness_r * pow(n-0.5, 2));
            g = amp_g * pow(2.0, - sharpness_g * pow(n-0.5, 2));
            b = amp_b * pow(2.0, - sharpness_b * pow(n-0.5, 2));

            // If statement could maybe only called once but idk now
            if (constAlpha == true) {
                a = 1.0;
            } else {
                a = amp_a * pow(2.0, - sharpness_a * pow(n-0.5, 2));
            }

            // Set the new pixel color
            img.setPixel(layer, x, y, {r, g, b, a});
        }
    }    
}