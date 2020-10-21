#include <iostream>
#include <opencv2/opencv.hpp>

#include "spatial_filter.h"

using namespace std;
using namespace cv;

void printmask(Mat& m) 
{
    for (int i = 0; i < m.size().height; i++) 
    {
        for (int j = 0; j < m.size().width; j++) 
        {
            cout << m.at<float>(i, j) << ",";
        }
        cout << "\n";
    }
}

int spatial_filter() 
{
    VideoCapture cap;  // open the default camera

    float media[] = { 0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111, 0.1111 };

    float gauss[] = { 0.0625, 0.125,  0.0625, 0.125, 0.25,
                     0.125,  0.0625, 0.125,  0.0625 };

    float horizontal[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

    float vertical[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

    float laplacian[] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };

    float boost[] = { 0, -1, 0, -1, 5.2, -1, 0, -1, 0 };

    Mat frame, framegray, frame32f, frameFiltered;
    Mat mask(3, 3, CV_32F);
    Mat result;
    bool laplacianOnTop = false;
    double width, height;
    int absolut;
    char key;

    cap.open(0);

    if (!cap.isOpened())  // check if we succeeded
        return -1;

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);

    cout << "width =" << width << "\n";
    cout << "height =" << height << "\n";
    cout << "fps    =" << cap.get(CAP_PROP_FPS) << "\n";
    cout << "format =" << cap.get(CAP_PROP_FORMAT) << "\n";

    namedWindow("spatialfilter", WINDOW_NORMAL);
    namedWindow("original", WINDOW_NORMAL);

    mask = Mat(3, 3, CV_32F, media);

    absolut = 1;  // calcs abs of the image

    while (true)
    {
        cap >> frame;  // get a new frame from camera
        cvtColor(frame, framegray, COLOR_BGR2GRAY);
        flip(framegray, framegray, 1);
        imshow("original", framegray);
        framegray.convertTo(frame32f, CV_32F);
        filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);

        if (laplacianOnTop)
        {
            Mat lap = Mat(3, 3, CV_32F, laplacian);
            filter2D(frameFiltered, frameFiltered, frameFiltered.depth(), lap, Point(1, 1), 0);
        }

        if (absolut) 
        {
            frameFiltered = abs(frameFiltered);
        }

        frameFiltered.convertTo(result, CV_8U);

        imshow("spatialfilter", result);

        key = (char)waitKey(10);
        if (key == 27) break;  // esc pressed!

        switch (key) 
        {
        case 'a':
            absolut = !absolut;
            break;
        case 'm':
            mask = Mat(3, 3, CV_32F, media);
            printmask(mask);
            laplacianOnTop = false;
            break;
        case 'g':
            mask = Mat(3, 3, CV_32F, gauss);
            printmask(mask);
            laplacianOnTop = false;
            break;
        case 'h':
            mask = Mat(3, 3, CV_32F, horizontal);
            printmask(mask);
            laplacianOnTop = false;
            break;
        case 'v':
            mask = Mat(3, 3, CV_32F, vertical);
            printmask(mask);
            laplacianOnTop = false;
            break;
        case 'l':
            mask = Mat(3, 3, CV_32F, laplacian);
            printmask(mask);
            laplacianOnTop = false;
            break;
        case 'b':
            mask = Mat(3, 3, CV_32F, boost);
            laplacianOnTop = false;
            break;
        case 'x':
            mask = Mat(3, 3, CV_32F, gauss);
            printmask(mask);
            laplacianOnTop = true;
            break;
        default:
            break;
        }
    }
    return 0;
}