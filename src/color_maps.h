#pragma once

#include "bmp_image.h"

using namespace std;

// This takes an noise input in range [0,1]
// Returns the same color for 0 and 1 
void colorMapSmoothEnds(BMPImage img, int layer,bool constAlpha , double sharpness_r, double sharpness_g, double sharpness_b, double sharpness_a, double amp_r, double amp_g, double amp_b, double amp_a);