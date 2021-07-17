#pragma once

#include "bmp_image.h"

using namespace std;

// Returns the same color for 0 and 1 
void colorMapSmoothEnds(BMPImage * img, int layer,bool constAlpha , double sharpness_r, double sharpness_g, double sharpness_b, double sharpness_a, double amp_r, double amp_g, double amp_b, double amp_a);

// Every color and alpha value is linear between 0 and 1
// low values are the value of the color when noise is 0 and heigh is 
// when noise is 1 between is linear
void colorMapLinear(BMPImage * img, int layer, double low_r, double heigh_r, double low_g, double high_g, double low_b, double heigh_b, double low_a, double high_a);