#include <iostream>

#include "pixels.h"
#include "swap.h"

using namespace std;

int main()
{
    cout << "1) Pixels" << endl;
    cout << "2) Swap Regions" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = pixels();
        break;
    case 2:
        result = swap();
        break;
    default:
        break;
    }

    return result;
}