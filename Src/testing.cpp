#include <opencv2/opencv.hpp>
#include <vector>
#include <array>

using namespace std;
using namespace cv;

int main(){
    Mat img = imread("/home/pi/Code/clsscan/img/test.png");
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
    vector<vector<float>> contours_point={{},{},{}};//
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
            contours_point[0].push_back(x);//
            contours_point[1].push_back(y);//
            contours_point[2].push_back(1);//
        }
    }
    int *right_loc,*left_loc;
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
            *right_loc= &locations[i];
        }
    }

    pair<int, Point> leftdown = make_pair(INT_MAX, Point(-1, -1));
    for (size_t i = 0; i < locations.size(); i++) {
        int distance_squared =locations[i].x *locations[i].x + (locations[i].y- height) * (locations[i].y- height);
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
    vector<Point2f> dst = {{0, 0}, {0, 200}, {200, 200}, {200, 0}};
    Mat m = getPerspectiveTransform(src, dst);

// 二维Vector转换为Mat类型
// Create a new, _empty_ cv::Mat with the row size of OrigSamples
    cv::Mat origin_point(0, contours_point[0].size(), cv::DataType<float>::type);
    for (unsigned int i = 0; i < contours_point.size(); ++i)
    {
	 // Make a temporary cv::Mat row and add to NewSamples _without_ data copy
	    cv::Mat Sample(1, contours_point[0].size(), cv::DataType<float>::type, contours_point[i].data());
	    origin_point.push_back(Sample);
    }
    origin_point.convertTo(origin_point,6,1,0);
    Mat standard_point=m*origin_point;
    //cout<<standard_point<<endl;
    for(int i=0;i<contours_point[0].size();i++){
        float x =(float)standard_point.at<float>(0,i);
        float y =(float)standard_point.at<float>(1,i);
        locations[i]=Point (x,y);
        }
    return 0;

    //Mat result;
    //cv::warpPerspective(img_open, result, m, Size(w, h));
}
