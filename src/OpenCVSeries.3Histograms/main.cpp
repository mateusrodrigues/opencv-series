#include <iostream>

#include "equalize.h"
#include "motion.h"

using namespace std;

int main()
{
    cout << "1) Histogram Equalization" << endl;
    cout << "2) Motion Detector" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = equalize();
        break;
    case 2:
        result = motion_detector();
        break;
    default:
        break;
    }

    return result;
}