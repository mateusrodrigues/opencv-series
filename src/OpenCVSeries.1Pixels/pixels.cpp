#include <opencv2/opencv.hpp>

#include "pixels.h"

using namespace std;
using namespace cv;

int pixels()
{
    Mat image;

    image = imread("Images/image.jpg", IMREAD_GRAYSCALE);

    if (!image.data)
    {
        cout << "[ERROR] Could not open file image.png" << endl;
        return -1;
    }

    Point2i start;
    Point2i end;

    cout << "Enter the starting point (x, y): ";
    cin >> start.x >> start.y;
    cout << "Enter the ending point (x, y): ";
    cin >> end.x >> end.y;

    // Verify whether negative section is within the image bounds.
    if (start.x < 0 || start.x > image.cols || end.x < 0 || end.x > image.cols ||
        start.y < 0 || start.y > image.rows || end.y < 0 || end.y > image.rows)
    {
        cout << "[ERROR] Point chosen is out of bounds." << endl;
        return -1;
    }

    for (int i = start.y; i < end.y; i++)
    {
        for (int j = start.x; j < end.x; j++)
        {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }

    namedWindow("Negative", WINDOW_AUTOSIZE);
    imshow("Negative", image);
    waitKey();

    return 0;
}
