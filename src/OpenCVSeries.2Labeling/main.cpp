#include <iostream>

#include "labeling.h"

using namespace std;

int main()
{
    cout << "1) Labeling" << endl;
    cout << "2) Labeling (bubbles with only 1 hole)" << endl;
    cout << "3) Labeling (bubbles with more than 1 hole)" << endl;
    cout << "4) Labeling colorful" << endl;

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
        result = labeling_with_holes("Images/bubbles.png");
        break;
    case 3:
        result = labeling_with_holes("Images/bubbles-2-holes.png");
        break;
    case 4:
        result = labeling_colorful();
        break;
    default:
        break;
    }

    return result;
}