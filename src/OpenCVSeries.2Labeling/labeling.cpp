#include <iostream>
#include <opencv2/opencv.hpp>

#include "labeling.h"

using namespace std;
using namespace cv;

int labeling()
{
    Mat bubbles;
    Mat equalized;
    int width;
    int height;
    int nobjects = 0;

    Point p;
    bubbles = imread("Images/bubbles.png", IMREAD_GRAYSCALE);

    if (!bubbles.data)
    {
        cout << "[ERROR] Could not open file bubbles.png" << endl;
        return -1;
    }

    width = bubbles.cols;
    height = bubbles.rows;

    cout << width << "x" << height << endl;

    p.x = 0;
    p.y = 0;

    // search for objects in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (bubbles.at<uchar>(i, j) == 255)
            {
                // if point is white, it belongs to an object.
                // fill it and increment object count.
                nobjects++;

                p.x = j;
                p.y = i;

                floodFill(bubbles, p, nobjects);
            }
        }
    }

    cout << "This image contains " << nobjects << " bubbles" << endl;
    equalizeHist(bubbles, equalized);
    imshow("bubbles", bubbles);
    imshow("equalized", equalized);
    imwrite("Images/labeling.png", bubbles);
    waitKey();

    return 0;
}