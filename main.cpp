#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <curl/curl.h>
#include <json/json.h>

using namespace cv;
using namespace std;

void baidu_ocr(const string& image_path, const string& recognize_granularity, string& response) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        stringstream ss;
        ss << "https://aip.baidubce.com/rest/2.0/ocr/v1/numbers?access_token=24.b0c4639aeba94eb2581ad45a3296592a.2592000.1711979054.282335-48885010&recognize_granularity=" << recognize_granularity;

        curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        string image_base64;
        ifstream image_file(image_path, ios::binary);
        if (image_file) {
            stringstream buffer;
            buffer << image_file.rdbuf();
            string image_data = buffer.str();
            image_base64 = base64_encode(reinterpret_cast<const unsigned char*>(image_data.c_str()), image_data.length());
        }

        string post_fields = "image=" + image_base64;

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

        string response_buffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            response = response_buffer;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

int main() {
    system("export OMP_NUM_THREADS=1");

    cv::setNumThreads(0);
    cv::setBreakOnError(true);
    cv::setErrMode(cv::ErrModes::ECC_Fail);
    cv::setNumThreads(1);

    cv::Mat img = cv::imread("ansheet.jpg");
    cv::pyrDown(img, img);

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    cv::Mat img_binary;
    cv::adaptiveThreshold(img_gray, img_binary, 100, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 31, 30);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::Mat img_open;
    cv::dilate(img_binary, img_open, kernel, cv::Point(-1, -1), 3);
    cv::erode(img_open, img_open, kernel, cv::Point(-1, -1), 3);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cv::findContours(img_open, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

    vector<Point> locations;
    vector<int> xx;
    vector<int> yy;
    vector<Rect> boundingRects;

    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        if (rect.width > rect.height && rect.width < 150 && rect.height < 150) {
            locations.push_back(Point(rect.x, rect.y));
            xx.push_back(rect.x);
            yy.push_back(rect.y);
            boundingRects.push_back(rect);
        }
    }

    int top = img.rows;
    vector<Point> new_locations;
    vector<int> labels;

    // Perform KMeans clustering
    Mat labelsMat;
    Mat centers;
    Mat locationsMat(locations);
    kmeans(locationsMat, 2, labelsMat, TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0), 1, KMEANS_PP_CENTERS, centers);

    // Extract labels
    for (int i = 0; i < labelsMat.rows; ++i) {
        labels.push_back(labelsMat.at<int>(i, 0));
    }

    vector<int> cluster0_indices;
    vector<int> cluster1_indices;

    for (size_t i = 0; i < labels.size(); ++i) {
        if (labels[i] == 0) {
            cluster0_indices.push_back(i);
        } else {
            cluster1_indices.push_back(i);
        }
    }

    // Process cluster 0
    for (size_t i = 0; i < cluster0_indices.size(); ++i) {
        int idx = cluster0_indices[i];
        int x = boundingRects[idx].x;
        int y = boundingRects[idx].y;
        rectangle(img_open, Point(x, y), Point(x + boundingRects[idx].width, y + boundingRects[idx].height), Scalar(0, 0, 255), 1);
    }

    // Process cluster 1
    for (size_t i = 0; i < cluster1_indices.size(); ++i) {
        int idx = cluster1_indices[i];
        int x = boundingRects[idx].x;
        int y = boundingRects[idx].y;
        rectangle(img_open, Point(x, y), Point(x + boundingRects[idx].width, y + boundingRects[idx].height), Scalar(0, 0, 255), 1);
    }

    // Draw bounding rectangles
    for (size_t i = 0; i < boundingRects.size(); ++i) {
        rectangle(img_open, boundingRects[i], Scalar(0, 0, 255), 1);
    }

    // Draw centroids
    for (int i = 0; i < centers.rows; ++i) {
        int x = centers.at<float>(i, 0);
        int y = centers.at<float>(i, 1);
        circle(img_open, Point(x, y), 5, Scalar(255, 0, 0), -1);
    }

    // Cut the image to the topmost bounding box
    for (size_t i = 0; i < labels.size(); ++i) {
        if (labels[i] == 1 && boundingRects[i].y < top) {
            top = boundingRects[i].y + boundingRects[i].height;
        }
    }

    img_open = img_open(Rect(0, 0, img_open.cols, top));
    img = img(Rect(0, 0, img.cols, top));

    cv::imwrite("example.jpg", img);
    cv::imshow("img", img_open);
    cv::waitKey(0);

    curl_global_cleanup();

    return 0;
}
