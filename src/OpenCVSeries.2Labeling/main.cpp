#include <iostream>

#include "labeling.h"

using namespace std;

int main()
{
    cout << "1) Labeling" << endl;
    cout << "2) Labeling enhanced" << endl;
    cout << "3) Labeling colorful" << endl;

    cout << "Choose the example to run: ";

    int example;
    cin >> example;

    int result = 0;

    switch (example)
    {
    case 1:
        result = labeling_original();
        break;
    case 2:
        result = labeling_enhanced();
        break;
    case 3:
        result = labeling_colorful();
        break;
    default:
        break;
    }

    return result;
}