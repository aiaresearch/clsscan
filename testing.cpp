#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>  

using namespace cv;
using namespace std;

int main() {
    vector<double> number_top = {153.0, 176.0, 199.0, 223.57142857142858, 246.0, 270.0, 294.0, 317.0, 338.0, 362.0};
    vector<int> column_location = {429, 460, 491, 522, 553, 583, 615, 646};
    int top = 745;
    int b = 5;
    int c = 6;

    Mat img = imread("test.png");
    //img = img(Rect(0, 0, img.cols, top));
    pyrDown(img, img);
    pyrDown(img, img);

    Mat img_gray, img_binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    adaptiveThreshold(img_gray, img_binary, 100, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,31, 30);
    cv::imshow("c",img_binary);
    cv::waitKey();

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

    cv::imshow("a",img_open);
    cv::waitKey();











    pair<int, Point> leftup = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) { 
        int distance_squared = xx[i] * xx[i] + yy[i] * yy[i];
        if (distance_squared < leftup.first) {
            leftup = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    pair<int, Point> rightdown = make_pair(0, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) {
        int distance_squared = xx[i] * xx[i] + yy[i] * yy[i];
        if (distance_squared > rightdown.first) {
            rightdown = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    pair<int, Point> leftdown = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) {
        int distance_squared = xx[i] * xx[i] + (yy[i]-img_open.rows) * (yy[i]-img_open.rows);
        if (distance_squared < leftdown.first) {
            leftdown = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    Point leftup_point = leftup.second;
    Point rightdown_point = rightdown.second;
    Point leftdown_point = leftdown.second;
    Point rightup_point = leftdown_point-leftup_point+rightdown_point;

    Mat img_cropped = img_open(Rect(leftup_point, rightdown_point));
    vector<vector<Point>> docCnt ;
    cout << docCnt << endl;

    // Mark points with circles
    vector<Point> points;
    for (size_t i = 0; i < docCnt.size(); ++i) {
        Point peak = docCnt[i][0];
        circle(im, peak, 10, Scalar(0, 0, 255), 2);
        points.push_back(peak);
    }
    cout << points << endl;
    imshow("im_point", im);

    // Calculate width and height using Pythagoras' theorem, then perform perspective transformation
    int h = sqrt(pow(points[1].x - points[0].x, 2) + pow(points[1].y - points[0].y, 2));
    int w = sqrt(pow(points[2].x - points[1].x, 2) + pow(points[2].y - points[1].y, 2));
    cout << "w: " << w << " h: " << h << endl;

    vector<Point2f> src = {points[0], points[1], points[2], points[3]};
    vector<Point2f> dst = {{0, 0}, {0, h}, {w, h}, {w, 0}};
    Mat m = getPerspectiveTransform(src, dst);
    Mat result;
    warpPerspective(gray, result, m, Size(w, h));

    imshow("result", result);
    waitKey(0);
    destroyAllWindows();

    imshow("img1", img_cropped);
    waitKey(0);

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

