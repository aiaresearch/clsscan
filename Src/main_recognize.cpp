#include "recognize.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;
using namespace std;
using namespace cv;


int main(){
    struct clsscan_config {
        std::vector<int> valid_idx;
        std::vector<int> columns;
        std::vector<int> rows;
        std::vector<std::pair<int, int>> reference_points;
    };
    typedef struct clsscan_config clsscan_config;
    ifstream f("config.json");
    json config = json::parse(f);
    clsscan_config conf;
    from_json(config,conf);
    Point leftup_=Point(config["points_to_transform"]["leftup"]["x"],config["points_to_transform"]["leftup"]["y"]);
    Point leftdown_=Point(config["points_to_transform"]["leftdown"]["x"],config["points_to_transform"]["leftdown"]["y"]);
    Point rightdown_=Point(config["points_to_transform"]["rightdown"]["x"],config["points_to_transform"]["rightdown"]["y"]);
    int rightdown_x=config["points_to_transform"]["rightdown"]["x"];
    int rightdown_y=config["points_to_transform"]["rightdown"]["y"];
    pair<vector<Point>,float> locations_height=image_proccess("/home/pi/Code/clsscan/img/scan.png");
    vector<Point> locations = locations_height.first;
    float height = locations_height.second;
    transform_img(locations, &rightdown_x,&rightdown_y,&height);

    return 0;
}
