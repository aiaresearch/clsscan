#include "recognize.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void from_json(const json& j, clsscan_config& config) {
    config.valid_idx.push_back(0);
    config.valid_idx.push_back(0);
    j.at("b").get_to(config.valid_idx[0]);
    j.at("c").get_to(config.valid_idx[1]);

    const json& number_top = j.at("number_top");
    config.columns.clear();
    for (auto it = number_top.begin(); it != number_top.end(); ++it) {
        config.columns.push_back(it.value());
    }

    const json& column_location = j.at("column_location");
    config.rows.clear();
    for (auto it = column_location.begin(); it != column_location.end(); ++it) {
        config.rows.push_back(it.value());
    }

    const json& points_to_transform = j.at("points_to_transform");
    config.reference_points.clear();
    config.reference_points.emplace_back(points_to_transform.at("leftup").at("x"), points_to_transform.at("leftup").at("y"));
    config.reference_points.emplace_back(points_to_transform.at("leftdown").at("x"), points_to_transform.at("leftdown").at("y"));
    config.reference_points.emplace_back(points_to_transform.at("rightdown").at("x"), points_to_transform.at("rightdown").at("y"));
}
