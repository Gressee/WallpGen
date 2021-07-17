#pragma once

// This perlin noise code is (mostly) from
// https://github.com/sol-prog/Perlin_Noise

#include "bmp_image.h"

#include <vector>

class PerlinNoise {
	
	// The permutation vector
	std::vector<int> p;

public:

	// Initialize with the reference values for the permutation vector
	PerlinNoise();

	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);

	// Sets a input in range [-1 ,1] to [0, 1]
	void correctLayerRange(BMPImage * img, int layer);

    // Create a new layer with detailed Noise (in BLACK & WHITE with ALPHA = 1)
	// detailLevel = 1 is the normal without detail
    void detailedNoiseLayer(BMPImage * img, int layer, int detailLevels, double baseFactor, double offset);

	// This first created detailed noise whjich is used 
	// to create more detailed noise
	// offset is used in animation as the time 
	void warpedNoiseLayer(BMPImage * img, int layer, int detailLevels, double baseFactor, double offset);

	// Wood noise
	// Tranforms a layer of an image to a wood like structure
	// Alpaha gets transformed like a color
	void woodNoiseLayer(BMPImage * img, int layer, int spacing);


	private:

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	// If non detailed nois is needed I recomend to use 
	// the detailedNoiseLayer function with a detailLevel = 1

	// Get a noise value, for 2D images z can have any value
	// Returns in range [-1, 1]
	double noise(double x, double y, double z);
};