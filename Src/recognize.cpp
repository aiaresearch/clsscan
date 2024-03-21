#include "recognize.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <array>

using namespace std;
using namespace cv;


    

pair<vector<Point>,int> image_proccess(const char * imgpath){ 
    Mat img = imread(imgpath);
    int height = img.rows;
    //img = img(Rect(0, 0, img.cols, top));
    pyrDown(img, img);
    pyrDown(img, img);
    
    Mat img_gray, img_binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    adaptiveThreshold(img_gray, img_binary, 100, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,31, 30);
//    imshow("c",img_binary);
//    waitKey();

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat img_open;
    dilate(img_binary, img_open, kernel, Point(-1, -1), 3);
    erode(img_open, img_open, kernel, Point(-1, -1), 3);

    

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);


    vector<int> xx, yy;
    vector<Point> locations;
    //vector<int> xx, yy;
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        int x = rect.x;
        int y = rect.y;
        int w = rect.width;
        int h = rect.height;
        if (w > h && w < 150 && h < 150) {
            rectangle(img_open, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 1);
            locations.push_back(Point(x, y));
            //xx.push_back(x);
            //yy.push_back(y);
        }
    }
    pair<vector<Point>,int> locations_height ={locations,height};
    return locations_height   ;

//    cv::imshow("a",img_open);
//    cv::waitKey();
}
void transform_img(vector<Point> locations,int *x,int *y,float *height){
    pair<int, Point> leftup = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) { 
        int distance_squared = locations[i].x *locations[i].x+ locations[i].y *locations[i].y;
        if (distance_squared < leftup.first) {
            leftup = make_pair(distance_squared, Point(locations[i]));
        }
    }

    pair<int, Point> rightdown = make_pair(0, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) {
        int distance_squared = locations[i].x *locations[i].x +locations[i].y *locations[i].y;
        if (distance_squared > rightdown.first) {
            rightdown = make_pair(distance_squared, Point(locations[i]));
        }
    }

    pair<int, Point> leftdown = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) {
        int distance_squared =locations[i].x *locations[i].x + (locations[i].y- *height) * (locations[i].y- *height);
        if (distance_squared < leftdown.first) {
            leftdown = make_pair(distance_squared, Point(locations[i]));
        }
    }
    Point leftup_point = leftup.second;
    Point rightdown_point = rightdown.second;
    Point leftdown_point = leftdown.second;
    Point rightup_point = leftdown_point-leftup_point+rightdown_point;
    //Mat img_cropped = img_open(Rect(leftup_point, rightdown_point));
    vector<Point> docCnt={leftup_point,rightdown_point,leftdown_point,rightup_point };
    //cout<<docCnt << endl;

    // Mark points with circles
    vector<Point> points;
    for (size_t i = 0; i < docCnt.size(); ++i) {
        Point peak = docCnt[i];
        points.push_back(peak);
    }
    
    vector<Point2f> src = {points[0], points[1], points[2], points[3]};
    vector<Point2f> dst = {{0, 0}, {0, *y}, {*x, *y}, {*x, 0}};
    Mat m = getPerspectiveTransform(src, dst);
    Mat result;
    //cv::warpPerspective(img_open, result, m, Size(w, h));
    
};

// pair<int,int> class(column_location,number_top){
//     vector<vector<int>> n(12);
//     for (size_t i = 0; i < locations.size(); i++) {
//         for (size_t j = 0; j < column_location.size(); j++) {
//             int column = column_location[j];
//             if (column - 10 < locations[i].x && locations[i].x < column + 15) {
//                 for (size_t k = 0; k < number_top.size(); k++) {
//                     double top = number_top[k];
//                     if (top - 10 < locations[i].y && locations[i].y < top + 10) {
//                         n[j].push_back(k);
//                     }
//                 }
//             }
//         }
//     }

//     int b_val = n[b - 1][0];
//     int c_val = n[c - 1][0];
//     pair<int,int> p(b_val,c_val);
//     return p;
// }

/*void cut (){
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

    pair<int, Point> rightup = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < xx.size(); i++) {
        int distance_squared = (xx[i]-img_open.cols) * (xx[i]-img_open.cols) + yy[i] * yy[i];
        if (distance_squared < rightup.first) {
            rightup = make_pair(distance_squared, Point(xx[i], yy[i]));
        }
    }

    Point leftup_point = leftup.second;
    Point rightdown_point = rightdown.second;
    Point leftdown_point = leftdown.second;
    //Point rightup_point = leftdown_point-leftup_point+rightdown_point;
    Point rightup_point =rightup.second;

    Mat img_cropped = img_open(Rect(leftup_point, rightdown_point));
    vector<Point> docCnt={leftup_point,rightdown_point,leftdown_point,rightup_point };
    //cout<<docCnt << endl;

    // Mark points with circles
    vector<Point> points;
    for (size_t i = 0; i < docCnt.size(); ++i) {
        Point peak = docCnt[i];
        circle(img, peak, 10, Scalar(0, 0, 255), 2);
        points.push_back(peak);
    }
    cv::pyrDown(img,img);
    std::cout << points << endl;
    cv::imshow("im_point", img);

}*/

