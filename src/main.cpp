#include "bmp_image.h"
#include "perlin_noise.h"
#include "color_maps.h"

#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;

int main() {

    // Just testing

    const int width = 1920;
    const int height = 1080;

    // Time meassurement
    auto start = chrono::high_resolution_clock::now();

    BMPImage img(1, width, height);

    
    PerlinNoise pn(100);
    float n;
    Pixel p;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            //n = pn.detailedNoise(5, (double)x/(double)width, (double)y/(double)height);
            //n = pn.detailedNoise(5, (double)x/(double)width + 2*n, (double)y/(double)height + 2*n);
            n = pn.detailedNoise(4, (double)x/(double)width, (double)y/(double)height);
            n = pn.detailedNoise(4, (double)x/(double)width + n, (double)y/(double)height + n);
            n = pn.setRange(n);
            n = pn.woodNoise(10, n);
        

            img.setPixel(0, x, y, {n,n,n,1.0f});
        }
    }

    //img.blurlayer(0, 10);

    img.exportToFile("../images/test.bmp");


    auto stop = chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(stop-start).count() << "ms" << endl; 

    return 1;
}
