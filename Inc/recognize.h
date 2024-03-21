#define CLSSCAN_RECOGNIZE_H
#ifndef CLSSCAN_RECOGNIZE_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using nlohmann::json;
struct clsscan_config {
    std::vector<int> valid_idx;
    std::vector<int> columns;
    std::vector<int> rows;
    std::vector<std::pair<int, int>> reference_points;
};
typedef struct clsscan_config clsscan_config;

void from_json(const json& j, clsscan_config& config);
pair<vector<Point>,float> image_proccess(const char * imgpath);
void transform_img(vector<Point> locations,int *x,int *y,float *height);

#endif