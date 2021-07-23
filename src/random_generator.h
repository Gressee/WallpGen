#pragma once

class RandomGenerator {
public:

    // This by far NOT PERFECT but sufficient for this project I think

    int seed;
    int originalSeed;

    RandomGenerator(int seed) {
        this->originalSeed = seed;
        this->seed = seed;
    }

    // The rand function generates a random int
    // If the function is called the seed gets increased
    // so the second function call gets a different result
    int rand() {
        
        // DONT CHANGE THIS CODE
        // Or seed reproducability will be lost

        float num = (float)seed * 4204269313.4242069313;
        int * pNum = (int *)(&num);
        *pNum = *pNum >> seed % 8;
        int result = *pNum;

        seed ++;
        return result;
    }

    void resetSeed() {
        seed = originalSeed;
    }

};