// This perlin noise code is from
// https://github.com/sol-prog/Perlin_Noise


#include "perlin_noise.h"
#include "bmp_image.h"

#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>


// Initialize with the reference values for the permutation vector
PerlinNoise::PerlinNoise() {

    // Initialize the permutation vector with the reference values
    p = {
        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
        8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
        35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
        134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
        55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
        18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
        250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
        189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 
        43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
        97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
        107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
    
    // Duplicate the permutation vector
    p.insert(p.end(), p.begin(), p.end());
}

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed) {
    p.resize(256);

    // Fill p with values from 0 to 255
    std::iota(p.begin(), p.end(), 0);

    // Initialize a random engine with seed
    std::default_random_engine engine(seed);

    // Suffle  using the above random engine
    std::shuffle(p.begin(), p.end(), engine);

    // Duplicate the permutation vector
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::fade(double t) { 
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) { 
    return a + t * (b - a); 
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // Convert lower 4 bits of hash into 12 gradient directions
    double u = h < 8 ? x : y,
    v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinNoise::noise(double x, double y, double z) {
    
    // Returns in range [-1, 1]

    // Find the unit cube that contains the point
    int X = (int) floor(x) & 255;
    int Y = (int) floor(y) & 255;
    int Z = (int) floor(z) & 255;

    // Find relative x, y,z of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute fade curves for each of x, y, z
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    // Hash coordinates of the 8 cube corners
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    // Add blended results from 8 corners of cube
    double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
    return res;
}

// Sets a input in range [-1 ,1] to [0, 1]
void PerlinNoise::correctLayerRange(BMPImage * img, int layer) {
    Pixel p;
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            p = img->getPixel(layer, x, y);
            p.r = (p.r + 1.0)/2.0;
            p.g = (p.g + 1.0)/2.0;
            p.b = (p.b + 1.0)/2.0;
            p.a = (p.a + 1.0)/2.0;

            img->setPixel(layer, x, y, p);
        }
    }

}


// Create a new layer with detailed Noise (in BLACK & WHITE with ALPHA = 1)
// detailLevel = 1 is the normal without detail
void PerlinNoise::detailedNoiseLayer(BMPImage * img, int layer, int detailLevels, double baseFactor, double offset) {

    // Basefactor is good in a range from 1-4

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {

            // The z value from the "noise" function id by default 0.0 
            // but it is random so it can change

            // Get the first normal noise layer
            double n = noise(baseFactor * (double)x / (double)img->width + offset, baseFactor * (double)y / (double)img->height + offset, 0.0);
            
            double factor = 2.0;
            double amp = 0.85;
            double total_amp = 1;
            
            for (int l = 0; l < detailLevels-1; l++) {
                total_amp += amp;
                n += amp * noise(baseFactor * factor * (double)x / (double)img->width, baseFactor * factor * (double)y / (double)img->height, 0.8);

                factor *= 2.0;
                amp *= 0.85;
            }

            // Bring back to range [-1,1]
            n = n / total_amp;

            // Set the pixel of the image
            img->setPixel(layer, x, y, {n, n, n, 1.0});

        }
    }
}
// This first created detailed noise whjich is used 
// to create more detailed noise
// offset is used in animation as the time 
void PerlinNoise::warpedNoiseLayer(BMPImage * img, int layer, int detailLevels, double baseFactor, double offset) {

    // Recomended: Low detailLevels (1-3); High baseFactor (5-10)

    // Create a image that stores the noise that is used
    // to warp the other noise 
    BMPImage buffer_img(1, img->width, img->height);
    detailedNoiseLayer(&buffer_img, 0, detailLevels, baseFactor, offset);

    // This is the same code as in the detailed noise but with the
    // warp amount included
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {

            // The z value from the "noise" function id by default 0.0 
            // but it is random so it can change

            // Get the first normal noise layer
            double warp = buffer_img.getPixel(0, x, y).r;  // what color doesnt matter bc r=g=b=a
            double noise_x = baseFactor * (double)x / (double)img->width + warp + offset;
            double noise_y = baseFactor * (double)y / (double)img->height + warp + offset;
            double n = noise(noise_x, noise_y, 0.0);
            
            double factor = 2.0;
            double amp = 0.85;
            double total_amp = 1;
            
            for (int l = 0; l < detailLevels-1; l++) {
                total_amp += amp;

                noise_x = baseFactor * factor * ((double)x / (double)img->width + warp + offset);
                noise_y = baseFactor * factor * ((double)y / (double)img->height + warp + offset);
                n += amp * noise(noise_x, noise_y, 0.8);

                factor *= 2.0;
                amp *= 0.85;
            }

            // Bring back to range [-1,1]
            n = n / total_amp;

            // Set the pixel of the image
            img->setPixel(layer, x, y, {n, n, n, 1.0});

        }
    }
}

// Wood noise
// Tranforms a layer of an image to a wood like structure
// Alpaha gets transformed like a color
void PerlinNoise::woodNoiseLayer(BMPImage * img, int layer, int spacing) {
    
    Pixel p;
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            p = img->getPixel(layer, x, y);

            p.r *= spacing;
            p.r = p.r - floor(p.r);

            p.g *= spacing;
            p.g = p.g - floor(p.g);

            p.b *= spacing;
            p.b = p.b - floor(p.b);

            p.a *= spacing;
            p.a = p.a - floor(p.a);

            img->setPixel(layer, x, y, p);
        }
    }
}
