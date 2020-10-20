#include <iostream>

#include "equalize.h"

using namespace std;

int main()
{
    cout << "1) Histogram" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = equalize();
        break;
    default:
        break;
    }

    return result;
}