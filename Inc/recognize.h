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
    std::vector<int> columns;
    std::vector<int> rows;
    std::vector<std::pair<int, int>> reference_points;
};
typedef struct clsscan_config conf;

void from_json(const json& j, clsscan_config& config);

pair<vector<Point>,int> image_proccess(const char * imgpath);
vector<Point> transform_img(vector<Point> locations,int *x,int *y,int *height);
pair<int,int> class_rec(float column_location[20] ,float number_top[10] ,int *b,int *c,int *locations);
#endif