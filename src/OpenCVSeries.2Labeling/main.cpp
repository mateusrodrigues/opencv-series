#include <iostream>

#include "labeling.h"

using namespace std;

int main()
{
    cout << "1) Labeling" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = labeling();
        break;
    default:
        break;
    }

    return result;
}