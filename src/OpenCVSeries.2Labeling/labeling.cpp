#include <iostream>
#include <opencv2/opencv.hpp>

#include "labeling.h"

using namespace std;
using namespace cv;

int labeling_original()
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
    waitKey();

    return 0;
}

int labeling_with_holes(cv::String filename)
{
    Mat bubbles;

    int width;
    int height;

    int allbubbles = 0;
    int solidbubbles = 0;
    int hollowbubbles = 0;

    set<uchar> shades_used;

    bubbles = imread(filename, IMREAD_GRAYSCALE);

    if (!bubbles.data)
    {
        cout << "[ERROR] Could not open file bubbles.png" << endl;
        return -1;
    }

    width = bubbles.cols;
    height = bubbles.rows;

    // Display starting image
    imshow("Starting image", bubbles);

#pragma region 1_remove_edge_bubbles
    // We iterate over the edge pixels of the image in order to
    // remove bubbles that touch the edge.
    for (int i = 0; i < height; i++)
    {
        int j = 0;
        if (bubbles.at<uchar>(Point(j, i)) == 255) floodFill(bubbles, Point(j, i), 0);
    }
    for (int i = 0; i < height; i++)
    {
        int j = width - 1;
        if (bubbles.at<uchar>(Point(j, i)) == 255) floodFill(bubbles, Point(j, i), 0);
    }
    for (int j = 0; j < width; j++)
    {
        int i = 0;
        if (bubbles.at<uchar>(Point(j, i)) == 255) floodFill(bubbles, Point(j, i), 0);
    }
    for (int j = 0; j < width; j++)
    {
        int i = height - 1;
        if (bubbles.at<uchar>(Point(j, i)) == 255) floodFill(bubbles, Point(j, i), 0);
    }

    imshow("Step 1: Remove edge bubbles", bubbles);
#pragma endregion

#pragma region 2_count_all_bubbles
    // Once edge bubbles are removed, we look for white (0) pixels, which
    // represent bubbles, and, for each one found, we paint it by shades of gray in the range 1..n,
    // which, in the end, will label each bubble with its own specific shade of gray.
    // The total number of bubbles will be stored in the variable 'allbubbles'.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (bubbles.at<uchar>(i, j) == 255)
            {
                allbubbles++;
                floodFill(bubbles, Point(j, i), allbubbles);
            }
        }
    }

    imshow("Step 2: Count all bubbles", bubbles);
#pragma endregion

#pragma region 3_paint_background_white
    // In order to find bubbles that are hollow, we need to paint the background white (255), which,
    // in turn, will only leave the internal parts of hollow bubbles with black (0) shades.
    floodFill(bubbles, Point(0, 0), 255);
    imshow("Step 3: Paint background white", bubbles);
#pragma endregion

#pragma region 4_search_hollow_bubbles
    // Since black (0) shades are only found in internal parts of hollow bubbles,
    // the number of occurrences of these shades represents the number of total hollow bubbles.
    // We store that number in 'hollowbubbles' and paint the internal part of the bubble white (255).
    // To consider a bubble has more than one hole, we need to store which bubble that hole belongs to by
    // storing that bubble's shade (label) in a set that only contains shades (bubbles) whose holes have
    // already been counted.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (bubbles.at<uchar>(i, j) == 0)
            {
                // Found a hole, which bubbles does that belong to?
                auto shade = bubbles.at<uchar>(i, j - 1);
                auto pos = shades_used.find(shade);
                if (pos == shades_used.end())
                {
                    // Shade not found, add it to set and increment hollow bubbles count.
                    hollowbubbles++;
                    shades_used.insert(shade);
                }

                floodFill(bubbles, Point(j, i), 255);
            }
        }
    }

    imshow("Step 4: Paint internals white and count hollow", bubbles);
#pragma endregion

    solidbubbles = allbubbles - hollowbubbles;

    cout << endl;
    cout << "The image contains" << endl;
    cout << allbubbles << " total bubbles, with:" << endl;
    cout << "\t" << solidbubbles << " solid bubbles" << endl;
    cout << "\t" << hollowbubbles << " hollow bubbles" << endl;

    waitKey();

    return 0;
}

int labeling_colorful()
{
    Mat bubbles;

    int width;
    int height;

    int allbubbles = 0;

    Point p;
    bubbles = imread("Images/bubbles-colorful.png", IMREAD_COLOR);

    if (!bubbles.data)
    {
        cout << "[ERROR] Could not open file bubbles.png" << endl;
        return -1;
    }

    width = bubbles.cols;
    height = bubbles.rows;

    // Display starting image
    imshow("Starting image", bubbles);

#pragma region count_all_bubbles
    // Knowing that there are more than 255 bubbles in the image, simply labeling
    // them with shades of gray won't be enough since there are more bubbles than
    // shades of gray available. In that case, we will label them with color shades
    // starting from (1, 0, 0) up to (255, 255, 254).

    uchar red = 1;
    uchar green = 0;
    uchar blue = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto color = bubbles.at<Vec3b>(Point(j, i));

            if (color[0] == 255 && color[1] == 255 && color[2] == 255)
            {
                floodFill(bubbles, Point(j, i), Vec3b(blue, green, red));
                allbubbles++;

                // Increment color
                red = (red + 1 > 255) ? 255 : red + 1;
                green = (red == 255) ? green + 1 : green;
                blue = (green == 255) ? blue + 1 : blue;
                blue = (blue == 255) ? 255 : blue; // We have reached the color limits
            }
        }
    }

    imshow("Step 1: Label bubbles with colors", bubbles);
#pragma endregion

    cout << endl;
    cout << "The image contains " << allbubbles << " bubbles";

    waitKey();

    return 0;
}