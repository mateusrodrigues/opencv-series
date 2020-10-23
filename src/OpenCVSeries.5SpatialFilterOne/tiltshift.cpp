#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

#include "tiltshift.h"

using namespace std;
using namespace cv;

double alfa;

int height = 0;
int height_max = 100;

int alpha = 0;
int alpha_max = 100;

int pos = 0;
int pos_max = 100;

Mat image1, image2, blended;
Mat imageTop;

char TrackbarName[50];

void on_trackbar_blend(int, void*) 
{
    alfa = (double)alpha / alpha_max;
    addWeighted(image1, 1 - alfa, imageTop, alfa, 0.0, blended);
    imshow("addweighted", blended);
}

void on_trackbar_line(int, void*) 
{
    image1.copyTo(imageTop);

    int height_p = height * 255 / 100;
    int pos_p = pos * 255 / 100;

    if (height_p > 0)
    {
        int x = pos_p - height_p / 2;
        if (x < 0)
        {
            height_p += x;
            x = 0;
        }

        if (x + height_p > image2.rows)
        {
            height_p = image2.rows - x;
            x = image2.rows - height_p;
        }

        Mat tmp = image2(Rect(0, x, 256, height_p));
        tmp.copyTo(imageTop(Rect(0, x, 256, height_p)));
    }
    on_trackbar_blend(alpha, 0);
}

int tiltshift()
{
    image1 = imread("Images/blend1.jpg");
    image2 = imread("Images/blend2.jpg");

    image2.copyTo(imageTop);
    namedWindow("addweighted", 1);

    sprintf_s(TrackbarName, "Height x %d", height_max);
    createTrackbar(TrackbarName, "addweighted",
        &height,
        height_max,
        on_trackbar_line);
    on_trackbar_line(height, 0);

    sprintf_s(TrackbarName, "Alpha x %d", alpha_max);
    createTrackbar(TrackbarName, "addweighted",
        &alpha,
        alpha_max,
        on_trackbar_blend);
    on_trackbar_blend(alpha, 0);

    sprintf_s(TrackbarName, "Position x %d", pos_max);
    createTrackbar(TrackbarName, "addweighted",
        &pos,
        pos_max,
        on_trackbar_line);

    waitKey(0);

    imwrite("Images/output.jpg", imageTop);

    return 0;
}