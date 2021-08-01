#include "bmp_image.h"
#include "perlin_noise.h"
#include "color_maps.h"
#include "random_generator.h"

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>


using namespace std;

int main() {

    //genNoiseWallpSequence(1600, 4096, 2160, "../images/");
    noiseAnimation(2221, 4096, 2160, 0, 3000, "../images/animation/", 0.0010);
    

    return 1;
}
