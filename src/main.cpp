#include "bmp_image.h"
#include "perlin_noise.h"
#include "color_maps.h"

#include <iostream>
#include <string>
#include <cmath>
#include <chrono>


using namespace std;

void warpedNoiseAnimation(int width, int height, int frames, string folder, double speed) {

    // Speed is the offset change per frame
    // recomended: 0.001 - 0.006

    // folder has to have a "/" at the end

    cout << "Warped Noise Animation Started\n";

    auto start = chrono::high_resolution_clock::now();

    BMPImage img(1, width, height);
    PerlinNoise pn(100);

    double offset = 0.0;

    string filepath;

    for (int i = 0; i < frames; i++) {

        auto frameStart = chrono::high_resolution_clock::now();
        
        pn.warpedNoiseLayer(&img, 0, 2, 10.0, offset);
        pn.correctLayerRange(&img, 0);
        colorMapLinear(&img, 0, 0.85, 0.23, 0.53, 0.04, 0.0, 0.0, 1.0, 1.0);

        filepath = folder;
        filepath.append("img");
        filepath.append(to_string(i));
        filepath.append(".bmp");

        img.exportToFile(filepath);
        
        offset += speed;

        auto frameStop = chrono::high_resolution_clock::now();
        cout << "Frame Time: " << chrono::duration_cast<chrono::milliseconds>(frameStop-frameStart).count() << "ms" << endl; 
    }

    auto stop = chrono::high_resolution_clock::now();
    cout << endl << "Animation Time: " << chrono::duration_cast<chrono::seconds>(stop-start).count() << "s" << endl; 
} 

void generateNoiseWallpaper(int seed, int width, int height, string folder) {
    
    PerlinNoise pn(10);

    string filepath;

    int i = 0;
    while (true) {

        BMPImage img(1, width, height);

        filepath = folder;
        filepath.append("img");
        filepath.append(to_string(i));
        filepath.append(".bmp");

        img.exportToFile(filepath);

        i++;

        delete &img;
        
    }

}

int main() {

    const int width = 1920;
    const int height = 1080;

    BMPImage img(1, width, height);
    PerlinNoise pn(10);

    pn.warpedNoiseLayer(&img, 0, 2, 4.0, 0.0);
    pn.correctLayerRange(&img, 0);

    img.exportToFile("../images/test.bmp");

    return 1;
}
