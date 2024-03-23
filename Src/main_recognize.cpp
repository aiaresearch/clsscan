#include "recognize.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;
using namespace std;
using namespace cv;


int main() {
    // Load the configuration file
    ifstream f("config.json");
    clsscan_config conf = json::parse(f);

    // Load the image
    Mat img_open = imread("img/test.png");
    img_open = img_preprocess(img_open);

    // Find the mark points
    vector<Point> mark_points = find_mark_points(img_open);

    // Transform the points
    vector<Point2f> transformed_points = transform_points(mark_points, img_open.cols, img_open.rows, conf);

    // Output the transformed points
    for (const auto& point : transformed_points) {
        cout << point << endl;
    }
    return 0;
}
