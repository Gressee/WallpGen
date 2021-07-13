# include "image.h"

#include <iostream>
#include <chrono>

using namespace std;

int main() {

    const int width = 200;
    const int height = 200;

    // Time meassurement
    auto start = chrono::high_resolution_clock::now();

    BMPImage img(2, width, height);

    
    img.drawRect(
        {1.0, 0, 0 , 1.0},
        0, 100, 100, 10, 10
    );
    

    img.exportToFile("../images/test.bmp");


    auto stop = chrono::high_resolution_clock::now();
    cout << "Time: ";
    cout << chrono::duration_cast<chrono::microseconds>(stop-start).count() << endl;


    return 1;
}
