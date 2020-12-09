#include <iostream>

#include "tiltshift.h"

using namespace std;

int main()
{
    cout << "1) Tilt Shift" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = tiltshift();
        break;
    default:
        break;
    }

    return result;
}