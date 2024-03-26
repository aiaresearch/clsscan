#include "recognize.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;


void from_json(const json &j, clsscan_config &config) {
    config.valid_idx.push_back(0);
    config.valid_idx.push_back(0);
    j.at("b").get_to(config.valid_idx[0]);
    j.at("c").get_to(config.valid_idx[1]);

    const json &number_top = j.at("number_top");
    config.columns.clear();
    for (auto it = number_top.begin(); it != number_top.end(); ++it) {
        config.columns.push_back(it.value());
    }

    const json &column_location = j.at("column_location");
    config.rows.clear();
    for (auto it = column_location.begin(); it != column_location.end(); ++it) {
        config.rows.push_back(it.value());
    }

    const json &points_to_transform = j.at("points_to_transform");
    config.reference_points.clear();
    config.reference_points.emplace_back(points_to_transform.at("leftup").at("x"),
                                         points_to_transform.at("leftup").at("y"));
    config.reference_points.emplace_back(points_to_transform.at("leftdown").at("x"),
                                         points_to_transform.at("leftdown").at("y"));
    config.reference_points.emplace_back(points_to_transform.at("rightdown").at("x"),
                                         points_to_transform.at("rightdown").at("y"));
}


void to_json(json &j, const clsscan_config &config) {
    j = json{{"b", config.valid_idx[0]},
             {"c", config.valid_idx[1]}};
    json number_top;
    for (auto it = config.columns.begin(); it != config.columns.end(); ++it) {
        number_top.push_back(*it);
    }
    j["number_top"] = number_top;

    json column_location;
    for (auto it = config.rows.begin(); it != config.rows.end(); ++it) {
        column_location.push_back(*it);
    }
    j["column_location"] = column_location;

    json points_to_transform;
    points_to_transform["leftup"]["x"] = config.reference_points[0].first;
    points_to_transform["leftup"]["y"] = config.reference_points[0].second;
    points_to_transform["leftdown"]["x"] = config.reference_points[1].first;
    points_to_transform["leftdown"]["y"] = config.reference_points[1].second;
    points_to_transform["rightdown"]["x"] = config.reference_points[2].first;
    points_to_transform["rightdown"]["y"] = config.reference_points[2].second;
    j["points_to_transform"] = points_to_transform;
}



