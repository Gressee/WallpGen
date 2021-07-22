#include "bmp_image.h"
#include "perlin_noise.h"
#include "color_maps.h"

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>


using namespace std;

int main() {

    //generateNoiseWallpaperSequence(42, 4096, 2160, "../images/");
    noiseAnimation(1533, 4096, 2160, 300, "../images/animation/", 0.0025);

    // generateNoiseWallpaper(1533, 1920, 1080, "../test0.bmp", 0.0);
    // generateNoiseWallpaper(1533, 1920, 1080, "../test1.bmp", 0.1);

    return 1;
}
