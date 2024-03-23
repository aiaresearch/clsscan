#include <opencv2/opencv.hpp>
#include <vector>
#include <array>

using namespace std;
using namespace cv;

int main() {
    Mat img = imread("/home/pi/Code/clsscan/img/test.png");//imread("/home/pi/Code/clsscan/img/test.png");
    int height = img.rows;
    //img = img(Rect(0, 0, img.cols, top));
    pyrDown(img, img);
    pyrDown(img, img);
    Scalar value((255, 255), (255, 255), (255, 255));
    //copyMakeBorder(img,img,400,0,20,20,BORDER_CONSTANT,value);
    Mat img_gray, img_binary;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    adaptiveThreshold(img_gray, img_binary, 100, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 31, 30);
//    imshow("c",img_binary);
//    waitKey();

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat img_open;
    dilate(img_binary, img_open, kernel, Point(-1, -1), 3);
    erode(img_open, img_open, kernel, Point(-1, -1), 3);
    pyrDown(img_open, img_open);
    pyrDown(img, img);

    imshow("c", img_open);
    waitKey();

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);


    vector<int> xx, yy;
    vector<Point> locations;
    vector<vector<float>> contours_point = {{},
                                            {},
                                            {}};//
    //vector<int> xx, yy;
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        int x = rect.x;
        int y = rect.y;
        int w = rect.width;
        int h = rect.height;
        if (w > h && w > 10 && w < 150 && h < 150) {
            rectangle(img_open, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 1);
            locations.push_back(Point(x, y));
            //xx.push_back(x);
            //yy.push_back(y);
            contours_point[0].push_back(x);//
            contours_point[1].push_back(y);//
            contours_point[2].push_back(1);//
        }
    }
    int right_loc, left_loc;
    pair<int, Point> leftup = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) {
        int distance_squared = locations[i].x * locations[i].x + locations[i].y * locations[i].y;
        if (distance_squared < leftup.first) {
            leftup = make_pair(distance_squared, Point(locations[i]));
            left_loc = i;
        }
    }

    pair<int, Point> rightdown = make_pair(0, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) {
        int distance_squared = locations[i].x * locations[i].x + locations[i].y * locations[i].y;
        if (distance_squared > rightdown.first) {
            rightdown = make_pair(distance_squared, Point(locations[i]));
            right_loc = i;
        }
    }

    pair<int, Point> leftdown = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) {
        int distance_squared = locations[i].x * locations[i].x + (locations[i].y - height) * (locations[i].y - height);
        if (distance_squared < leftdown.first) {
            leftdown = make_pair(distance_squared, Point(locations[i]));
        }
    }
    Point leftup_point = leftup.second;
    Point rightdown_point = rightdown.second;
    Point leftdown_point = leftdown.second;
    Point rightup_point = rightdown_point - leftdown_point + leftup_point;
    //Mat img_cropped = img_open(Rect(leftup_point, rightdown_point));
    cout << leftup_point << rightdown_point << leftdown_point << rightup_point << endl;
    vector<Point> docCnt = {leftup_point, leftdown_point, rightdown_point, rightup_point};
    //cout<<docCnt << endl;

    // Mark points with circles
    vector<Point> points;
    for (size_t i = 0; i < docCnt.size(); ++i) {
        Point peak = docCnt[i];
        circle(img_open, peak, 10, Scalar(0, 0, 255), 2);
        points.push_back(peak);
    }
    imshow("a", img_open);
    waitKey();
    imwrite("/home/pi/Desktop/img_enrode.jpg", img_open);

    vector<Point2f> src = {points[0], points[1], points[2], points[3]};
    cout << points[0] << points[1] << points[2] << endl;
    vector<Point2f> dst = {{0,   0},
                           {0,   400},
                           {250, 400},
                           {250, 0}};
    Mat m = getPerspectiveTransform(src, dst);
    Mat img_finish;
    warpPerspective(img_open, img_finish, m, Size(250, 400));
    warpPerspective(img, img, m, Size(250, 400));
    imwrite("img_finish.jpg", img_finish);
    imwrite("img2.jpg", img);


}