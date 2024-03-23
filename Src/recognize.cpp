#include "recognize.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <array>

using namespace std;
using namespace cv;


cv::Mat img_preprocess(const cv::Mat& image) {
    // Process the input image
    cv::Mat processedImage;
    cv::cvtColor(image, processedImage, cv::COLOR_BGR2GRAY);
    pyrDown(image, processedImage);
    pyrDown(processedImage, processedImage);

    Mat img_gray, img_binary;
    cvtColor(processedImage, img_gray, COLOR_BGR2GRAY);
    adaptiveThreshold(img_gray, img_binary, 100, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 31, 30);
    return img_binary;
}


vector<cv::Point> find_mark_points(const cv::Mat& img) {
    // Process the image
    Mat kernel = cv::getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat img_open;
    cv::dilate(img, img_open, kernel, Point(-1, -1), 3);
    cv::erode(img_open, img_open, kernel, Point(-1, -1), 3);

    // Find contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cout << "findContours" << endl;
    cv::findContours(img_open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    cout << "findContours done" << endl;

    // Find the bounding box of the contours and return their locations
    std::vector<cv::Point> markPoints;
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Rect boundingBox = cv::boundingRect(contours[i]);
        int w = boundingBox.width;
        int h = boundingBox.height;
        // Check if the bounding box satisfies the criteria
        if (w > h && w < 150 && h < 150) {
            markPoints.push_back(boundingBox.tl()); // Add top-left corner of the bounding box
        }
    }

    return markPoints;
}

vector<cv::Point2f> transform_points(const std::vector<cv::Point>& locations, int im_w, int im_h, const clsscan_config& config) {

    // Calculate the corners of the image
    vector<cv::Point2f> corners = {
            cv::Point2f(0, 0), // Top-left
            cv::Point2f(0, im_h), // Bottom-left
            cv::Point2f(im_w, im_h), // Bottom-right
            cv::Point2f(im_w, 0)  // Top-right
    };

    // Find the nearest point to the corners
    vector<cv::Point2f> src(4);

    double distance[4];
    double d;

    for (int i = 0; i < locations.size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            d = cv::norm(cv::Point2f(locations[i]) - corners[j]);
            if (i == 0 || d < distance[j]) {
                distance[j] = d;
                src[j] = locations[i];
            }
        }
    }

    // Set the destination points
    vector<cv::Point2f> dst = {
            cv::Point2f(config.reference_points[0].first, config.reference_points[0].second), // Left-up
            cv::Point2f(config.reference_points[1].first, config.reference_points[1].second), // Left-down
            cv::Point2f(config.reference_points[2].first, config.reference_points[2].second), // Right-down
    };
    dst.push_back(dst[0] + dst[2] - dst[1]); // Right-up

    // Calculate the perspective transform matrix
    Mat m = cv::getPerspectiveTransform(src, dst);

    // Transform the locations
    vector<cv::Point2f> result;
    cv::Mat points(3, locations.size(), CV_32FC1);
    for (int i = 0; i < locations.size(); ++i) {
        points.at<float>(0, i) = (float)locations[i].x;
        points.at<float>(1, i) = (float)locations[i].y;
        points.at<float>(2, i) = 1;
    }
    cv::transform(points, points, m);
    for (int i = 0; i < locations.size(); ++i)
        result.push_back(cv::Point(points.at<float>(0, i), points.at<float>(1, i)));

    return result;
}

pair<int, int>
class_rec(vector<float> column_location, vector<int> number_top, int *b, int *c, vector<Point> locations) {
    vector<vector<int>> n(12);
    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = 0; j < column_location.size(); j++) {
            int column = column_location[j];
            if (column - 10 < locations[i].x && locations[i].x < column + 15) {
                for (size_t k = 0; k < number_top.size(); k++) {
                    double top = number_top[k];
                    if (top - 10 < locations[i].y && locations[i].y < top + 10) {
                        n[j].push_back(k);
                    }
                }
            }
        }
    }

    int b_val = n[*b - 1][0];
    int c_val = n[*c - 1][0];
    pair<int, int> p(b_val, c_val);
    return p;
}



/*void cut (){
    pair<int, Point> leftup = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) { 
        int distance_squared = xx[i] * xx[i] + yy[i] * yy[i];
        if (distance_squared < leftup.first) {
            leftup = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    pair<int, Point> rightdown = make_pair(0, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) {
        int distance_squared = xx[i] * xx[i] + yy[i] * yy[i];
        if (distance_squared > rightdown.first) {
            rightdown = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    pair<int, Point> leftdown = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) {
        int distance_squared = xx[i] * xx[i] + (yy[i]-img_open.rows) * (yy[i]-img_open.rows);
        if (distance_squared < leftdown.first) {
            leftdown = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    pair<int, Point> rightup = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) {
        int distance_squared = (xx[i]-img_open.cols) * (xx[i]-img_open.cols) + yy[i] * yy[i];
        if (distance_squared < rightup.first) {
            rightup = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    Point leftup_point = leftup.second;
    Point rightdown_point = rightdown.second;
    Point leftdown_point = leftdown.second;
    //Point rightup_point = leftdown_point-leftup_point+rightdown_point;
    Point rightup_point =rightup.second;

    Mat img_cropped = img_open(Rect(leftup_point, rightdown_point));
    vector<Point> docCnt={leftup_point,rightdown_point,leftdown_point,rightup_point };
    //cout<<docCnt << endl;

    // Mark points with circles
    vector<Point> points;
    for (size_t i = 0; i < docCnt.size(); ++i) {
        Point peak = docCnt[i];
        circle(img, peak, 10, Scalar(0, 0, 255), 2);
        points.push_back(peak);
    }
    cv::pyrDown(img,img);
    std::cout << points << endl;
    cv::imshow("im_point", img);

}*/

