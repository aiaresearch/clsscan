#ifndef CLSSCAN_RECOGNIZE_H
#define CLSSCAN_RECOGNIZE_H


#include <opencv2/opencv.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using nlohmann::json;
using namespace cv;
using namespace std;
struct clsscan_config {
    std::vector<int> valid_idx;
    std::vector<float> columns;
    std::vector<int> rows;
    std::vector<std::pair<int, int>> reference_points;
};
typedef struct clsscan_config conf;

void from_json(const json &j, clsscan_config &config);

cv::Mat img_preprocess(const cv::Mat& image);

vector<cv::Point2f> find_mark_points(const cv::Mat& img);

vector<cv::Point2f> transform_points(const std::vector<cv::Point2f>& locations, int im_w, int im_h, const clsscan_config& config);

pair<int, int>
class_rec(vector<float> column_location, vector<int> number_top, int *b, int *c, vector<Point> locations);

#endif