#include "recognize.h"
#include "config.h"

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


vector<cv::Point2f> find_mark_points(const cv::Mat& img) {
    // Process the image
    Mat kernel = cv::getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat img_open;
    cv::dilate(img, img_open, kernel, Point(-1, -1), 3);
    cv::erode(img_open, img_open, kernel, Point(-1, -1), 3);

    // Find contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cv::findContours(img_open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

    // Find the bounding box of the contours and return their locations
    std::vector<cv::Point2f> markPoints;
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

vector<cv::Point2f> transform_points(const std::vector<cv::Point2f>& locations, int im_w, int im_h, const clsscan_config& config) {

    // Calculate the corners of the image
    cv::Point2f corners[3] = {
            cv::Point2f(0, 0), // Top-left
            cv::Point2f(0, im_h), // Bottom-left
            cv::Point2f(im_w, im_h), // Bottom-right
//            cv::Point2f(im_w, 0)  // Top-right
    };

    // Find the nearest point to the corners
    cv::Point2f src[3];

    double distance[4];
    double d;

    for (int i = 0; i < locations.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            d = cv::norm(cv::Point2f(locations[i]) - corners[j]);
            if (i == 0 || d < distance[j]) {
                distance[j] = d;
                src[j] = locations[i];
            }
        }
    }

    // Set the destination points
    Point2f dst[3] = {
            cv::Point2f(config.reference_points[0].first, config.reference_points[0].second), // Left-up
            cv::Point2f(config.reference_points[1].first, config.reference_points[1].second), // Left-down
            cv::Point2f(config.reference_points[2].first, config.reference_points[2].second), // Right-down
    };
//    dst.push_back(dst[0] + dst[2] - dst[1]); // Right-up

    // Calculate the perspective transform matrix
//    Mat m = cv::getPerspectiveTransform(src, dst);
    Mat m = cv::getAffineTransform(src, dst);

    // Transform the locations
    vector<cv::Point2f> result;
//    cv::Mat points(3, locations.size(), CV_32FC1);
//    for (int i = 0; i < locations.size(); ++i) {
//        points.at<float>(0, i) = (float)locations[i].x;
//        points.at<float>(1, i) = (float)locations[i].y;
//        points.at<float>(2, i) = 1;
//    }
//    cout << points.size << endl;
//    cout << m.size << endl;
    cv::transform(locations, result, m);
//    for (int i = 0; i < locations.size(); ++i)
//        result.push_back(cv::Point(points.at<float>(0, i), points.at<float>(1, i)));

    return result;
}

int extract_class_id(const std::vector<cv::Point2f>& locations, const clsscan_config& config) {
    // Extract the class ID
    int serial_num[config.columns.size() - 1];
    int mid, left, right, col, row;
    int class_id = 0;

    // Initialize the serial number
    for (int i = 0; i < config.columns.size() - 1; ++i)
        serial_num[i] = -1;

    // Iterate over the detected points
    for (const auto& point : locations) {
        // Check if the point is within the target region
        cout << endl << "point: " << point << endl;
        if (point.x < config.rows.front()-7 || point.x > config.rows.back()+7 || point.y < config.columns.front()-7 || point.y > config.columns.back()+7)
            continue;
        // Match the point with the rows and columns
        // Rows
        left = 0;
        right = (int)config.rows.size() - 1;
        while (left < right) {
            mid = (left + right) / 2;
            if (point.x < config.rows[mid])
                right = mid;
            else
                left = mid + 1;
        }
        col = left;

        // Columns
        left = 0;
        right = (int)config.columns.size() - 1;
        while (left < right) {
            mid = (left + right) / 2;
            if (point.y < config.columns[mid])
                right = mid;
            else
                left = mid + 1;
        }
        row = left;

        // Extract the class ID
        if (col >= 0 && row >= 0) {
            serial_num[col] = row;
        }

        cout << "col: " << col << " row: " << row << endl;
    }

    // Output the serial number
    cout << "Serial number: ";
    for (int i = 0; i < config.columns.size() - 1; ++i)
        cout << serial_num[i] << " ";
    cout << endl;

    // Calculate the class ID
    for (int i = 0; i < config.valid_idx.size(); ++i) {
        if (serial_num[config.valid_idx[i]-1] == -1)
            return -1;
        class_id = class_id * 10 + serial_num[config.valid_idx[i]-1];
    }

    return class_id;
}