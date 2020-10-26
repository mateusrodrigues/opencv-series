#include <iostream>
#include <opencv2/opencv.hpp>

#include "motion.h"

using namespace std;
using namespace cv;

int motion_detector()
{
    VideoCapture cap;

    vector<Mat> planes;
    Mat histR1, histR2;
    int nbins = 64;

    float range[] = { 0, 256 };
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
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));

    Mat image;
    int key;

    while (1)
    {
        cap >> image;
        split(image, planes);
        calcHist(&planes[2], 1, 0, Mat(), histR1, 1, &nbins, &histrange, uniform, acummulate);
        normalize(histR1, histR1, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

        cap >> image;
        split(image, planes);
        calcHist(&planes[2], 1, 0, Mat(), histR2, 1, &nbins, &histrange, uniform, acummulate);
        normalize(histR2, histR2, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

        histImgR.setTo(Scalar(0));

        for (int i = 0; i < nbins; i++)
        {
            line(histImgR,
                Point(i, histh),
                Point(i, histh - cvRound(histR2.at<float>(i))),
                Scalar(0, 0, 255), 1, 8, 0);
        }

        histImgR.copyTo(image(Rect(0, 0, nbins, histh)));

        double comparison = compareHist(histR1, histR2, HISTCMP_CHISQR_ALT);
        cout << comparison << endl;

        if (comparison > 60)
        {
            circle(image, Point(width / 2, height / 2), 50, Scalar(0, 0, 255), 5, 8, 0);
        }

        imshow("Motion Detector", image);
        key = waitKey(30);
        if (key == 27) break;
    }

    return 0;
}