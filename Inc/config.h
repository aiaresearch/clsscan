//
// Created by Tony Mai on 2024/3/24.
//

#ifndef CLSSCAN_CONFIG_H
#define CLSSCAN_CONFIG_H

#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
struct clsscan_config {
    std::vector<int> valid_idx;
    std::vector<float> columns;
    std::vector<int> rows;
    std::vector<std::pair<int, int>> reference_points;
};
typedef struct clsscan_config conf;

void from_json(const json &j, clsscan_config &config);

#endif //CLSSCAN_CONFIG_H
