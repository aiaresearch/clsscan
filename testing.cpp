#include <opencv2/opencv.hpp>
#include <windows.h>

using namespace cv;
using namespace std;

int main() {
    vector<double> number_top = {153.0, 176.0, 199.0, 223.57142857142858, 246.0, 270.0, 294.0, 317.0, 338.0, 362.0};
    vector<int> column_location = {429, 460, 491, 522, 553, 583, 615, 646};
    int top = 745;
    int b = 5;
    int c = 6;

    Mat img = imread("ansheet.jpg");
    img = img(Rect(0, 0, img.cols, top));
    pyrDown(img, img);

    Mat img_gray, img_binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    adaptiveThreshold(img_gray, img_binary, 100, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 31, 30);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat img_open;
    dilate(img_binary, img_open, kernel, Point(-1, -1), 3);
    erode(img_open, img_open, kernel, Point(-1, -1), 3);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);



    vector<Point> locations;
    vector<int> xx, yy;
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        int x = rect.x;
        int y = rect.y;
        int w = rect.width;
        int h = rect.height;
        if (w > h && w < 150 && h < 150) {
            rectangle(img_open, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 1);
            locations.push_back(Point(x, y));
            xx.push_back(x);
            yy.push_back(y);
        }
    }

    vector<vector<int>> n(12);
    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = 0; j < column_location.size(); j++) {
            int column = column_location[j];
            if (column - 10 < locations[i].x && locations[i].x < column + 15) {
                for (size_t k = 0; k < number_top.size(); k++) {
                    double top = number_top[k];
                    if (top - 10 < locations[i].y && locations[i].y < top + 10) {
                        n[j].push_back(k);
                    }
                }
            }
        }
    }

    int b_val = n[b - 1][0];
    int c_val = n[c - 1][0];

    cout << "class:" << b_val << " " << c_val << endl;

    imshow("a", img);
    waitKey(0);

    return 0;
}

