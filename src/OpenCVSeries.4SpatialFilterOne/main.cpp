#include <iostream>

#include "spatial_filter.h"

using namespace std;

int main()
{
    cout << "1) Spatial Filter" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = spatial_filter();
        break;
    default:
        break;
    }

    return result;
}