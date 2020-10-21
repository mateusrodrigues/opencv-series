#include <iostream>
#include <opencv2/opencv.hpp>

#include "equalize.h"

using namespace std;
using namespace cv;

int equalize()
{
    VideoCapture cap;

    Mat hist, histeq;
    int nbins = 64;

    float range[] = { 0, 255 };
    const float* histrange = { range };

    bool uniform = true;
    bool acummulate = false;

    cap.open(0);

    if (!cap.isOpened())
    {
        cout << "[ERROR] Camera Unavailable...";
        return -1;
    }

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);

    cout << "width = " << width << endl;
    cout << "height  = " << height << endl;

    int histw = nbins, histh = nbins / 2;
    Mat histImg(histh, histw, CV_8UC1, Scalar(0, 0, 0));
    Mat histeqImg(histh, histw, CV_8UC1, Scalar(0, 0, 0));

    Mat image, equalized;
    int key;

    while (1) 
    {
        cap >> image;
        cvtColor(image, image, COLOR_BGR2GRAY);
        equalizeHist(image, equalized);

        calcHist(&image, 1, 0, Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);
        calcHist(&equalized, 1, 0, Mat(), histeq, 1, &nbins, &histrange, uniform, accumulate);

        normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
        normalize(histeq, histeq, 0, histeqImg.rows, NORM_MINMAX, -1, Mat());

        histImg.setTo(Scalar(0));
        histeqImg.setTo(Scalar(0));

        for (int i = 0; i < nbins; i++) 
        {
            line(histImg,
                Point(i, histh),
                Point(i, histh - cvRound(hist.at<float>(i))),
                Scalar(255, 255, 255), 1, 8, 0);
            line(histeqImg,
                Point(i, histh),
                Point(i, histh - cvRound(histeq.at<float>(i))),
                Scalar(255, 255, 255), 1, 8, 0);
        }

        histImg.copyTo(image(Rect(0, 0, nbins, histh)));
        histeqImg.copyTo(equalized(Rect(0, histh, nbins, histh)));

        imshow("Not Equalized", image);
        imshow("Equalized", equalized);

        key = waitKey(30);
        if (key == 27) break;
    }
    return 0;
}