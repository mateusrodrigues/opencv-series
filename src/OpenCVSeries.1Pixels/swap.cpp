#include <opencv2/opencv.hpp>

#include "swap.h"

using namespace std;
using namespace cv;

int swap()
{
    Mat image;

    image = imread("Images/image2.png", IMREAD_COLOR);

    if (!image.data)
    {
        cout << "[ERROR] Could not open file image.png" << endl;
        return -1;
    }

    int xHalf = image.cols / 2;
    int yHalf = image.rows / 2;

    Mat A(image, Range(0, yHalf), Range(0, xHalf));
    Mat B(image, Range(0, yHalf), Range(xHalf, image.cols));
    Mat C(image, Range(yHalf, image.rows), Range(0, xHalf));
    Mat D(image, Range(yHalf, image.rows), Range(xHalf, image.cols));

    Mat result1;
    vconcat(D, B, result1);

    Mat result2;
    vconcat(C, A, result2);

    Mat result;
    hconcat(result1, result2, result);

    namedWindow("Swap Regions", WINDOW_AUTOSIZE);
    imshow("Swap Regions", result);
    waitKey();

    return 0;
}
