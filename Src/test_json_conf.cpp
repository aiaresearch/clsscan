#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;

typedef struct {
    std::vector<int> valid_idx;
    std::vector<int> columns;
    std::vector<int> rows;
    std::vector<std::pair<int, int>> reference_points;
} clsscan_config;
/*
void to_json(json& j, const clsscan_config& config) {
    j = json{
        {"b", config.valid_idx[0]},
        {"c", config.valid_idx[1]},
        {"number_top", config.columns},
        {"column_location", config.rows},
        {"points_to_transform", json{
            {"leftup", json{{"x", config.reference_points[0].first}, {"y", config.reference_points[0].second}}},
            {"leftdown", json{{"x", config.reference_points[1].first}, {"y", config.reference_points[1].second}}},
            {"rightdown", json{{"x", config.reference_points[2].first}, {"y", config.reference_points[2].second}}}
        }}
    };
}
*/
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

int main() {
    std::ifstream f("config.json");
    clsscan_config conf = json::parse(f);
    std::cout << conf.valid_idx[0] << ' ' << conf.valid_idx[1] << std::endl;
    std::cout << conf.reference_points[0].first << conf.reference_points[0].second << std::endl;
    return 0;
}

