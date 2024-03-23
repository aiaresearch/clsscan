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
    ifstream f("config.json");
    json config = json::parse(f);
    clsscan_config conf;
    conf=config;
    Point leftup_=Point(config["points_to_transform"]["leftup"]["x"],config["points_to_transform"]["leftup"]["y"]);
    Point leftdown_=Point(config["points_to_transform"]["leftdown"]["x"],config["points_to_transform"]["leftdown"]["y"]);
    Point rightdown_=Point(config["points_to_transform"]["rightdown"]["x"],config["points_to_transform"]["rightdown"]["y"]);
    int rightdown_x=config["points_to_transform"]["rightdown"]["x"];
    int rightdown_y=config["points_to_transform"]["rightdown"]["y"];
    pair<pair<vector<Point>,int>,vector<vector<float>> > pair1=image_proccess("/home/pi/Code/clsscan/img/scan.png");
    vector<Point> locations = pair1.first.first;
    int height =pair1.first.second;
    vector<vector<float>> contours_point = pair1.second;
    locations=transform_img(locations, rightdown_x,rightdown_y,&height,contours_point);
    cout << 29 << endl;
    vector<float> columns=conf.columns;
    cout << 31 << ' ' << columns[0] << endl;
    vector<int> rows=conf.rows;
    pair<int,int> result =class_rec(columns,rows,&conf.valid_idx[0],&conf.valid_idx[1],locations);
    return 0;
}
