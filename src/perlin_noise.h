#pragma once

// This perlin noise code is (mostly) from
// https://github.com/sol-prog/Perlin_Noise

#include <vector>

class PerlinNoise {
	
	// The permutation vector
	std::vector<int> p;

private:

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

public:

	// Initialize with the reference values for the permutation vector
	PerlinNoise();

	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);

	// Sets a input in range [-1 ,1] to [0, 1]
	double setRange(double n) {
		return (n + 1.0)/2.0;
	}

	// Get a noise value, for 2D images z can have any value
	// Returns in range [-1, 1]
	double noise(double x, double y, double z);

    // Get a noise with details
    double detailedNoise(int detailLevels, double x, double y);

	// Wood noise
	// This takes a noise value as input so every other noise
	// generation functions can be jused
	// Results in realy different results if in range [-1,1] or [0,1]
	double woodNoise(int spacing, double noiseValue);
};