#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include "iostream"

using namespace cv;
using namespace std;

int main() {













    // Calculate width and height using Pythagoras' theorem, then perform perspective transformation
    int h = sqrt(pow(points[1].x - points[0].x, 2) + pow(points[1].y - points[0].y, 2));
    int w = sqrt(pow(points[2].x - points[1].x, 2) + pow(points[2].y - points[1].y, 2));
    std::cout << "w: " << w << " h: " << h << endl;


    cv::imshow("result", result);
    cv::waitKey(0);
    cv::destroyAllWindows();

    cv::imshow("img1", img_cropped);
    cv::waitKey(0);

    return 0;

    // vector<vector<int>> n(12);
    // for (size_t i = 0; i < locations.size(); i++) {
    //     for (size_t j = 0; j < column_location.size(); j++) {
    //         int column = column_location[j];
    //         if (column - 10 < locations[i].x && locations[i].x < column + 15) {
    //             for (size_t k = 0; k < number_top.size(); k++) {
    //                 double top = number_top[k];
    //                 if (top - 10 < locations[i].y && locations[i].y < top + 10) {
    //                     n[j].push_back(k);
    //                 }
    //             }
    //         }
    //     }
    // }

    // int b_val = n[b - 1][0];
    // int c_val = n[c - 1][0];

    // std::cout << "class:" << b_val << " " << c_val << endl;

    // cv::imshow("a", img);
    // waitKey(0);

    // return 0;
}

